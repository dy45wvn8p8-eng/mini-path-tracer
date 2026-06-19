#ifndef RAY_H
#define RAY_H

// 3차원 벡터 구조체 정의
struct vec3 {
    double e[3];
    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator+(const vec3& v) const { return vec3(e[0]+v.e[0], e[1]+v.e[1], e[2]+v.e[2]); }
    vec3 operator-(const vec3& v) const { return vec3(e[0]-v.e[0], e[1]-v.e[1], e[2]-v.e[2]); }
    vec3 operator*(double t) const { return vec3(e[0]*t, e[1]*t, e[2]*t); }
};

// 광선 클래스 정의
class ray {
public:
    ray() {}
    ray(const vec3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    vec3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    // P(t) = A + t*B 연산
    vec3 at(double t) const {
        return orig + dir * t;
    }

private:
    vec3 orig;
    vec3 dir;
};

// 벡터 정규화 (크기를 1로 만들기)
inline vec3 unit_vector(vec3 v) {
    double len = __builtin_sqrt(v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2]);
    return vec3(v.e[0]/len, v.e[1]/len, v.e[2]/len);
}

#endif