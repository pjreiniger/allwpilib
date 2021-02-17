load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")


def third_party_compiler_roborio():
    http_archive(
        name = "roborio-compiler-win",
        # strip_prefix = "rules_jvm_external-%s" % RULES_JVM_EXTERNAL_TAG,
        sha256 = "f3e9ba32b63d3cd26e242feeb14e878fecbda86c19c12b98c3084c629e06acb3",
        url = "https://github.com/wpilibsuite/roborio-toolchain/releases/download/v2021-2/FRC-2021-Windows-Toolchain-7.3.0.zip",
        build_file = "roborio-compiler/BUILD",
    )