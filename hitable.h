#ifndef HITABLE_H_
#define HITABLE_H_
#include "ray.h"
using std::cerr;
using std::endl;
using std::string;


class material; // this declaration is crucial because of the circularity of references
class aabb;

struct hit_record {
    float t; // hit time
    vec3 p; // hit point
    vec3 normal;
    material *mat_ptr;
    string whom; // hit whom
};

class hitable {
public:
    static unsigned int HID;

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
    virtual void print() const = 0;
    hitable() : hid(++HID) {}
    hitable(const hitable &h) : hid(++HID) {}
    hitable &operator=(const hitable &h) { hid = ++HID; return *this; }
    unsigned int hid;
};

unsigned int hitable::HID = 0;

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
        tmin = t0 > tmin ? t0 : tmin; // fmax(tmin, t0);
        tmax = t1 < tmax ? t1 : tmax; // fmin(tmax, t1);
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

bool box_compare_x(const aabb &a, const aabb &b) {
    return a.min_corner.x < b.min_corner.x;
}

bool box_compare_y(const aabb &a, const aabb &b) {
    return a.min_corner.y < b.min_corner.y;
}

bool box_compare_z(const aabb &a, const aabb &b) {
    return a.min_corner.z < b.min_corner.z;
}

bool box_compare_i(int axis, const aabb &a, const aabb &b) {
    return a.min_corner[axis] < b.min_corner[axis];
}

bool hitable_compare_i(int axis, const hitable *a, const hitable *b) {
    aabb aa, bb;
    if(!a->bounding_box(0,0,aa) || !b->bounding_box(0,0,bb)) {
        cerr << "no bounding_box comparing hitable" << endl;
    }
    return box_compare_i(axis, aa, bb);
}
#endif
