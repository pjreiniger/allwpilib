
filegroup(
    name = "gcc",
    srcs = select({
		"@bazel_tools//src/conditions:windows": ["frc2021/roborio/bin/arm-frc2021-linux-gnueabi-gcc.exe"],
        "//conditions:default": ["frc2021/roborio/bin/arm-frc2021-linux-gnueabi-gcc"],
	}),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "ar",
    srcs = select({
		"@bazel_tools//src/conditions:windows": ["frc2021/roborio/bin/arm-frc2021-linux-gnueabi-ar.exe"],
        "//conditions:default": ["frc2021/roborio/bin/arm-frc2021-linux-gnueabi-ar"],
	}),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "includes",
    srcs = glob([
        "frc2021/roborio/arm-frc2021-linux-gnueabi/usr/lib/gcc/arm-frc2021-linux-gnueabi/7.3.0/include/**",
        "frc2021/roborio/arm-frc2021-linux-gnueabi/usr/lib/gcc/arm-frc2021-linux-gnueabi/7.3.0/include-fixed/**",
        "frc2021/roborio/arm-frc2021-linux-gnueabi/usr/include/c++/7.3.0/**",
        "frc2021/roborio/arm-frc2021-linux-gnueabi/usr/include/c++/7.3.0/arm-frc2021-linux-gnueabi/**",
        "frc2021/roborio/arm-frc2021-linux-gnueabi/usr/include/c++/7.3.0/backward/**",
        "frc2021/roborio/arm-frc2021-linux-gnueabi/usr/include/**",
    ]),

    visibility = ["//visibility:public"],
)


filegroup(
    name = "everything",
    srcs = [":ar", ":gcc", ":includes"],
    visibility = ["//visibility:public"],
)