

load("//bazel_scripts/rules:java.bzl", "wpilib_java_library")
load("//bazel_scripts/rules:cc.bzl",  "wpilib_cc_binary",  "wpilib_cc_library", "DEFAULT_CC_SRC_PATTERN", "DEFAULT_CC_INCLUDES", "DEFAULT_CC_HDR_PATTERN", "DEFAULT_CC_TEST_INCLUDE_PATTERN")


def get_default_jni_cc_srcs():
    return native.glob(DEFAULT_CC_SRC_PATTERN, exclude=["src/main/native/cpp/**/jni/**"])

def _impl(ctx):
    # https://github.com/bazelbuild/rules_scala/pull/286/files
    ctx.actions.run(
        inputs = [ctx.attr.lib[JavaInfo].outputs.native_headers],
        tools = [ctx.executable._zipper],
        outputs = ctx.outputs.outs,
        executable = ctx.executable._zipper.path,
        arguments = ["xf", ctx.attr.lib[JavaInfo].outputs.native_headers.path, "-d", ctx.outputs.outs[0].dirname],
    )


extract_native_header_jar = rule(
    implementation=_impl,
    attrs={
        "lib": attr.label(mandatory=True, allow_single_file=True),
        "outs": attr.output_list(),
        # https://github.com/bazelbuild/bazel/issues/2414
        "_zipper": attr.label(executable=True, cfg="host", default=Label("@bazel_tools//tools/zip:zipper"), allow_files=True)
    },
    output_to_genfiles = True,
)

def wpilib_jni_library(name,
                       cpp_srcs = [], cpp_hdrs = [], cpp_deps = [], includes = [],
                       java_srcs = [], java_deps = [], java_gen = [],
                       **kwargs):

    wpilib_cc_library(
        name = name + "-cpp",
        srcs = cpp_srcs or get_default_jni_cc_srcs(),
        hdrs = cpp_hdrs,
        deps = cpp_deps,
        includes = includes or DEFAULT_CC_INCLUDES,
        **kwargs,
    )

    wpilib_java_library(
        name = name + "-native-helper",
        srcs = java_srcs or native.glob(["src/main/java/**/*.java"]),
        generated_srcs = java_gen,
        deps = java_deps,
        visibility=["//visibility:public"],
    )

    jni_files = native.glob(["src/main/java/**/*JNI.java"])
    jni_files = [f[14:-5].replace("/", "_") + ".h" for f in jni_files]

    extract_native_header_jar(
        name = "extract_headers",
        outs = jni_files,
        lib = ":" + name + "-native-helper",
    )

    jni_deps = [":" + f for f in jni_files]

    wpilib_cc_library(
        name = name + "-jni-lib",
        srcs = native.glob(["src/main/native/cpp/jni/**/*.cpp"]),
        hdrs = [":" + name + "-native-helper"] + jni_deps,
        includes = ["."],
        deps = [
            "//bazel_scripts/rules:copy_jni_hdr_lib",
            ":" + name + "-cpp",
        ],
        alwayslink = True,
    )

    wpilib_cc_binary(
        name = name + "jni",
        linkshared = True,
        deps = [":" + name + "-jni-lib"],
        visibility=["//visibility:public"],
    )

    native.genrule(
        name = name + "jni-jar",
        srcs = [
                ":" + name + "jni",
            ],
        outs = [name + "jni.jar"],
        cmd = "$(locations @//bazel_scripts/rules/gen:create_jni_jar) $@ $(SRCS)",
        tools = ["@//bazel_scripts/rules/gen:create_jni_jar"],
    )


    wpilib_java_library(
        name = name + "-java",
        srcs = java_srcs or native.glob(["src/main/java/**/*.java"]),
        generated_srcs = java_gen,
        deps = java_deps,
        visibility=["//visibility:public"],
        runtime_deps = [":" + name + "jni-jar"]
    )
