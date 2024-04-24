#include "segment.h"
#include "segment_asm.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <immintrin.h>
#include <random>

const uint32_t kTestCasesCount = 128; // should be power of 2 for performance
constexpr uint32_t kMod = kTestCasesCount - 1;

struct TestCase {
    double a[4];
    double b[4];
};

std::vector<TestCase> GenerateTestCases(int n) {
    std::random_device gen;
    std::mt19937 rnd(gen());
    std::uniform_int_distribution<int> dist{-100, 100};

    std::vector<TestCase> cases(n);

    for (int i = 0; i < n; i++) {
        for (double e : cases[i].a) {
            e = static_cast<double>(dist(rnd));
        }
        for (double e : cases[i].b) {
            e = static_cast<double>(dist(rnd));
        }
    }

    return cases;
}


TEST_CASE("Bench Distance") {
    BENCHMARK_ADVANCED("Bench Distance, CPP")(Catch::Benchmark::Chronometer meter) {
            auto cases = GenerateTestCases(kTestCasesCount);

            meter.measure([&](int i) {
                TestCase &test_case = cases[i & kMod];
                return seg_distance(test_case.a[0],test_case.a[1],test_case.a[2],test_case.a[3],test_case.b[0],test_case.b[1],test_case.b[2],test_case.b[3]);
            });
        };

    BENCHMARK_ADVANCED("Bench Distance, AVX")(Catch::Benchmark::Chronometer meter) {
            auto cases = GenerateTestCases(kTestCasesCount);

            meter.measure([&](int i) {
                TestCase &test_case = cases[i & kMod];
                auto s1 = _mm256_setr_pd(test_case.a[0],test_case.a[1],test_case.a[2],test_case.a[3]);
                auto s2 = _mm256_setr_pd(test_case.b[0],test_case.b[1],test_case.b[2],test_case.b[3]);
                return seg_distance(s1, s2);
            });
        };
}