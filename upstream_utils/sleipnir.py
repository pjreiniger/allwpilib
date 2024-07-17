#!/usr/bin/env python3

import shutil
from pathlib import Path

from upstream_utils import (
    copy_to,
    Lib,
    temp_walk,
)


def copy_upstream_src(wpilib_root):
    wpimath = wpilib_root / "wpimath"

    # Delete old install
    for d in [
        "src/main/native/thirdparty/sleipnir/src",
        "src/main/native/thirdparty/sleipnir/include",
    ]:
        shutil.rmtree(wpimath / d, ignore_errors=True)

    # Copy Sleipnir source files into allwpilib
    src_files = [Path(dp) / f for dp, dn, fn in temp_walk(Path("src")) for f in fn]
    src_files = copy_to(src_files, wpimath / "src/main/native/thirdparty/sleipnir")

    # Copy Sleipnir header files into allwpilib
    include_files = [
        Path(dp) / f
        for dp, dn, fn in temp_walk("include")
        for f in fn
        if not f.endswith("small_vector.hpp")
    ]
    include_files = copy_to(
        include_files, wpimath / "src/main/native/thirdparty/sleipnir"
    )

    for filename in [
        ".clang-format",
        ".clang-tidy",
        ".styleguide",
        ".styleguide-license",
    ]:
        shutil.copyfile(
            filename,
            wpimath / "src/main/native/thirdparty/sleipnir" / filename,
        )


def main():
    name = "sleipnir"
    url = "https://github.com/SleipnirGroup/Sleipnir"
    # main on 2024-07-09
    tag = "b6ffa2d4fdb99cab1bf79491f715a6a9a86633b5"

    patch_list = [
        "0001-Remove-using-enum-declarations.patch",
        "0002-Use-fmtlib.patch",
        "0003-Remove-unsupported-constexpr.patch",
        "0004-Use-wpi-SmallVector.patch",
        "0005-Suppress-clang-tidy-false-positives.patch",
    ]

    sleipnir = Lib(name, url, tag, patch_list, copy_upstream_src)
    sleipnir.main()


if __name__ == "__main__":
    main()
