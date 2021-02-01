
load("@rules_jvm_external//:defs.bzl", "maven_install")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_jar")
load("@bazel_tools//tools/build_defs/repo:jvm.bzl", "jvm_maven_import_external")

def third_party_big_maven():

    JACKSON_VERSION = "2.10.0"
    JUNIT_JUPITER_VERSION = "5.6.1"
    JUNIT_PLATFORM_VERSION = "1.6.1"

    maven_install(
        name = "big_maven",
        artifacts = [

            "com.fasterxml.jackson.core:jackson-annotations:" + JACKSON_VERSION,
            "com.fasterxml.jackson.core:jackson-core:" + JACKSON_VERSION,
            "com.fasterxml.jackson.core:jackson-databind:" + JACKSON_VERSION,
            'org.junit.jupiter:junit-jupiter-api:' + JUNIT_JUPITER_VERSION,
            'org.junit.jupiter:junit-jupiter-params:' + JUNIT_JUPITER_VERSION,
            'org.junit.jupiter:junit-jupiter-engine:' + JUNIT_JUPITER_VERSION,
            "org.junit.platform:junit-platform-commons:" + JUNIT_PLATFORM_VERSION,
            "org.junit.platform:junit-platform-console:" + JUNIT_PLATFORM_VERSION,
            "org.junit.platform:junit-platform-engine:" + JUNIT_PLATFORM_VERSION,
            "org.junit.platform:junit-platform-launcher:" + JUNIT_PLATFORM_VERSION,
            "org.junit.platform:junit-platform-suite-api:" + JUNIT_PLATFORM_VERSION,
            'com.google.guava:guava:21.0',
            'org.mockito:mockito-core:2.27.0',
            "org.ejml:ejml-simple:0.38",
        ],
        repositories = ["https://repo1.maven.org/maven2"],
    )
