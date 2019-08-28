#ifndef AABB_H_
#define AABB_H_
#include "ray.h"
using std::cerr;
using std::endl;

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb {
public:
    aabb() {}
    aabb(const vec3 &a, const vec3 &b) : min_corner(a), max_corner(b) {}

    bool hit(const ray& r, float tmin, float tmax) const;

    friend aabb surrounding_box(aabb box0, aabb box1);

    vec3 min_corner, max_corner;
};

bool aabb::hit(const ray& r, float tmin, float tmax) const {
    // x0 = Ax + t0*Bx <==> t0 = (x0 - Ax) / Bx
    // cout << "aabb (" << min_corner << ", " << max_corner << ")";
    for (int i = 0; i < 3; ++i) {
        float invB = 1.0 / r.direction()[i];
        float t0 = (min_corner[i] - r.origin()[i]) * invB;
        float t1 = (max_corner[i] - r.origin()[i]) * invB;
        if(invB < 0) { std::swap(t0, t1); }
        tmin = ffmax(tmin, t0);
        tmax = ffmin(tmax, t1);
        if(tmin >= tmax) {
            // cout << " no." << endl;
            return false;
        }
    }
    // cout << " hit!" << endl;
    return true;
}

aabb surrounding_box(aabb box0, aabb box1) {
    vec3 corner0 = min(box0.min_corner, box1.min_corner);
    vec3 corner1 = max(box0.max_corner, box1.max_corner);
    return aabb(corner0, corner1);
}

#endif