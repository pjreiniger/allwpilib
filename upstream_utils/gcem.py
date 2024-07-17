#!/usr/bin/env python3

import shutil

from upstream_utils import (
    comment_out_invalid_includes,
    walk_cwd_and_copy_if,
    Lib,
)


def copy_upstream_src(wpilib_root):
    wpimath = wpilib_root / "wpimath"

    # Delete old install
    for d in [
        "src/main/native/thirdparty/gcem/include",
    ]:
        shutil.rmtree(wpimath / d, ignore_errors=True)

    # Copy gcem include files into allwpilib
    include_files = walk_cwd_and_copy_if(
        lambda dp, f: str(dp).startswith("include"),
        wpimath / "src/main/native/thirdparty/gcem",
    )

    for f in include_files:
        comment_out_invalid_includes(
            f, [wpimath / "src/main/native/thirdparty/gcem/include"]
        )


def main():
    name = "gcem"
    url = "https://github.com/kthohr/gcem.git"
    tag = "v1.18.0"

    patch_list = [
        "0001-Call-std-functions-if-not-constant-evaluated.patch",
        "0002-Add-hypot-x-y-z.patch",
    ]

    gcem = Lib(name, url, tag, patch_list, copy_upstream_src)
    gcem.main()


if __name__ == "__main__":
    main()
