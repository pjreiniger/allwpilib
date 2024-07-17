#!/usr/bin/env python3

import shutil
from pathlib import Path

from upstream_utils import (
    Lib,
)


def copy_upstream_src(wpilib_root: Path):
    wpiutil = wpilib_root / "wpiutil"

    # Copy expected header into allwpilib
    dest_filename = wpiutil / "src/main/native/thirdparty/expected/include/wpi/expected"
    shutil.copyfile("include/tl/expected.hpp", dest_filename)

    # Rename namespace from tl to wpi
    with dest_filename.open(encoding="utf-8") as f:
        content = f.read()
    content = content.replace("namespace tl", "namespace wpi")
    content = content.replace("tl::", "wpi::")
    content = content.replace("TL_", "WPI_")

    dest_filename.write_text(content)


def main():
    name = "expected"
    url = "https://github.com/TartanLlama/expected"
    # master on 2024-01-25
    tag = "3f0ca7b19253129700a073abfa6d8638d9f7c80c"

    expected = Lib(name, url, tag, [], copy_upstream_src)
    expected.main()


if __name__ == "__main__":
    main()
