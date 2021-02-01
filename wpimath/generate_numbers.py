import os
import sys


def main():
    MAX_NUM = 20

    dirname, _ = os.path.split(os.path.abspath(__file__))

    # This indicates this is being called by bazel
    if len(sys.argv) > 3:
        nat_file_path = sys.argv[1]
        number_file_paths = sys.argv[2:]
        rootPath = os.path.dirname(nat_file_path)

    # Called by cmake
    else:
        output_base = sys.argv[1]
        rootPath = f"{output_base}/generated/main/java/edu/wpi/first/wpiutil/math/numbers"
        nat_file_path = f"{output_base}/generated/main/java/edu/wpi/first/wpiutil/math/Nat.java"
        number_file_paths = [f"{rootPath}/N{i}.java" for i in range(MAX_NUM + 1)]

    
    generic_number_template_file = os.path.join(dirname, "src", "generate", "GenericNumber.java.in")
    with open(generic_number_template_file, "r") as templateFile:
        template = templateFile.read()

        if not os.path.exists(rootPath):
            os.makedirs(rootPath)

        for i in range(MAX_NUM + 1):
            new_contents = template.replace("${num}", str(i))
            output_file = number_file_paths[i]

            if os.path.exists(output_file):
                with open(output_file, "r") as f:
                    if f.read() == new_contents:
                        continue

            # File either doesn't exist or has different contents
            with open(output_file, "w") as f:
                f.write(new_contents)

    with open(os.path.join(f"{dirname}", "src", "generate", "Nat.java.in"), "r") as templateFile:
        template = templateFile.read()
        
        with open(os.path.join(f"{dirname}", "src", "generate", "NatGetter.java.in"),
                  "r") as getterFile:
            getter = getterFile.read()

        importsString = ""

        for i in range(MAX_NUM + 1):
            importsString += f"import edu.wpi.first.wpiutil.math.numbers.N{i};\n"
            template += getter.replace("${num}", str(i))

        template += "}\n"

        template = template.replace('{{REPLACEWITHIMPORTS}}', importsString)

        if os.path.exists(nat_file_path):
            with open(nat_file_path, "r") as f:
                if f.read() == template:
                    return 0

        # File either doesn't exist or has different contents
        with open(nat_file_path, "w") as f:
            f.write(template)


if __name__ == "__main__":
    main()
