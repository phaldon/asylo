#
# Copyright 2018 Asylo authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

licenses(["notice"])  # Apache v2.0

# This package defines proto libraries used in enclave auth{N,Z}.

load("//asylo/bazel:proto.bzl", "asylo_proto_library")

asylo_proto_library(
    name = "identity_proto",
    srcs = ["identity.proto"],
    visibility = ["//visibility:public"],
)

asylo_proto_library(
    name = "identity_acl_proto",
    srcs = ["identity_acl.proto"],
    visibility = ["//visibility:public"],
    deps = [":identity_proto"],
)

asylo_proto_library(
    name = "enclave_assertion_authority_config_proto",
    srcs = ["enclave_assertion_authority_config.proto"],
    visibility = ["//visibility:public"],
    deps = [":identity_proto"],
)

asylo_proto_library(
    name = "sealed_secret_proto",
    srcs = ["sealed_secret.proto"],
    visibility = ["//visibility:public"],
    deps = [
        ":identity_acl_proto",
        ":identity_proto",
    ],
)

cc_library(
    name = "init",
    srcs = ["init_internal.h"],
    hdrs = ["init.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":enclave_assertion_authority_config_proto_cc",
        "//asylo/identity:enclave_assertion_authority",
        "//asylo/identity:enclave_assertion_generator",
        "//asylo/identity:enclave_assertion_verifier",
        "//asylo/util:status",
        "@com_google_asylo//asylo/util:logging",
    ],
)

cc_test(
    name = "init_test",
    srcs = ["init_test.cc"],
    tags = ["regression"],
    deps = [
        ":enclave_assertion_authority_config_proto_cc",
        ":init",
        "//asylo/identity/null_identity:null_assertion_generator",
        "//asylo/identity/null_identity:null_assertion_verifier",
        "//asylo/test/util:status_matchers",
        "//asylo/test/util:test_main",
        "@com_google_googletest//:gtest",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_library(
    name = "secret_sealer",
    srcs = ["secret_sealer.cc"],
    hdrs = ["secret_sealer.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":identity_proto_cc",
        ":sealed_secret_proto_cc",
        "//asylo/identity/util:byte_container_util",
        "//asylo/identity/util:byte_container_view",
        "//asylo/platform/common:static_map",
        "//asylo/util:cleansing_types",
        "//asylo/util:status",
    ],
)

cc_library(
    name = "identity_acl_evaluator",
    srcs = ["identity_acl_evaluator.cc"],
    hdrs = ["identity_acl_evaluator.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":identity_acl_proto_cc",
        ":identity_expectation_matcher",
        ":identity_proto_cc",
        "//asylo/util:status",
        "@com_google_absl//absl/strings",
        "@com_google_protobuf//:protobuf_lite",
    ],
)

cc_library(
    name = "identity_expectation_matcher",
    srcs = [
        "delegating_identity_expectation_matcher.cc",
        "named_identity_expectation_matcher.cc",
    ],
    hdrs = [
        "delegating_identity_expectation_matcher.h",
        "identity_expectation_matcher.h",
        "named_identity_expectation_matcher.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":identity_proto_cc",
        "//asylo/identity/util:byte_container_util",
        "//asylo/identity/util:byte_container_view",
        "//asylo/platform/common:static_map",
        "//asylo/util:status",
        "@com_google_absl//absl/strings",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_test(
    name = "identity_expectation_matcher_test",
    srcs = ["identity_expectation_matcher_test.cc"],
    tags = ["regression"],
    deps = [
        ":identity_expectation_matcher",
        ":identity_proto_cc",
        "//asylo/platform/common:static_map",
        "//asylo/test/util:status_matchers",
        "//asylo/test/util:test_main",
        "//asylo/util:status",
        "@com_google_googletest//:gtest",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_library(
    name = "enclave_assertion_authority",
    hdrs = ["enclave_assertion_authority.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":identity_proto_cc",
        "//asylo/identity/util:byte_container_util",
        "//asylo/util:status",
    ],
)

cc_library(
    name = "enclave_assertion_generator",
    hdrs = ["enclave_assertion_generator.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":enclave_assertion_authority",
        ":identity_proto_cc",
        "//asylo/platform/common:static_map",
        "//asylo/util:status",
    ],
)

cc_library(
    name = "enclave_assertion_verifier",
    hdrs = ["enclave_assertion_verifier.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":enclave_assertion_authority",
        ":identity_proto_cc",
        "//asylo/platform/common:static_map",
        "//asylo/util:status",
    ],
)
