//format: vectors [x1, y1, *, *], [x2, y2, *, *]
//returns: vector where the first element is square of distance
double dist_sqr (__m256d a, __m256d b) {
    __m256d s = _mm256_sub_pd(a, b);
    __m256d m = s;
    a = _mm256_mul_pd(m, s);
    b = a; 
    __m256d rez = _mm256_hadd_pd(a, b);

    double ptr[4]
    _mm256_store_pd (ptr, rez);
    return ptr[0];
}

//get: segment
//return: minimum x of its ends
double min_x(__m256d a) {
    double ptr[4]
    _mm256_store_pd (ptr, a);
    return std::min(ptr[0], ptr[2]);
}

//swap 2 packed segments
void swap(__m256d &a, __m256d &b) {
  auto a2 = _mm256_blendv_pd(a, b, 0xFF);
  auto b2 = _mm256_blendv_pd(a, b, 0x00);
  a = a2;
  b = b2;
}

//1-2 = a segment = [x1, y1, x2, y2]
//3-4 = b segment = [x3, y3, x4, y4]
bool intersec(__m256d a, __m256d b) {
    if (__mm256_cmp_pd (a, b, const int imm8))
}