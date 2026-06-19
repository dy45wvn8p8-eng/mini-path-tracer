#include <iostream>
#include <fstream>
#include "ray.h"

// 광선이 하늘에 부딪혔을 때 색상을 결정하는 함수 (Linear Interpolation)
vec3 ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    // y값을 -1.0 ~ 1.0 범위에서 0.0 ~ 1.0 범위로 정규화
    double t = 0.5 * (unit_direction.y() + 1.0);
    // 흰색과 파란색을 y축 높이에 따라 부드럽게 섞음
    return vec3(1.0, 1.0, 1.0) * (1.0 - t) + vec3(0.5, 0.7, 1.0) * t;
}

int main() {
    // 1. 이미지 규격
    const double aspect_ratio = 1.0;
    const int image_width = 256;
    const int image_height = 256;

    // 2. 가상 카메라 및 스크린(뷰포트) 설정
    double viewport_height = 2.0;
    double viewport_width = viewport_height * aspect_ratio;
    double focal_length = 1.0; // 카메라와 스크린 사이의 거리

    vec3 camera_origin = vec3(0, 0, 0);
    vec3 horizontal = vec3(viewport_width, 0, 0);
    vec3 vertical = vec3(0, viewport_height, 0);
    // 스크린의 좌측 하단 구석 좌표 계산
    vec3 lower_left_corner = camera_origin - horizontal*0.5 - vertical*0.5 - vec3(0, 0, focal_length);

    // 3. 렌더링 시작
    std::ofstream image_file("image.ppm");
    image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            double u = double(i) / (image_width - 1);
            double v = double(j) / (image_height - 1);
            
            // 카메라에서 가상 스크린의 특정 픽셀을 향하는 광선 생성
            ray r(camera_origin, lower_left_corner + horizontal*u + vertical*v - camera_origin);
            
            // 광선이 감지한 색상 계산
            vec3 pixel_color = ray_color(r);
            
            int ir = static_cast<int>(255.999 * pixel_color.x());
            int ig = static_cast<int>(255.999 * pixel_color.y());
            int ib = static_cast<int>(255.999 * pixel_color.z());

            image_file << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    std::cout << "성공: 3D 가상 우주가 생성되었습니다.\n";
    return 0;
}

