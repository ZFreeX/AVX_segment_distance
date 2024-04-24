#include "segment.h"
#include "segment_asm.h"
#include <immintrin.h>
#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>
#include <deque>
#include <iterator>
#include <list>
#include <memory>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <chrono>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

TEST_CASE("Single_distance") {
    REQUIRE(seg_distance(1., 1., 2., 2., 2., 1., 3., 0.) == Catch::Approx(0.70710678118654752));
    REQUIRE(
        seg_distance(_mm256_setr_pd(1., 1., 2., 2.), _mm256_setr_pd(2., 1., 3., 0.)) ==
        Catch::Approx(0.70710678118654752));

    //(0, 0, 4, 3) (1, 1, 5, 5) 0.2
    REQUIRE(seg_distance(0, 0, 4, 3, 1, 1, 5, 5) == Catch::Approx(0.2));
    REQUIRE(
        seg_distance(_mm256_setr_pd(1, 1, 5, 5), _mm256_setr_pd(0, 0, 4, 3)) == Catch::Approx(0.2));
}

TEST_CASE("Intersec group test") {
    bool r1 = intersec(_mm256_setr_pd(2., 2., 4., 4.), _mm256_setr_pd(5., 5., 6., 6.));
    bool r2 = intersec(2, 2, 4, 4, 5, 5, 6, 6);
    REQUIRE(r1 == r2);
    REQUIRE(
        intersec(_mm256_setr_pd(2., 2., 4., 2.), _mm256_setr_pd(4., 2., 2., 2.)) ==
        intersec(2, 2, 4, 2, 4, 2, 2, 2));
}

TEST_CASE("Random dist test group") {
    const int lim = 100;
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    for (int i = 0; i < 100; i++) {
        std::vector<int> x(4);
        std::vector<int> y(4);
        // point - x1, y1
        // seg - [x2, y2, x3, y3]
        for (auto& e : x) {
            e = rng() % lim - 5;
        }
        for (auto& e : y) {
            e = rng() % lim - 5;
        }

        REQUIRE(
            dist(_mm256_setr_pd(x[0], y[0], x[0], y[0]), _mm256_setr_pd(x[1], y[1], x[2], y[2])) ==
            Catch::Approx(dist(x[0], y[0], x[1], y[1], x[2], y[2])));
    }
}

TEST_CASE("Dist_sqr_test") {
    {
        __m256d vec1 = _mm256_setr_pd(4.0, 5.0, 0., 0.);
        __m256d vec2 = _mm256_setr_pd(9.0, 3.0, 0., 0.);

        REQUIRE(dist_sqr(vec1, vec2) == Catch::Approx(29));
    }
    {
        __m256d vec1 = _mm256_setr_pd(9.0, 5.0, 0., 0.);
        __m256d vec2 = _mm256_setr_pd(9.0, 3.0, 0., 0.);

        REQUIRE(dist_sqr(vec1, vec2) == Catch::Approx(4));
    }
}

TEST_CASE("Min-Max") {
    __m256d vec1 = _mm256_setr_pd(4.0, 5.0, 0., 0.);
    __m256d vec2 = _mm256_setr_pd(9.0, 3.0, 0., 0.);

    REQUIRE(min_x(vec1) == Catch::Approx(0.));
    REQUIRE(max_x(vec2) == Catch::Approx(9.0));
    REQUIRE(max_y(vec2) == Catch::Approx(3.0));
    REQUIRE(min_y(vec1) == Catch::Approx(0.0));
}

TEST_CASE("Intersec") {
    __m256d vec1 = _mm256_setr_pd(2., 4., 10., 4.);
    __m256d vec2 = _mm256_setr_pd(2., 2., 6., 6.);

    REQUIRE(intersec(vec1, vec2));
}

TEST_CASE("dist:point-seg") {
    __m256d seg = _mm256_setr_pd(2., 4., 10., 4.);
    __m256d pt = _mm256_setr_pd(6., 6., 6., 6.);

    REQUIRE(dist(6., 6., 2., 4., 10., 4.) == Catch::Approx(2.));
    REQUIRE(dist(pt, seg) == Catch::Approx(2.));
}
