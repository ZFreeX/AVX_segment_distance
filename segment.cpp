#include "segment.h"
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
     
}

TEST_CASE("Dist_sqr_test") {
     __m256d vec1 = _mm256_setr_pd(4.0, 5.0, 0., 0.);
    __m256d vec2 = _mm256_setr_pd(9.0, 3.0, 0., 0.);

    REQUIRE(dist_sqr(vec1, vec2) == Catch::Approx(29));
}
