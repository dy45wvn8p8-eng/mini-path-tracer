#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects; // 물체들을 담는 동적 배열

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        // 우주의 모든 물체와 광선을 대조해 가장 가까운 충돌점 찾기
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

inline bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max; // 지금까지 가장 가까운 충돌 거리 정보

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t; // 더 가까운 물체가 있으면 기준면을 갱신!
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif