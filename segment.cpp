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
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

using Catch::Matchers::RangeEquals;

TEST_CASE("Single_distance") {
     REQUIRE(seg_distance(1LL, 1LL, 2LL, 2LL, 2LL, 1LL, 3LL, 0LL) == Catch::Approx(0.70710678118654752));
     //REQUIRE(seg_distance(_mm256_setr_pd(1., 1., 2., 2.), _mm256_setr_pd(2., 1., 3., 0.)) == Catch::Approx(0.70710678118654752));
}

TEST_CASE("Dist_sqr_test") {
     __m256d vec1 = _mm256_setr_pd(4.0, 5.0, 0., 0.);
     __m256d vec2 = _mm256_setr_pd(9.0, 3.0, 0., 0.);

    REQUIRE(dist_sqr(vec1, vec2) == Catch::Approx(29));
}

TEST_CASE("Min-Max") {
     __m256d vec1 = _mm256_setr_pd(4.0, 5.0, 0., 0.);
     __m256d vec2 = _mm256_setr_pd(9.0, 3.0, 0., 0.);

    REQUIRE(min_x(vec1) == Catch::Approx(0.));
    REQUIRE(max_x(vec2) == Catch::Approx(9.0));
    REQUIRE(max_y(vec2) == Catch::Approx(3.0));
    REQUIRE(min_y(vec1) == Catch::Approx(0.0));
}

/*TEST_CASE("Intersec") {
     __m256d vec1 = _mm256_setr_pd(2., 4., 10., 4.);
     __m256d vec2 = _mm256_setr_pd(2., 2., 6., 6.);

    REQUIRE(intersec(vec1, vec2));
}*/

TEST_CASE("Check_functions") {
     __m256d vec1 = _mm256_setr_pd(1., 2., 3., 4.);
     double ptr[4];
     _mm256_store_pd (ptr, vec1);

    REQUIRE(ptr[0] == Catch::Approx(1.));
    REQUIRE(ptr[1] == Catch::Approx(2.));
    REQUIRE(ptr[2] == Catch::Approx(3.));
    REQUIRE(ptr[3] == Catch::Approx(4.));

    vec1 = _mm256_permute_pd(vec1, 0b0000'1010);

    _mm256_store_pd (ptr, vec1);

    REQUIRE(ptr[0] == Catch::Approx(1.));
    REQUIRE(ptr[1] == Catch::Approx(2.));
    REQUIRE(ptr[2] == Catch::Approx(3.));
    REQUIRE(ptr[3] == Catch::Approx(4.));
}

// TEST_CASE("dist:point-seg") {
//     __m256d seg = _mm256_setr_pd(2., 4., 10., 4.);
//     __m256d pt = _mm256_setr_pd(6., 6., 6., 6.);

//     REQUIRE(dist(pt, seg) == Catch::Approx(2.));
// }

TEST_CASE("hsub-check") {
    __m256d a1 = _mm256_setr_pd(3., 4., 10., 4.);
    __m256d a2 = _mm256_setr_pd(1., 2., 3., 4.);

    __m256d tmp = _mm256_hsub_pd(a1, a2);
    double ptr[4];
    _mm256_store_pd (ptr, tmp);
    REQUIRE(ptr[0] == Catch::Approx(-1.));
    REQUIRE(ptr[1] == Catch::Approx(-1.));
    REQUIRE(ptr[2] == Catch::Approx(6.));
    REQUIRE(ptr[3] == Catch::Approx(-1.));
}