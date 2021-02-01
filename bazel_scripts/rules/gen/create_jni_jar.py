import sys
import os
from zipfile import ZipFile


def main():
    output_file = sys.argv[1]
    libraries = sys.argv[2:]

    from sys import platform
    if platform == "linux" or platform == "linux2":
        output_directory = os.path.join("linux", "x86-64")
    elif platform == "darwin":
        output_directory = os.path.join("osx", "x86-64")
    elif platform == "win32":
        output_directory = os.path.join("windows", "x86-64")
    else:
        raise Exception(f"Uh oh '{platform}''")

    for lib in libraries:
        print(os.path.basename(lib))
    with ZipFile(output_file, 'w') as zip:
        # writing each file one by one

        for file in libraries:
            import uuid
            random_hash = uuid.uuid4().hex

            hash_name = os.path.basename(lib).split(".")[0] + '.hash'
            zip.write(file,
                      arcname=os.path.join(output_directory,
                                           os.path.basename(lib)))
            zip.writestr(os.path.join(output_directory, hash_name), random_hash)


if __name__ == "__main__":
    main()
