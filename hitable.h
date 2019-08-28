#ifndef HITABLE_H_
#define HITABLE_H_
#include "ray.h"
#include "aabb.h"
using std::string;

class material; // this declaration is crucial because of the circularity of references

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

#endif
