#!/usr/bin/env python3

import shutil
from pathlib import Path

from upstream_utils import (
    walk_cwd_and_copy_if,
    Lib,
)


def copy_upstream_src(wpilib_root: Path):
    wpinet = wpilib_root / "wpinet"

    # Delete old install
    for d in ["src/main/native/thirdparty/libuv"]:
        shutil.rmtree(wpinet / d, ignore_errors=True)

    include_ignorelist = [
        "aix.h",
        "os390.h",
        "stdint-msvc2008.h",
        "sunos.h",
    ]

    include_files = walk_cwd_and_copy_if(
        lambda dp, f: dp.is_relative_to("include") and f not in include_ignorelist,
        wpinet / "src/main/native/thirdparty/libuv",
    )

    src_ignorelist = [
        "aix-common.c",
        "aix.c",
        "bsd-proctitle.c",
        "darwin-stub.c",
        "haiku.c",
        "hurd.c",
        "os390-proctitle.c",
        "os390-syscalls.c",
        "os390-syscalls.h",
        "os390.c",
        "qnx.c",
        "sunos.c",
        "sysinfo-loadavg.c",
        "sysinfo-memory.c",
    ]
    src_files = walk_cwd_and_copy_if(
        lambda dp, f: dp.is_relative_to("src") and f not in src_ignorelist,
        wpinet / "src/main/native/thirdparty/libuv",
    )


def main():
    name = "libuv"
    url = "https://github.com/libuv/libuv"
    tag = "v1.48.0"

    patch_list = [
        "0001-Revert-win-process-write-minidumps-when-sending-SIGQ.patch",
        "0002-Fix-missing-casts.patch",
        "0003-Fix-warnings.patch",
        "0004-Preprocessor-cleanup.patch",
        "0005-Cleanup-problematic-language.patch",
        "0006-Fix-Win32-warning-suppression-pragma.patch",
        "0007-Use-C-atomics.patch",
        "0008-Remove-static-from-array-indices.patch",
        "0009-Add-pragmas-for-missing-libraries-and-set-_WIN32_WIN.patch",
        "0010-Remove-swearing.patch",
    ]

    libuv = Lib(name, url, tag, patch_list, copy_upstream_src)
    libuv.main()


if __name__ == "__main__":
    main()
