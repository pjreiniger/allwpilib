
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")


def third_party_gtest():
    git_repository(
        name = "gtest",
        remote = "https://github.com/google/googletest",
        commit = "703bd9caab50b139428cea1aaff9974ebee5742e",
        shallow_since = "1570114335 -0400"
    )
