load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "segment",
    hdrs = ["segment.h", "segment_asm.h"],
)

cc_test(
    name = "benchmark_main",
    # size = "small",
    srcs = ["bench.cpp"],
    deps = [
        ":segment",
        "@catch2//:catch2_main",
    ],
    linkstatic=True,
    copts = [
         "-m64",
        "-mavx2",
        "-mfma",
        "-O3"
    ],
)

cc_test(
    name = "test_main",
    # size = "small",
    srcs = ["segment.cpp"],
    deps = [
        ":segment",
        "@catch2//:catch2_main",
    ],
    linkstatic=True,
    copts = [
         "-m64",
        "-mavx2",
        "-mfma",
        "-O3"
    ],
)
