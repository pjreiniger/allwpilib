load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Rules Python
http_archive(
    name = "rules_python",
    sha256 = "94750828b18044533e98a129003b6a68001204038dc4749f40b195b24c38f49f",
    strip_prefix = "rules_python-0.21.0",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.21.0/rules_python-0.21.0.tar.gz",
)

# Download Extra java rules
http_archive(
    name = "rules_jvm_external",
    sha256 = "f86fd42a809e1871ca0aabe89db0d440451219c3ce46c58da240c7dcdc00125f",
    strip_prefix = "rules_jvm_external-5.2",
    url = "https://github.com/bazelbuild/rules_jvm_external/releases/download/5.2/rules_jvm_external-5.2.tar.gz",
)

load("@rules_jvm_external//:repositories.bzl", "rules_jvm_external_deps")

rules_jvm_external_deps()

# TODO temporary
http_archive(
    name = "rules_bzlmodrio_toolchains",
    sha256 = "5cc49c34a56cef8560446146d4ad124b61b3af6e3068a5cdba0df09861eaba97",
    strip_prefix = "rules_bzlmodrio_toolchains-8ed5bdadbca20c0f6e7c3c1e8d0f8089fe449706",
    url = "https://github.com/bzlmodRio/rules_bzlmodrio_toolchains/archive/8ed5bdadbca20c0f6e7c3c1e8d0f8089fe449706.zip",
)

# local_repository(
#     name = "bzlmodRio",
#     path = "../bzlmodRio/monorepo/bzlmodRio",
# )
http_archive(
    name = "bzlmodRio",
    sha256 = "3e39c594f7d3dfb6ae0433a78632106667352067b3aba99ca5a311ceff4d7b3d",
    strip_prefix = "bzlmodRio-61f4f427b31cd6703691ee35d91359d38277eace",
    url = "https://github.com/bzlmodRio/bzlmodRio/archive/61f4f427b31cd6703691ee35d91359d38277eace.tar.gz",
)

load("@bzlmodRio//private/non_bzlmod:download_dependencies.bzl", "download_dependencies")

download_dependencies(
    allwpilib_version = None,
    apriltaglib_version = "3.3.0-1",
    imgui_version = "1.89.9-1",
    libssh_version = "0.105-1",
    local_monorepo_base = "../bzlmodRio/monorepo",
    navx_version = None,
    ni_version = "2024.1.1",
    opencv_version = "4.8.0-1",
    phoenix_version = None,
    revlib_version = None,
    rules_bazelrio_version = "0.0.12",
    rules_checkstyle_version = None,
    rules_pmd_version = None,
    rules_spotless_version = None,
    rules_toolchains_version = None,
    rules_wpi_styleguide_version = None,
    rules_wpiformat_version = None,
)

load("@bzlmodRio//private/non_bzlmod:setup_dependencies.bzl", "setup_dependencies")

setup_dependencies()

load("//shared/bazel/deps:repo.bzl", "load_third_party")

load_third_party()

# Initialize repositories for all packages in requirements_lock.txt.
load("@allwpilib_pip_deps//:requirements.bzl", "install_deps")

install_deps()

load("@maven//:defs.bzl", "pinned_maven_install")

pinned_maven_install()
