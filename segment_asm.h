#include <immintrin.h>
#include <utility>
#include <array>
#include <cmath>
#include <iostream>

bool comp (double a, double b) {
  return a < b || abs(a - b) < (1e-9);
}

//format: vectors [x1, y1, *, *], [x2, y2, *, *]
//returns: vector where the first element is square of distance
double dist_sqr (__m256d a, __m256d b) {
    __m256d s = _mm256_sub_pd(a, b);
    __m256d m = s;
    a = _mm256_mul_pd(m, s);
    b = a; 
    __m256d rez = _mm256_hadd_pd(a, b);
    return *(double*)&rez;
}

//get: segment
//return: minimum x of its ends
double min_x(__m256d a) {
    double ptr[4];
    _mm256_store_pd (ptr, a);
    return std::min(ptr[0], ptr[2]);
}

double min_y(__m256d a) {
    double ptr[4];
    _mm256_store_pd (ptr, a);
    return std::min(ptr[1], ptr[3]);
}

double max_y(__m256d a) {
    double ptr[4];
    _mm256_store_pd (ptr, a);
    return std::max(ptr[1], ptr[3]);
}

double max_x(__m256d a) {
    double ptr[4];
    _mm256_store_pd (ptr, a);
    return std::max(ptr[0], ptr[2]);
}

//swap 2 packed segments
// void swap(__m256d &a, __m256d &b) {
//   __m256d c = a;
//   a = b;
//   b = c;
// }

