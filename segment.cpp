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

using Catch::Matchers::RangeEquals;

TEST_CASE("Single_distance") {
     REQUIRE(seg_distance(1LL, 1LL, 2LL, 2LL, 2LL, 1LL, 3LL, 0LL) == Catch::Approx(0.70710678118654752));
     REQUIRE(seg_distance(_mm256_setr_pd(1., 1., 2., 2.), _mm256_setr_pd(2., 1., 3., 0.)) == Catch::Approx(0.70710678118654752));
}


void CheckEquals(vector<int>& l, const auto& expected) {
    REQUIRE(l.size() == expected.size());
    CHECK_THAT(l, RangeEquals(expected));
}
	


/*
TEST_CASE("Intersec part test") {
     //__m256d vec1 = _mm256_setr_pd(4.0, 5.0, 0., 0.);
     //__m256d vec2 = _mm256_setr_pd(9.0, 3.0, 0., 0.);
    //swap(vec1, vec2);
    auto a = debugcpp(2LL, 2LL, 4LL, 4LL, 5LL, 5LL, 6LL, 6LL);
    auto b = debugasm(_mm256_setr_pd(2., 2., 4., 4.), _mm256_setr_pd(5., 5., 6., 6.));
    CheckEquals(a, b);
} */

TEST_CASE("Intersec group test") {
	bool r1 = intersec(_mm256_setr_pd(2., 2., 4., 4.), _mm256_setr_pd(5., 5., 6., 6.));
	bool r2 = intersec(2, 2, 4, 4, 5, 5, 6, 6);
	CHECK(r1 == r2);
	r1 = intersec(_mm256_setr_pd(2., 2., 4., 2.), _mm256_setr_pd(4., 2., 2., 2.));
	r2 = intersec(2, 2, 4, 2, 4, 2, 2, 2);
	CHECK(r1 == r2);
}


TEST_CASE("Random dist test group") {
	const int lim = 100;
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	for (int i = 0; i < 100; i++) {
		vector<int> x(4);
		vector<int> y(4);
		//point - x1, y1
		//seg - [x2, y2, x3, y3]
		for (auto&e :x) {
			e = rng() % lim - 5;
		}
		for (auto&e :y) {
			e = rng() % lim - 5;
		}
		
		REQUIRE(dist(_mm256_setr_pd(x[0], y[0], x[0], y[0]), _mm256_setr_pd(x[1], y[1], x[2], y[2])) == Catch::Approx(dist(x[0], y[0], x[1], y[1], x[2], y[2])));
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

    vec1 = _mm256_permute_pd(vec1, 0b0000'1011);

    _mm256_store_pd (ptr, vec1);

    std::cout << ptr[0] << ' ' << ptr[1] << ' ' << ptr[2] << ' ' << ptr[3];

//     REQUIRE(ptr[0] == Catch::Approx(1.));
//     REQUIRE(ptr[1] == Catch::Approx(2.));
//     REQUIRE(ptr[2] == Catch::Approx(3.));
//     REQUIRE(ptr[3] == Catch::Approx(4.));
}

// TEST_CASE("dist:point-seg") {
//     __m256d seg = _mm256_setr_pd(2., 4., 10., 4.);
//     __m256d pt = _mm256_setr_pd(6., 6., 6., 6.);

//     REQUIRE(dist(6., 6., 2., 4., 10., 4.) == Catch::Approx(2.));
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
