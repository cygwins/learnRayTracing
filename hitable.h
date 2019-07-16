#ifndef HITABLE_H_
#define HITABLE_H_
#include "ray.h"

class material; // this declaration is crucial because of the circularity of references
class aabb;

struct hit_record {
    float t; // hit time
    vec3 p; // hit point
    vec3 normal;
    material *mat_ptr;
};

class hitable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
};

class aabb {
public:
    aabb() {}
    aabb(const vec3 &a, const vec3 &b) : min_corner(a), max_corner(b) {}

    bool hit(const ray& r, float tmin, float tmax) const;

    vec3 min_corner, max_corner;
};

bool aabb::hit(const ray& r, float tmin, float tmax) const {
    // x0 = Ax + t0*Bx <==> t0 = (x0 - Ax) / Bx
    for (int i = 0; i < 3; ++i) {
        float invB = 1.0 / r.direction()[i];
        float t0 = (min_corner[i] - r.origin()[i]) * invB;
        float t1 = (max_corner[i] - r.origin()[i]) * invB;
        if(invB < 0) { std::swap(t0, t1); }
        tmin = fmax(tmin, t0); // t0 > tmin ? t0 : tmin;
        tmax = fmin(tmax, t1); // t1 < tmax ? t1 : tmax;
        if(tmin >= tmax) { return false; }
    }
    return true;
}

aabb surrounding_box(aabb box0, aabb box1) {
    vec3 corner0 = min(box0.min_corner, box1.min_corner);
    vec3 corner1 = min(box0.max_corner, box1.max_corner);
    return aabb(corner0, corner1);
}

#endif
