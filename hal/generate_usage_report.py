#!/usr/bin/env python3

# Copyright (c) FIRST and other WPILib contributors.
# Open Source Software; you can modify and/or share it under the terms of
# the WPILib BSD license file in the root directory of this project.


import os


def main():
    dirname, _ = os.path.split(os.path.abspath(__file__))
    print(dirname)

    os.makedirs(f"{dirname}/src/generated/main/native/include/hal", exist_ok=True)
    os.makedirs(f"{dirname}/src/generated/main/java/edu/wpi/first/hal", exist_ok=True)

    # Create contents
    usage_reporting_types_cpp = ""
    usage_reporting_types_java = ""
    usage_reporting_instances_cpp = ""
    usage_reporting_instances_java = ""

    with open(os.path.abspath(f"{dirname}/src/generate/Instances.txt")) as instances:
        for instance in instances:
            usage_reporting_instances_cpp += f"    {instance.strip()},\n"
            usage_reporting_instances_java += f"    public static final int {instance.strip()};\n"

    with open(f"{dirname}/src/generate/ResourceType.txt") as resource_types:
        for resource_type in resource_types:
            usage_reporting_types_cpp += f"    {resource_type.strip()},\n"
            usage_reporting_types_java += f"    public static final int {resource_type.strip()};\n"

    # Run template'ing
    with open(f"{dirname}/src/generate/FRCNetComm.java.in") as java_usage_reporting:
        contents = java_usage_reporting.read()
        contents = contents.replace(r"${usage_reporting_types}", usage_reporting_types_java)
        contents = contents.replace(r"${usage_reporting_instances}", usage_reporting_instances_java)
        with open(f"{dirname}/src/generated/main/java/edu/wpi/first/hal/FRCNetComm.java", "w") as java_out:
            java_out.write(contents)

    with open(f"{dirname}/src/generate/FRCUsageReporting.h.in") as cpp_usage_reporting:
        contents = cpp_usage_reporting.read()
        contents = contents.replace(r"${usage_reporting_types_cpp}",
                         usage_reporting_types_cpp)
        contents = contents.replace(
                r"${usage_reporting_instances_cpp}", usage_reporting_instances_cpp)
        with open(f"{dirname}/src/generated/main/native/include/hal/FRCUsageReporting.h", "w") as cpp_out:
            cpp_out.write(contents)


if __name__ == "__main__":
    main()