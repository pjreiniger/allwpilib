
load("//bazel_scripts/rules:wpilib_repo.bzl", "wpilib_native_dependency")


def third_party_ni_libraries():

    wpilib_native_dependency(
        "chipobject",
        "2020.10.1",
        "https://frcmaven.wpi.edu/artifactory/release/edu/wpi/first/ni-libraries",
        "3ea073ce8cbd7a117fc944073c8ef0d1a49f356a7ac87f6beaf33a50a581abe9",
        [("linuxathena", None)]
    )

    wpilib_native_dependency(
        "netcomm",
        "2020.10.1",
        "https://frcmaven.wpi.edu/artifactory/release/edu/wpi/first/ni-libraries",
        "f4693644d9bc0e481e42a3f46cc781a5658e1e68085014bc38fbb626b8094ec8",
        [("linuxathena", None)]
    )

    wpilib_native_dependency(
        "visa",
        "2020.10.1",
        "https://frcmaven.wpi.edu/artifactory/release/edu/wpi/first/ni-libraries",
        "909927c9b68fbc6c0955596e1faf6addf47a95675f1e881dee1fadce4436723c",
        [("linuxathena", None)]
    )
