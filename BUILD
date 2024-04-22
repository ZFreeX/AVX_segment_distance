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
        "//tools/bazel:catch2",
    ],
    copts = [
        "-m64",
        "-mavx512f",
        "-march=skylake-avx512",
        "-mprefer-vector-width=512",     
        "-masm=intel",
        "-fverbose-asm", 
        "-O3",
    ],
)

cc_test(
    name = "test_main",
    # size = "small",
    srcs = ["segment.cpp"],
    deps = [
        ":segment",
        "//tools/bazel:catch2",
    ],
    copts = [
        "-m64",
        "-mavx512f",
    ],
)
