load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")


def third_party_compiler_roborio():
    http_archive(
        name = "roborio-compiler-win",
        sha256 = "f3e9ba32b63d3cd26e242feeb14e878fecbda86c19c12b98c3084c629e06acb3",
        url = "https://github.com/wpilibsuite/roborio-toolchain/releases/download/v2021-2/FRC-2021-Windows-Toolchain-7.3.0.zip",
        build_file = "roborio-compiler/BUILD",
    )

    http_archive(
        name = "roborio-compiler-linux",
        sha256 = "00cc58bf0607d71e725919d28e22714ce1920692c4864bc86353fc8139cbf7b7",
        url = "https://github.com/wpilibsuite/roborio-toolchain/releases/download/v2021-2/FRC-2021-Linux-Toolchain-7.3.0.tar.gz",
        build_file = "roborio-compiler/BUILD",
    )

    http_archive(
        name = "roborio-compiler-mac",
        sha256 = None, #"f3e9ba32b63d3cd26e242feeb14e878fecbda86c19c12b98c3084c629e06acb3",
        url = "https://github.com/wpilibsuite/roborio-toolchain/releases/download/v2021-2/FRC-2021-Mac-Toolchain-7.3.0.tar.gz",
        build_file = "roborio-compiler/BUILD",
    )