load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

RULES_JVM_EXTERNAL_TAG = "3.3"

RULES_JVM_EXTERNAL_SHA = "d85951a92c0908c80bd8551002d66cb23c3434409c814179c0ff026b53544dab"

http_archive(
    name = "rules_jvm_external",
    sha256 = RULES_JVM_EXTERNAL_SHA,
    strip_prefix = "rules_jvm_external-%s" % RULES_JVM_EXTERNAL_TAG,
    url = "https://github.com/bazelbuild/rules_jvm_external/archive/%s.zip" % RULES_JVM_EXTERNAL_TAG,
)

http_archive(
    name = "rules_python",
    sha256 = "b6d46438523a3ec0f3cead544190ee13223a52f6a6765a29eae7b7cc24cc83a0",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.1.0/rules_python-0.1.0.tar.gz",
)

git_repository(
    name = "wpi_bazel_deps",
    commit = "3dd22c28d7c70c682b7e10e8cff0885318cf6693",
    remote = "https://github.com/bazelRio/wpi-bazel-deps-rules",
    shallow_since = "1621309258 -0400",
)

git_repository(
    name = "wpi_bazel_rules",
    commit = "bcd12eced4b5b7243681391701260aaa46ce23cf",
    remote = "https://github.com/bazelRio/wpi-bazel-rules",
    shallow_since = "1621308850 -0400",
)

load("@wpi_bazel_deps//third_party/edu_wpi_first_thirdparty_imgui:load_imgui.bzl", "load_imgui")
load("@wpi_bazel_deps//third_party/edu_wpi_first_thirdparty_opencv:load_opencv.bzl", "load_opencv")

load_imgui(year = 2021)

load_opencv(year = 2021)

load("//bazelThirdParty:repos.bzl", "wpilib_third_party_repositories")

wpilib_third_party_repositories()
