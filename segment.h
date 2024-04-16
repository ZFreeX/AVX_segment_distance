#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <immintrin.h>

using namespace std;

bool comp (long double a, long double b) { return a < b || abs(a - b) < (1e-9); }

bool intersec (long long x1, long long y1, long long x2, long long y2, long long x3, long long y3, long long x4, long long y4) {
    if (min(x1, x2) > min(x3, x4)) {
        swap(x1, x3); swap(x2, x4);
        swap(y1, y3); swap(y2, y4);
    }
    /// line
    long long a1 = y2 - y1, b1 = x1 - x2, c1 = y1 * x2 - y2 * x1;
    long long a2 = y4 - y3, b2 = x3 - x4, c2 = y3 * x4 - y4 * x3;
    if (a1 * b2 == a2 * b1 && a1 * c2 == a2 * c1 && b1 * c2 == b2 * c1) {
        /// [x1, x2] [x3, x4]
        vector <pair <int, int>> X;
        X.push_back({min(x1, x2), max(x1, x2)});
        X.push_back({min(x3, x4), max(x3, x4)});

        vector <pair <int, int>> Y;
        Y.push_back({min(y1, y2), max(y1, y2)});
        Y.push_back({min(y3, y4), max(y3, y4)});

        sort(X.begin(), X.end()); sort(Y.begin(), Y.end());
        if (X[0].second >= X[1].first && Y[0].second >= Y[1].first) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (a1 * b2 == a2 * b1 && a1 * c2 != a2 * c1 && b1 * c2 != b2 * c1) {
        return false;
    }
    else {
        long double x = (long double)(b1 * c2 - b2 * c1) / (long double)(a1 * b2 - a2 * b1);
        long double y = (long double)(a2 * c1 - a1 * c2) / (long double)(a1 * b2 - a2 * b1);
        if (comp(min(x1, x2), x) && comp(x, max(x1, x2)) && comp(min(x3, x4), x) && comp(x, max(x3, x4)) &&
            comp(min(y1, y2), y) && comp(y, max(y1, y2)) && comp(min(y3, y4), y) && comp(y, max(y3, y4))) {
            return true;
        }
        else {
            return false;
        }
    }
}

double dist_sqr (double x1, double y1, double x2, double y2){
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

double S (double a, double b, double c) {
    double p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

double dist (double x, double y, double sx, double sy, double fx, double fy) {
    double la = dist_sqr(x, y, sx, sy), lb = dist_sqr(x, y, fx, fy), lc = dist_sqr(sx, sy, fx, fy);
    if (lb > la + lc) {
        return sqrt(la);
    }
    else if (la > lb + lc) {
        return sqrt(lb);
    }
    else {
        return abs((fy - sy) * x + (sx - fx) * y + (sy * fx - sx * fy)) / sqrt(lc);
    }
}

double seg_distance(long long x1,long long y1, long long x2, long long y2, long long x3, long long y3, long long x4, long long y4) {
if (intersec(x1, y1, x2, y2, x3, y3, x4, y4)) return 0.;
    else {
        double ans = 1e10;
        ans = min(ans, dist(x1, y1, x3, y3, x4, y4));
        ans = min(ans, dist(x2, y2, x3, y3, x4, y4));
        ans = min(ans, dist(x3, y3, x1, y1, x2, y2));
        ans = min(ans, dist(x4, y4, x1, y1, x2, y2));
        return ans;
}


}

