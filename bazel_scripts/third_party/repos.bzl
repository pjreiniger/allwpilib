

load("//bazel_scripts/third_party:com_ni_libraries.bzl", "third_party_ni_libraries")
load("//bazel_scripts/third_party:edu_wpi_first_compiler_bionic.bzl", "third_party_compiler_bionic")
load("//bazel_scripts/third_party:edu_wpi_first_compiler_raspbian.bzl", "third_party_compiler_raspbian")
load("//bazel_scripts/third_party:edu_wpi_first_compiler_roborio.bzl", "third_party_compiler_roborio")
load("//bazel_scripts/third_party:edu_wpi_first_thirdparty_opencv.bzl", "third_party_opencv")
load("//bazel_scripts/third_party:edu_wpi_first_thirdparty_imgui.bzl", "third_party_imgui")
load("//bazel_scripts/third_party:gtest.bzl", "third_party_gtest")
load("//bazel_scripts/third_party:big_maven.bzl", "third_party_big_maven")

def wpilib_third_party_repositories():

    third_party_compiler_roborio()
    third_party_compiler_bionic()
    third_party_compiler_raspbian()
    third_party_gtest()
    third_party_opencv()
    third_party_imgui()
    third_party_big_maven()
    third_party_ni_libraries()

