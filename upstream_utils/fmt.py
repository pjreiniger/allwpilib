#!/usr/bin/env python3

import shutil

from upstream_utils import (
    comment_out_invalid_includes,
    walk_cwd_and_copy_if,
    Lib,
)


def copy_upstream_src(wpilib_root):
    wpiutil = wpilib_root / "wpiutil"

    # Delete old install
    for d in [
        "src/main/native/thirdparty/fmtlib/src",
        "src/main/native/thirdparty/fmtlib/include",
    ]:
        shutil.rmtree(wpiutil / d, ignore_errors=True)

    # Copy fmt source files into allwpilib
    src_files = walk_cwd_and_copy_if(
        lambda dp, f: str(dp).startswith("src") and f.endswith(".cc") and f != "fmt.cc",
        wpiutil / "src/main/native/thirdparty/fmtlib",
    )

    # Copy fmt header files into allwpilib
    include_files = walk_cwd_and_copy_if(
        lambda dp, f: str(dp).startswith("include/fmt"),
        wpiutil / "src/main/native/thirdparty/fmtlib",
    )

    for f in src_files:
        comment_out_invalid_includes(
            f, [wpiutil / "src/main/native/thirdparty/fmtlib/include"]
        )
    for f in include_files:
        comment_out_invalid_includes(
            f, [wpiutil / "src/main/native/thirdparty/fmtlib/include"]
        )


def main():
    name = "fmt"
    url = "https://github.com/fmtlib/fmt"
    tag = "11.0.1"

    patch_list = ["0001-Suppress-warnings-we-can-t-fix.patch"]

    fmt = Lib(name, url, tag, patch_list, copy_upstream_src)
    fmt.main()


if __name__ == "__main__":
    main()
