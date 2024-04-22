#include <immintrin.h>


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

//swap 2 packed segments
void swap(__m256d &a, __m256d &b) {
  auto mask1 = _mm256_setr_pd(1., 1., 1., 1.);
  auto mask2 = _mm256_setr_pd(0., 0., 0., 0.);
  auto a2 = _mm256_blendv_pd(a, b, mask1);
  auto b2 = _mm256_blendv_pd(a, b, mask2);
  a = a2;
  b = b2;
}

//1-2 = a segment = [x1, y1, x2, y2]
//3-4 = b segment = [x3, y3, x4, y4]
bool intersec(__m256d a, __m256d b) {
  if (min_x(a) > min_x(b)) {
    swap(a, b);
  }
  double ptr_a[4], ptr_b[4];
  _mm256_store_pd (ptr_a, a);
  _mm256_store_pd (ptr_b, b);

  //1
  std::swap(ptr_a[0], ptr_a[3]);
  std::swap(ptr_a[2], ptr_b[0]);

  //2
  std::swap(ptr_a[1], ptr_b[3]);
  std::swap(ptr_b[0], ptr_b[2]);

  //3
  std::swap(ptr_a[1], ptr_a[3]);
  std::swap(ptr_b[0], ptr_b[3]);
  std::swap(ptr_b[1], ptr_b[2]);

  __m256d new_a = _mm256_load_pd (ptr_a);
  __m256d new_b = _mm256_load_pd (ptr_b);

  //[a1,b1,a2,b2]
  auto ab_koef = _mm256_sub_pd (new_a, new_b);
  double* ptr_ab[4];
  _mm256_store_pd (ptr_ab, ab_koef);

  std::swap(ptr_ab[0], ptr_ab[3]);
  std::swap(ptr_ab[2], ptr_ab[1]);
  _m256d new_ab_koef = _mm256_load_pd (ptr_ab);
  
  
  //[x2,x1,x4,x3]
  //[y1,y2,y3,y4]
  std::swap(ptr_a[0], ptr_b[1]);
  std::swap(ptr_a[2], ptr_b[3]);

  new_a = _mm256_load_pd (ptr_a);
  new_b = _mm256_load_pd (ptr_b);

  new_a = _mm256_mul_pd (new_a, new_b);

  //[c1,*,c2,*]
  auto c_koef = _mm256_hsub_pd (new_a, new_b);

  double* ptr_c[4];
  _mm256_store_pd (ptr_c, c_koef);

  if ()
}