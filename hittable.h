#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

// 충돌 지점의 상세 정보를 담는 상자
struct hit_record {
    vec3 p;          // 부딪힌 지점의 3D 좌표
    vec3 normal;     // 부딪힌 지점의 법선 벡터
    double t;        // 광선 시작점으로부터의 거리(시간)
    bool front_face; // 광선이 물체 밖에서 부딪혔는지, 안에서 부딪혔는지 판별

    // 법선 벡터가 항상 광선의 반대 방향을 향하도록 정렬하는 함수
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = (r.direction().e[0]*outward_normal.e[0] + 
                      r.direction().e[1]*outward_normal.e[1] + 
                      r.direction().e[2]*outward_normal.e[2]) < 0;
        normal = front_face ? outward_normal : vec3(0,0,0) - outward_normal;
    }
};

// 무언가와 부딪힐 수 있는 오브젝트들의 기본 뼈대 (추상 클래스)
class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif