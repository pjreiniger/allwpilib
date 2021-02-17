
import os

def main():

    toolchains = []
    toolchains.append(("bionic", "bionic-compiler-%s/bionic/bin", "aarch64-bionic-linux-gnu-"))
    toolchains.append(("roborio", "roborio-compiler-%s/frc2021/roborio/bin", "arm-frc2021-linux-gnueabi-"))
    toolchains.append(("raspbian", "raspbian-compiler-%s/raspbian10/bin", "arm-raspbian10-linux-gnueabihf-"))

    tools = []
    tools.append("gcc")
    tools.append("ld")
    tools.append("ar")
    tools.append("cpp")
    tools.append("gcov")
    tools.append("nm")
    tools.append("objdump")
    tools.append("strip")

    for toolchain_name, toolchain_path, binary_prefix in toolchains:
        output_dir = os.path.join(r"C:\Users\PJ\Documents\GitHub\wpilib\original\allwpilib\bazel_scripts\toolchains", toolchain_name, "wrappers")
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        for tool in tools:
            tool_script = os.path.join(output_dir, tool + ".bat")
            win_toolchain_path = toolchain_path % "win"
            with open(tool_script, 'w') as f:
                f.write(f"@echo off\nexternal/{win_toolchain_path}/{binary_prefix}{tool}.exe %*".replace("/", "\\"))
                
            tool_script = os.path.join(output_dir, tool + ".sh")
            linux_toolchain_path = toolchain_path % "linux"
            with open(tool_script, 'w') as f:
                f.write(f'external/{linux_toolchain_path}/{binary_prefix}{tool} "$@"')
        print(output_dir)



    pass

if __name__ == "__main__":
    main()