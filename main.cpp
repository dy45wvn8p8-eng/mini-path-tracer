#include <iostream>
#include <fstream>
#include <cmath>
#include "ray.h"

// 두 벡터의 내적(Dot Product) 연산 함수
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2];
}

// 광선이 구체와 부딪히는지 판별하고, 부딪힌 지점의 t값을 반환하는 함수
double hit_sphere(const vec3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        return -1.0; // 안 부딪히면 -1 반환
    } else {
        // 근의 공식 중 더 가까운(앞에 있는) 교점의 t값 계산
        return (-b - sqrt(discriminant)) / (2.0*a);
    }
}

// 픽셀의 색상을 결정하는 함수
vec3 ray_color(const ray& r) {
    // 1. 구체 충돌 검사 (반환된 t값 확인)
    auto t = hit_sphere(vec3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        // 충돌 지점 P 계산: P(t) = A + tB
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        
        // 벡터의 범위 (-1 ~ 1)를 RGB 색상 범위 (0 ~ 1)로 변환하는 마법의 맵핑
        return vec3(N.x()+1, N.y()+1, N.z()+1) * 0.5;
    }

    // 2. 안 부딪히면 기존의 그라데이션 하늘 배경
    vec3 unit_direction = unit_vector(r.direction());
    auto t_bg = 0.5 * (unit_direction.y() + 1.0);
    return vec3(1.0, 1.0, 1.0) * (1.0 - t_bg) + vec3(0.5, 0.7, 1.0) * t_bg;
}

int main() {
    // 이미지 스펙 설정
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // 카메라(상상의 눈) 세팅
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = vec3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal * 0.5 - vertical * 0.5 - vec3(0, 0, focal_length);

    // PPM 파일 출력 스트림 열기
    std::ofstream out("image.ppm");
    if (!out) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return 1;
    }

    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // 픽셀 스캔 (좌상단부터 우하단으로 광선 발사)
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\r남은 스캔라인: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + horizontal*u + vertical*v - origin);
            vec3 pixel_color = ray_color(r);
            
            // 파일에 RGB 데이터 쓰기
            out << static_cast<int>(255.999 * pixel_color.x()) << ' '
                << static_cast<int>(255.999 * pixel_color.y()) << ' '
                << static_cast<int>(255.999 * pixel_color.z()) << '\n';
        }
    }
    std::cerr << "\n렌더링 완료.\n";
    return 0;
}