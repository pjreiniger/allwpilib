
load("@rules_java//java:defs.bzl", "java_binary", "java_library", "java_test")
load("//bazel_scripts/rules/gen:flat_copy_files.bzl",  "flat_copy_files")


def wpilib_java_library(name, srcs = [], resources = [], generated_srcs = [], **kwargs):

    full_srcs = generated_srcs + (srcs or native.glob(["src/main/java/**/*.java"]))

    java_library(
        name = name,
        srcs = full_srcs,
        resources = resources or native.glob(["src/main/resources/**"]),
        **kwargs
    )

def wpilib_java_test(name, lib_directory = "", deps=[], srcs = [], resources = [], runtime_deps = [], jni_deps = [], package = "edu", **kwargs):

    prepared_deps = []
    if jni_deps:
        flat_copy_files(
            name = name + "_prepare_jni_deps",
            targeted_filegroups = jni_deps,
            output_directory = select({
                "@bazel_tools//src/conditions:windows": name + ".exe.runfiles/__main__",
                "//conditions:default": name + ".runfiles/__main__",
            }),
        )
        prepared_deps = [":" + name + "_prepare_jni_deps"]

    java_test(
        name = name,
        srcs = srcs or native.glob(["src/test/java/**/*.java",]),
        args = ["--select-package", package],
        main_class = "org.junit.platform.console.ConsoleLauncher",
        use_testrunner = False,
        deps = deps + [
            "@big_maven//:org_junit_jupiter_junit_jupiter_api",
            "@big_maven//:org_junit_jupiter_junit_jupiter_params",
            "@big_maven//:org_junit_jupiter_junit_jupiter_engine",
        ],
        runtime_deps = runtime_deps + [
            "@big_maven//:org_junit_platform_junit_platform_commons",
            "@big_maven//:org_junit_platform_junit_platform_console",
            "@big_maven//:org_junit_platform_junit_platform_engine",
            "@big_maven//:org_junit_platform_junit_platform_launcher",
            "@big_maven//:org_junit_platform_junit_platform_suite_api",
        ],
        data = prepared_deps,
        resources = resources or native.glob(["src/test/resources/**"]),

        env = select({
                "@bazel_tools//src/conditions:windows": {},
                "//conditions:default": {"LD_LIBRARY_PATH": lib_directory + "/" + name + ".runfiles/__main__"},
            }),
        jvm_flags = select({
                "@bazel_tools//src/conditions:windows": [],
                "//conditions:default": ["-Djava.library.path=" + lib_directory + "/" + name + ".runfiles/__main__"],
            }),
        **kwargs,

    )


def wpilib_java_binary(name, main_class, srcs = [], jni_deps = [], data = [], resources = [], combined_runtime_libs = [], **kwargs):
    pass

    prepared_deps = []
    if jni_deps:
        flat_copy_files(
            name = name + "_prepare_jni_deps",
            targeted_filegroups = jni_deps,
            output_directory = "jni_libs",
        )
        prepared_deps = [":" + name + "_prepare_jni_deps"]

    java_library(
        name = name,
        # main_class = main_class,
        # jvm_flags = ["-Djava.library.path=../jni_libs"],
        srcs = srcs or native.glob(["src/main/java/**/*.java"]),
        resources = combined_runtime_libs + (resources or native.glob(["src/main/resources/**"])),
        data = data or prepared_deps,
        **kwargs,
    )
