#ifndef HITABLE_H_
#define HITABLE_H_
#include "ray.h"
#include "aabb.h"
using std::string;
using std::cout;
using std::endl;

class material; // this declaration is crucial because of the circularity of references

struct hit_record {
    float t; // hit time
    vec3 p; // hit point
    float u, v; // hit uv for texture mapping
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

class flip_normals : public hitable {
public:
    flip_normals(hitable *p) : ptr(p) { }
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        if(ptr->hit(r, t_min, t_max, rec)) {
            rec.normal = -rec.normal;
            return true;
        } else {
            return false;
        }
    }
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        return ptr->bounding_box(t0, t1, box);
    }
    virtual void print() const {
        ptr->print();
        cout << "with normals flipped." << endl;
    }
    
    hitable *ptr;
};

#endif
