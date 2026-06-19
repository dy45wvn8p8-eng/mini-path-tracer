#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
    public:
        vec3 center;
        double radius;

        sphere() {}
        sphere(vec3 cen, double r) : center(cen), radius(r) {};

        // 광선이 이 구체와 부딪히는지 판별하는 함수 (기존 main의 함수를 이식)
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

inline bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().e[0]*r.direction().e[0] + r.direction().e[1]*r.direction().e[1] + r.direction().e[2]*r.direction().e[2];
    auto b = 2.0 * (oc.e[0]*r.direction().e[0] + oc.e[1]*r.direction().e[1] + oc.e[2]*r.direction().e[2]);
    auto c = (oc.e[0]*oc.e[0] + oc.e[1]*oc.e[1] + oc.e[2]*oc.e[2]) - radius*radius;

    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0) return false;

    // 가장 가까운 충돌 지점(t값)이 유효한 범위(t_min ~ t_max) 안에 있는지 확인
    auto sqrtd = sqrt(discriminant);
    auto root = (-b - sqrtd) / (2.0*a);
    if (root < t_min || t_max < root) {
        root = (-b + sqrtd) / (2.0*a);
        if (root < t_min || t_max < root)
            return false;
    }

    // 충돌 정보 기록 상자(hit_record)에 데이터 채우기
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) * (1.0 / radius); // 법선 벡터 계산
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif