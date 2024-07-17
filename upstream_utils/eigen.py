#!/usr/bin/env python3

import re
import shutil
from pathlib import Path

from upstream_utils import (
    comment_out_invalid_includes,
    walk_cwd_and_copy_if,
    Lib,
)


def eigen_inclusions(dp, f):
    """Returns true if the given file in the "Eigen" include directory of the
    Eigen git repo should be copied into allwpilib

    Keyword arguments:
    dp -- directory path
    f -- filename
    """
    if not isinstance(dp, Path):
        raise Exception(f"Unexpected type: {type(dp)}, {dp}")

    if not str(dp).startswith("Eigen"):
        return False

    abspath = dp / f

    # Exclude NonMPL2.h since all non-MPL2 code will be excluded anyway
    if f == "NonMPL2.h":
        return False

    # Exclude BLAS support
    if f.endswith("_BLAS.h") or "blas" in f:
        return False

    # Exclude LAPACK support
    if f.endswith("_LAPACKE.h") or "lapack" in f:
        return False

    # Exclude MKL support
    if "MKL" in f:
        return False

    # Include architectures we care about
    if "Core/arch/" in str(abspath):
        return (
            "arch/AVX/" in str(abspath)
            or "arch/Default" in str(abspath)
            or "arch/NEON" in str(abspath)
            or "arch/SSE" in str(abspath)
        )

    # Include the following modules
    modules = [
        "Cholesky",
        "Core",
        "Eigenvalues",
        "Householder",
        "IterativeLinearSolvers",
        "Jacobi",
        "LU",
        "OrderingMethods",
        "QR",
        "SVD",
        "SparseCholesky",
        "SparseCore",
        "SparseLU",
        "SparseQR",
        "misc",
        "plugins",
    ]
    return bool(re.search(r"|".join("/" + m for m in modules), str(abspath)))


def unsupported_inclusions(dp, f):
    """Returns true if the given file in the "unsupported" include directory of
    the Eigen git repo should be copied into allwpilib

    Keyword arguments:
    dp -- directory path
    f -- filename
    """
    if not str(dp).startswith("unsupported"):
        return False

    abspath = dp / f

    # Exclude build system and READMEs
    if f == "CMakeLists.txt" or "README" in f:
        return False

    # Include the MatrixFunctions module
    return "MatrixFunctions" in str(abspath)


def copy_upstream_src(wpilib_root):
    print(type(wpilib_root), wpilib_root)
    wpimath = wpilib_root / "wpimath"
    if not isinstance(wpimath, Path):
        raise

    # Delete old install
    for d in ["src/main/native/thirdparty/eigen/include"]:
        shutil.rmtree(wpimath / d, ignore_errors=True)

    # Copy Eigen headers into allwpilib
    eigen_files = walk_cwd_and_copy_if(
        eigen_inclusions,
        wpimath / "src/main/native/thirdparty/eigen/include",
    )

    # Copy unsupported headers into allwpilib
    unsupported_files = walk_cwd_and_copy_if(
        unsupported_inclusions,
        wpimath / "src/main/native/thirdparty/eigen/include",
    )

    for f in eigen_files:
        comment_out_invalid_includes(
            f, [wpimath / "src/main/native/thirdparty/eigen/include"]
        )
    for f in unsupported_files:
        comment_out_invalid_includes(
            f, [wpimath / "src/main/native/thirdparty/eigen/include"]
        )

    shutil.copyfile(
        ".clang-format",
        wpimath / "src/main/native/thirdparty/eigen/include/.clang-format",
    )


def main():
    name = "eigen"
    url = "https://gitlab.com/libeigen/eigen.git"
    tag = "c4d84dfddc9f9edef0fdbe7cf9966d2f4a303198"
    patch_list = [
        "0001-Disable-warnings.patch",
        "0002-Intellisense-fix.patch",
        "0003-Suppress-has_denorm-and-has_denorm_loss-deprecation-.patch",
    ]

    eigen = Lib(name, url, tag, patch_list, copy_upstream_src)
    eigen.main()


if __name__ == "__main__":
    main()
