#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// 상수의 전역 정의
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// 유틸리티 함수: 도(Degrees)를 라디안(Radians)으로 변환
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// 유틸리티 함수: 0.0 이상 1.0 미만의 무작위 실수(double) 반환
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

// 유틸리티 함수: [min, max) 범위의 무작위 실수 반환
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

#endif