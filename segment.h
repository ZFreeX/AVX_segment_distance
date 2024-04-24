#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

//using namespace std;

bool comp(double a, double b) {
    return a < b || std::abs(a - b) < (1e-9);
}

bool intersec(
    double x1, double y1, double x2, double y2, double x3, double y3,
    double x4, double y4) {
    if (std::min(x1, x2) > std::min(x3, x4)) {
        std::swap(x1, x3);
        std::swap(x2, x4);
        std::swap(y1, y3);
        std::swap(y2, y4);
    }
    /// line
    double a1 = y2 - y1, b1 = x1 - x2, c1 = y1 * x2 - y2 * x1;
    double a2 = y4 - y3, b2 = x3 - x4, c2 = y3 * x4 - y4 * x3;

    // making vector [y2, x1, y4, x3]
    // making vector [y1, x2, y3, x4]
    // sub = calculating a1, a2, b1, b2

    if (a1 * b2 == a2 * b1 && a1 * c2 == a2 * c1 && b1 * c2 == b2 * c1) {
        /// [x1, x2] [x3, x4]
        if (y1 > y2) {
            std::swap(y1, y2);
        }
        if (y3 > y4) {
            std::swap(y3, y4);
        }

        if (x1 > x2) {
            std::swap(x1, x2);
        }
        if (x3 > x4) {
            std::swap(x3, x4);
        }
        if (x1 > x3) {
            std::swap(x1, x3);
            std::swap(x2, x4);
        }
        if (y1 > y3) {
            std::swap(y1, y3);
            std::swap(y2, y4);
        }

        return (x1 <= x4 && x2 >= x3 && y1 <= y4 && y2 >= y3);
    } else if (a1 * b2 == a2 * b1 && a1 * c2 != a2 * c1 && b1 * c2 != b2 * c1) {
        return false;
    } else {
        double x = (double)(b1 * c2 - b2 * c1) / (double)(a1 * b2 - a2 * b1);
        double y = (double)(a2 * c1 - a1 * c2) / (double)(a1 * b2 - a2 * b1);
        return (comp(std::min(x1, x2), x) && comp(x, std::max(x1, x2)) && comp(std::min(x3, x4), x) &&
            comp(x, std::max(x3, x4)) && comp(std::min(y1, y2), y) && comp(y, std::max(y1, y2)) &&
            comp(std::min(y3, y4), y) && comp(y, std::max(y3, y4)));
    }
}

double dist_sqr(double x1, double y1, double x2, double y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

double dist(double x, double y, double sx, double sy, double fx, double fy) {
    double la = dist_sqr(x, y, sx, sy), lb = dist_sqr(x, y, fx, fy), lc = dist_sqr(sx, sy, fx, fy);
    if (lb > la + lc) {
        return std::sqrt(la);
    } else if (la > lb + lc) {
        return std::sqrt(lb);
    } else {
        return std::abs((fy - sy) * x + (sx - fx) * y + (sy * fx - sx * fy)) / std::sqrt(lc);
    }
}

double seg_distance(
    double x1, double y1, double x2, double y2, double x3, double y3,
    double x4, double y4) {
    if (intersec(x1, y1, x2, y2, x3, y3, x4, y4)) {
        return 0.;
    } else {
        double ans = 1e10;
        ans = std::min(ans, dist(x1, y1, x3, y3, x4, y4));
        ans = std::min(ans, dist(x2, y2, x3, y3, x4, y4));
        ans = std::min(ans, dist(x3, y3, x1, y1, x2, y2));
        ans = std::min(ans, dist(x4, y4, x1, y1, x2, y2));
        return ans;
    }
}
