#!/usr/bin/env python3

import shutil
import subprocess
from pathlib import Path

from upstream_utils import (
    walk_cwd_and_copy_if,
    Lib,
    temp_chdir,
)


def copy_upstream_src(wpilib_root: Path):
    wpiutil = wpilib_root / "wpiutil"

    # Delete old install
    for d in [
        "src/main/native/thirdparty/mpack/src",
        "src/main/native/thirdparty/mpack/include",
    ]:
        shutil.rmtree(wpiutil / d, ignore_errors=True)

    # Run the amalgmation script
    subprocess.check_call(["bash", "tools/amalgamate.sh"])

    # Copy the files
    amalgamation_source_dir = Path(".") / ".build" / "amalgamation" / "src" / "mpack"
    temp_chdir(amalgamation_source_dir)

    walk_cwd_and_copy_if(
        lambda dp, f: f.endswith(".h"),
        wpiutil / "src/main/native/thirdparty/mpack/include/wpi",
    )
    walk_cwd_and_copy_if(
        lambda dp, f: f.endswith(".c"),
        wpiutil / "src/main/native/thirdparty/mpack/src",
    )


def main():
    name = "mpack"
    url = "https://github.com/ludocode/mpack"
    tag = "v1.1.1"

    patch_list = [
        "0001-Don-t-emit-inline-defs.patch",
        "0002-Update-amalgamation-script.patch",
        "0003-Use-namespace-for-C.patch",
        "0004-Group-doxygen-into-MPack-module.patch",
    ]

    mpack = Lib(name, url, tag, patch_list, copy_upstream_src)
    mpack.main()


if __name__ == "__main__":
    main()
