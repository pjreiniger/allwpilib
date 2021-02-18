load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")


def third_party_compiler_bionic():
    http_archive(
        name = "bionic-compiler-win",
        sha256 = "d2109a24142b0d835c59d4d019033b620428a1795ce08cc02395df40722fdb86",
        url = "https://github.com/wpilibsuite/aarch64-bionic-toolchain/releases/download/v1.1.1/Bionic-Windows-Toolchain-8.3.0.zip",
        build_file = "bionic-compiler/BUILD",
    )
    
    http_archive(
        name = "bionic-compiler-linux",
        sha256 = None, #"d2109a24142b0d835c59d4d019033b620428a1795ce08cc02395df40722fdb86",
        url = "https://github.com/wpilibsuite/aarch64-bionic-toolchain/releases/download/v1.1.1/Bionic-Linux-Toolchain-8.3.0.zip",
        build_file = "bionic-compiler/BUILD",
    )
    
    http_archive(
        name = "bionic-compiler-mac",
        sha256 = None, #"d2109a24142b0d835c59d4d019033b620428a1795ce08cc02395df40722fdb86",
        url = "https://github.com/wpilibsuite/aarch64-bionic-toolchain/releases/download/v1.1.1/Bionic-Mac-Toolchain-8.3.0.zip",
        build_file = "bionic-compiler/BUILD",
    )