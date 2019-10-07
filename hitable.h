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
    int depth = 0; // for bvh, generic objects have depth 0, bvh_nodes can have more depth
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

class translate : public hitable {
public:
    translate(hitable *p, const vec3 &displacement) : ptr(p), offset(displacement) { }
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        ray moved_r(r.origin() - offset, r.direction(), r.time());
        if(ptr->hit(moved_r, t_min, t_max, rec)) {
            rec.p += offset;
            return true;
        } else {
            return false;
        }
    }
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        if(ptr->bounding_box(t0, t1, box)) {
            box = aabb(box.min_corner + offset, box.max_corner + offset);
            return true;
        } else {
            return false;
        }
    }
    virtual void print() const {
        ptr->print();
        cout << "with offset " << offset << endl;
    }
    
    vec3 offset;
    hitable *ptr;
};

#endif
