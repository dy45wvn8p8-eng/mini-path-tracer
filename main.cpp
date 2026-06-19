#include <iostream>
#include <fstream>
#include <cmath>
#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"
#include "ray.h"

// [개조] 이제 개별 구체가 아닌, hittable 우주 전체를 받아 색상을 결정함
vec3 ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    // 0.001부터 무한대(infinity) 사이에서 가장 먼저 부딪히는 물체를 탐색
    if (world.hit(r, 0.001, infinity, rec)) {
        return vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1) * 0.5;
    }

    // 배경 하늘색 그라데이션
    vec3 unit_direction = unit_vector(r.direction());
    auto t_bg = 0.5 * (unit_direction.y() + 1.0);
    return vec3(1.0, 1.0, 1.0) * (1.0 - t_bg) + vec3(0.5, 0.7, 1.0) * t_bg;
}

int main() {
    // 이미지 사양 설정
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100; 

    // [월드 창조] 가상 우주 바구니 생성
    hittable_list world;
    
    // 물체 1: 우리가 보던 중앙의 무지갯빛 구슬 생성
    world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
    
    // 물체 2: 중앙 구슬 밑에 깔릴 엄청나게 거대한 바닥 구체 생성! (y축을 -100.5로 내려서 지평선처럼 만듦)
    world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));

    // 카메라 세팅
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = vec3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal * 0.5 - vertical * 0.5 - vec3(0, 0, focal_length);

    std::ofstream out("image.ppm");
    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\r남은 스캔라인: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            vec3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (double(i) + random_double()) / (image_width-1);
                auto v = (double(j) + random_double()) / (image_height-1);
                ray r(origin, lower_left_corner + horizontal*u + vertical*v - origin);
                
                // 개조된 world 객체를 넘겨줌
                pixel_color = pixel_color + ray_color(r, world); 
            }
            
            auto scale = 1.0 / samples_per_pixel;
            double r_color = pixel_color.x() * scale;
            double g_color = pixel_color.y() * scale;
            double b_color = pixel_color.z() * scale;

            out << static_cast<int>(255.999 * r_color) << ' '
                << static_cast<int>(255.999 * g_color) << ' '
                << static_cast<int>(255.999 * b_color) << '\n';
        }
    }
    std::cerr << "\n렌더링 완료.\n";
    return 0;
}