//1-2 = a segment = [x1, y1, x2, y2]
//3-4 = b segment = [x3, y3, x4, y4]
bool intersec(__m256d a, __m256d b) {
  __m256d save1, save2, save3;

  if (min_x(a) > min_x(b)) {
    std::swap(a, b);
  }
  double ptr_a[4], ptr_b[4];
  _mm256_store_pd (ptr_a, a);
  _mm256_store_pd (ptr_b, b);

  //1
  std::swap(ptr_a[0], ptr_a[3]);
  std::swap(ptr_a[2], ptr_b[0]);
  //1-2 = a segment = [y2, y1, x3, x1]
  //3-4 = b segment = [x2, y3, x4, y4]

  //2
  std::swap(ptr_a[1], ptr_b[3]);
  std::swap(ptr_b[0], ptr_b[2]);
  //1-2 = a segment = [y2, y4, x3, x1]
  //3-4 = b segment = [x4, y3, x2, y1]

  //3
  std::swap(ptr_a[1], ptr_a[3]);
  std::swap(ptr_b[0], ptr_b[3]);
  std::swap(ptr_b[1], ptr_b[2]);
  std::swap(ptr_b[2], ptr_b[3]);
  //1-2 = a segment = [y2, x1, x3, y4]
  //3-4 = b segment = [y1, x2, x4, y3]  

  __m256d new_a = _mm256_load_pd (ptr_a);
  __m256d new_b = _mm256_load_pd (ptr_b);

  //[a1,b1,b2,a2]
  auto ab_koef = _mm256_sub_pd (new_a, new_b);
  double ptr_ab[4];
  _mm256_store_pd (ptr_ab, ab_koef);
  

  std::swap(ptr_ab[0], ptr_ab[2]);
  std::swap(ptr_ab[1], ptr_ab[3]);
  //std::swap(ptr_ab[2], ptr_ab[3]);
  auto new_ab_koef = _mm256_load_pd (ptr_ab);
  //[b2,a2,a1,b1]
  
  
  
  std::swap(ptr_a[0], ptr_b[1]);
  std::swap(ptr_a[3], ptr_b[2]);
  new_a = _mm256_load_pd (ptr_a);
  new_b = _mm256_load_pd (ptr_b);
  new_a = _mm256_permute_pd(new_a, 0b0000'1'0'0'1);
  new_b = _mm256_permute_pd(new_b, 0b0000'1'0'0'1);
  //[x2,x1,x4,x3]
  //[y1,y2,y3,y4]

  new_a = _mm256_mul_pd (new_a, new_b);
  //[x2*y1,x1*y2,x4*y3,x3*y4]

  auto c_koef = _mm256_hsub_pd (new_b, new_a);
  //[c1,*,c2,*]
  _mm256_store_pd (ptr_ab, c_koef);
  std::swap(ptr_ab[0], ptr_ab[2]);
  //[c2,*,c1,*]
  c_koef = _mm256_load_pd (ptr_ab);
  c_koef = _mm256_permute_pd (c_koef, 0b0000'1111);
  //[c2,c2,c1,c1]
  
  auto ab_mul = _mm256_mul_pd (new_ab_koef, ab_koef);
  //[a1*b2,b1*a2,b2*a1,a2*b1]
  ab_mul = _mm256_permute_pd(ab_mul, 0b0000'1'0'0'1);
  //[a1*b2,b1*a2,a2*b1,b2*a1]
  save3 = ab_mul;
  _mm256_store_pd (ptr_ab, ab_mul);
  //[a1,b1,b2,a2]-ab_koef
  auto abc_mul = _mm256_mul_pd (c_koef, ab_koef);
  //[a1*c2,b1*c2,b2*c1,a2*c1]
  abc_mul = _mm256_permute_pd(abc_mul, 0b0000'1'0'0'1);
  //[a1*c2,b1*c2,a2*c1,b2*c1]
  save1 = abc_mul;
  double ptr[4];
  _mm256_store_pd (ptr, abc_mul);
  std::swap(ptr[1], ptr[3]);
  abc_mul = _mm256_load_pd (ptr);
  save2 = abc_mul;
  //[a2*c1,b2*c1,a1*c2,b1*c2]
  auto for_cmp_c = _mm256_permute_pd (abc_mul, 0b0000'0'1'0'1);
  _mm256_store_pd (ptr, _mm256_cmp_pd (for_cmp_c, abc_mul, 0));
  //[2nd_cmp,3rd_cmp,2nd_cmp,3rd_cmp]
  if (ptr_ab[0] == ptr_ab[2] && ptr[0] && ptr[1]) {
        _mm256_store_pd (ptr_a, a);
        _mm256_store_pd (ptr_b, b);
        std::array <std::pair <int, int>, 2> X;
        X[0] = {std::min(ptr_a[0], ptr_a[2]), std::max(ptr_a[0], ptr_a[2])};
        X[1] = {std::min(ptr_b[0], ptr_b[2]), std::max(ptr_b[0], ptr_b[2])};

        std::array <std::pair <int, int>, 2> Y;
        Y[0] = {std::min(ptr_a[1], ptr_a[3]), std::max(ptr_a[1], ptr_a[3])};
        Y[1] = {std::min(ptr_b[1], ptr_b[3]), std::max(ptr_b[1], ptr_b[3])};

        std::sort(X.begin(), X.end()); std::sort(Y.begin(), Y.end());
        if (X[0].second >= X[1].first && Y[0].second >= Y[1].first) {
            return true;
        }
        else {
            return false;
        }
  } else if (ptr_ab[0] == ptr_ab[2] && !ptr[0] && !ptr[1]) {
    return false;
  } else {
    double ptr[4];
    //save1-[a1*c2,b1*c2,a2*c1,b2*c1]
    //save2-[a2*c1,b2*c1,a1*c2,b1*c2]
    //save3-[a1*b2,b1*a2,a2*b1,b2*a1]
    save3 = _mm256_permute_pd (save3, 0b0000'0'1'1'0);
    //save3-[b1*a2,a1*b2,a2*b1,b2*a1]
    save3 = _mm256_hsub_pd (save3, save1);
    //save3-[*,a1*b2-b1*a2,*,a1*b2-a2*b1]

    auto rez = _mm256_sub_pd (save1, save2);
    //rez-[*,b1*c2-b2*c1,a2*c1-a1*c2,*]
    rez = _mm256_div_pd(rez, save3);
    //rez-[*,x,y,*]
    _mm256_store_pd (ptr, rez);
    double x = ptr[1], y = ptr[2];
    return (comp(min_x(a), x) && comp(x, max_x(a)) && comp(min_x(b), x) && comp(x, max_x(b)) &&
            comp(min_y(a), y) && comp(y, max_y(a)) && comp(min_y(b), y) && comp(y, max_y(b)));
  }
}

//point-[x,y,x,y]
//seg-[sx,sy,fx,fy]
double dist (__m256d point, __m256d seg) {
    double ptr[4];
    _mm256_store_pd (ptr, seg);
    std::swap(ptr[0], ptr[2]);
    std::swap(ptr[1], ptr[3]);
    auto rev_seg = _mm256_load_pd (ptr);
    //[fx,fy,sx,sy]
    double la = dist_sqr(point, seg), lb = dist_sqr(point, rev_seg), lc = dist_sqr(seg, rev_seg);
    if (lb > la + lc) {
        return std::sqrt(la);
    }
    else if (la > lb + lc) {
        return std::sqrt(lb);
    }
    else {
      auto subs = _mm256_sub_pd(rev_seg, seg);
      //[fx-sx, fy-sy, sx-fx, sy-fy]
      auto changed_point = _mm256_permute_pd(point, 0b0000'0'1'0'1);
      //[y,x,y,x]
      subs = _mm256_mul_pd(subs, changed_point);
      //[y(fx-sx), x(fy-sy), y(sx-fx), x(sy-fy)]

      auto rez = _mm256_permute_pd(seg, 0b0000'1'0'0'1);
      //[sx,sy,fy,fx]
      rez = _mm256_mul_pd (rez, rev_seg);
      //[sx*fx,sy*fy,fy*sx,fx*sy]
      rez = _mm256_hsub_pd (rez, rez);
      //[sy*fy-sx*fx, sy*fy-sx*fx, fx*sy-fy*sx, fx*sy-fy*sx]
      rez = _mm256_add_pd(rez, subs);
      //[sy*fy-sx*fx + y(fx-sx), sy*fy-sx*fx + x(fy-sy), fx*sy-fy*sx + y(sx-fx), fx*sy-fy*sx + x(sy-fy)]
      double ptr[4];
      _mm256_store_pd (ptr, rez);
      double ptr2[4];
      _mm256_store_pd (ptr2, subs);
      return std::abs((ptr[2] + ptr2[3])) / std::sqrt(lc);
    }
}

//1-2 = a segment = [x1, y1, x2, y2]
//3-4 = b segment = [x3, y3, x4, y4]
double seg_distance(__m256d a, __m256d b) {
  if (intersec(a, b)) {
    return 0.;
  } else {
          double ans = 1e10;
          double ptr_a[4], ptr_b[4];
          _mm256_store_pd (ptr_a, a);
          _mm256_store_pd (ptr_b, b);
          ans = std::min(ans, dist(_mm256_set_pd (ptr_a[0], ptr_a[1], ptr_a[0], ptr_a[1]), b));
          ans = std::min(ans, dist(_mm256_set_pd (ptr_a[2], ptr_a[3], ptr_a[2], ptr_a[3]), b));
          ans = std::min(ans, dist(_mm256_set_pd (ptr_b[0], ptr_b[1], ptr_b[0], ptr_b[1]), a));
          ans = std::min(ans, dist(_mm256_set_pd (ptr_b[2], ptr_b[3], ptr_b[2], ptr_b[3]), a));
          return ans;
  }
}