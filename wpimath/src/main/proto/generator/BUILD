
load("@allwpilib_pip_deps//:requirements.bzl", "requirement")

py_library(
    name = "helpers",
    srcs = glob(["*_helpers.py"]),
    deps = ["//wpimath/src/main/proto:py_proto", requirement("jinja2")],
    visibility = ['//visibility:public']
)
