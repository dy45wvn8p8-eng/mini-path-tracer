#include <iostream>
#include <fstream>
#include <cmath>
#include "rtweekend.h" // 난수/상수 패키지 장착
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
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0*a);
    }
}

// 픽셀의 색상을 결정하는 함수
vec3 ray_color(const ray& r) {
    auto t = hit_sphere(vec3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return vec3(N.x()+1, N.y()+1, N.z()+1) * 0.5;
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t_bg = 0.5 * (unit_direction.y() + 1.0);
    return vec3(1.0, 1.0, 1.0) * (1.0 - t_bg) + vec3(0.5, 0.7, 1.0) * t_bg;
}

int main() {
    // 이미지 사양 설정
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    
    // 픽셀당 발사할 광선 수 샘플링 설정 (100발 난사)
    const int samples_per_pixel = 100; 

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
    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // 픽셀 스캔 (좌상단부터 우하단으로 광선 발사)
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\r남은 스캔라인: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            
            // 100발의 광선이 가져온 색상을 누적할 바구니
            vec3 pixel_color(0, 0, 0);
            
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (double(i) + random_double()) / (image_width-1);
                auto v = (double(j) + random_double()) / (image_height-1);
                
                ray r(origin, lower_left_corner + horizontal*u + vertical*v - origin);
                
                // 에러가 났던 += 연산자를 안전한 일반 덧셈 형태로 풀어씀
                pixel_color = pixel_color + ray_color(r); 
            }
            
            // 누적된 색상을 총 광선 수(100)로 나누어 평균값을 구함
            auto scale = 1.0 / samples_per_pixel;
            
            // 이름 중복 에러를 막기 위해 r_color, g_color, b_color로 명확히 분리!
            double r_color = pixel_color.x() * scale;
            double g_color = pixel_color.y() * scale;
            double b_color = pixel_color.z() * scale;

            // 파일에 최종 부드러워진 RGB 데이터 쓰기
            out << static_cast<int>(255.999 * r_color) << ' '
                << static_cast<int>(255.999 * g_color) << ' '
                << static_cast<int>(255.999 * b_color) << '\n';
        }
    }
    std::cerr << "\n렌더링 완료.\n";
    return 0;
}