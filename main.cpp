#include <iostream>
#include <fstream>
#include "ray.h"

// 두 벡터의 내적(Dot Product) 연산 함수
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2];
}

// 광선이 구체와 부딪히는지 판별하는 함수 (2차 방정식 판별식)
bool hit_sphere(const vec3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;               // (A - C)
    double a = dot(r.direction(), r.direction()); // B · B
    double b = 2.0 * dot(r.direction(), oc);      // 2 * B · (A - C)
    double c = dot(oc, oc) - radius * radius;     // (A - C) · (A - C) - R^2
    
    double discriminant = b*b - 4*a*c;           // 판별식 D = b^2 - 4ac
    return (discriminant > 0);                   // D > 0 이면 광선이 구체를 관통함!
}

// 픽셀의 색상을 결정하는 함수
vec3 ray_color(const ray& r) {
    // 화면 중앙 뒤편(0, 0, -1)에 반지름이 0.5인 구체 배치
    if (hit_sphere(vec3(0, 0, -1), 0.5, r))
        return vec3(1.0, 0.0, 0.0); // 구체에 부딪히면 정열의 빨간색!

    // 안 부딪히면 기존의 아름다운 그라데이션 하늘 배경
    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return vec3(1.0, 1.0, 1.0) * (1.0 - t) + vec3(0.5, 0.7, 1.0) * t;
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
    
    // 연산자 에러를 방지하기 위해 / 2 대신 * 0.5로 완전히 교체한 라인
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