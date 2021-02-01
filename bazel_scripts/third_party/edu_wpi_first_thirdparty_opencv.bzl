
load("//bazel_scripts/rules:wpilib_repo.bzl", "wpilib_native_dependency")
load("@rules_jvm_external//:defs.bzl", "maven_install")


def third_party_opencv():

    maven_install(
        name = "opencv",
        artifacts = ["edu.wpi.first.thirdparty.frc2021.opencv:opencv-java:3.4.7-5"],
        repositories = ["https://frcmaven.wpi.edu/artifactory/release/"],
    )

    wpilib_native_dependency(
        "opencv-cpp",
        "3.4.7-5",
        "https://frcmaven.wpi.edu/artifactory/release/edu/wpi/first/thirdparty/frc2021/opencv",
        "48852219062e2f9f4b348996d4e23bda3cad6a777a97ee9efa33031115780c44",
        [
            ("linuxaarch64bionic", "2dc903682613e33e9849156b9044cdc2d3a56ef23fea53673bc73abd1b896fc8"),
            ("linuxathena", "927e12ad2fa5d890504a3891e658493015d2b2cf9444492f0839e63a67f62fe3"),
            ("linuxraspbian", "be8ee220510301daff92e11ce4598e1237e369fd97b09e4d96d38b67911d56f3"),
            ("linuxx86-64", "b4f80e276e12b940f450740cd05c3c9c0e6e879153660b3ef1c3297af7dad12f"),
            ("osxx86-64", "bf6321427b66a31f4330398e7cfd9a78209fd769ac898dd8264f914561c023f8"),
            ("windowsx86-64", "d8723747f4d5a69c693951ec6cf0fb649f8a644786783b936bc15148cceebb38"),
        ]
    )
