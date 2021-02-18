load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")


def third_party_compiler_raspbian():
    http_archive(
        name = "raspbian-compiler-win",
        sha256 = "3f49461d4f7d2a0ccd3f4e56815dab01c6c3af544467ee3035e23c6a330d7988",
        url = "https://github.com/wpilibsuite/raspbian-toolchain/releases/download/v2.1.0/Raspbian10-Windows-Toolchain-8.3.0.zip",
        build_file = "raspbian-compiler/BUILD",
    )
    
    http_archive(
        name = "raspbian-compiler-linux",
        sha256 = None, #"3f49461d4f7d2a0ccd3f4e56815dab01c6c3af544467ee3035e23c6a330d7988",
        url = "https://github.com/wpilibsuite/raspbian-toolchain/releases/download/v2.1.0/Raspbian10-Linux-Toolchain-8.3.0.tar.gz",
        build_file = "raspbian-compiler/BUILD",
    )
    
    http_archive(
        name = "raspbian-compiler-mac",
        sha256 = None, #"3f49461d4f7d2a0ccd3f4e56815dab01c6c3af544467ee3035e23c6a330d7988",
        url = "https://github.com/wpilibsuite/raspbian-toolchain/releases/download/v2.1.0/Raspbian10-Mac-Toolchain-8.3.0.tar.gz",
        build_file = "raspbian-compiler/BUILD",
    )