
load("//bazel_scripts/rules:wpilib_repo.bzl", "wpilib_native_dependency")


def third_party_imgui():

    wpilib_native_dependency(
        "imgui",
        "1.79-1",
        "https://frcmaven.wpi.edu/artifactory/release/edu/wpi/first/thirdparty/frc2021",
        "94e167b9538c1330e72567d5c768dd4df1c365f766a82bc83b4034c1aecb7e11",
        [
            ("windowsx86-64", "c6fe9bbd3bf196df021f6e0a2aba3898b063ad7f91d62a4ead91343f039bb798"),
            ("linuxx86-64", "2d30b5d54bef3350d147177de1777cfcb9b346b9cff5e5cf9114b1211a93d9ce"),
            ("osxx86-64", "0e19fb2e28aaf4e3e8ae5282921102a4f0354a6f75497d306971cf738faab8ca"),
        ],
        static_only = True,
    )
