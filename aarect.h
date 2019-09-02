#ifndef AARECT_H_
#define AARECT_H_
#include "hitable.h"
#include "material.h"

class xy_rect : public hitable {
public:
    xy_rect() { }
    xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material *mat)
    : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(mat) { }
    
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = aabb(vec3(x0, y0, k-0.0001), vec3(x1, y1, k+0.0001));
        return true;
    }
    virtual void print() const;

    float x0, x1, y0, y1, k; // on z = k plane
    material *mat_ptr;
};

bool xy_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    float t = (k - r.origin().z) / r.direction().z;
    if(t < t_min || t > t_max) {
        return false;
    }
    float x = r.at(t).x;
    float y = r.at(t).y;
    if(x < x0 || x > x1 || y < y0 || y > y1) {
        return false;
    }
    rec.t = t;
    rec.p = r.at(t);
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.mat_ptr = mat_ptr;
    rec.normal = vec3(0,0,1);
    return true;
}

void xy_rect::print() const {
    cout << "xy-rect " << to_string(hid) << " where z = " << k << ", ("
    << x0 << ", " << y0 << ") - (" << x1 << ", " << y1 << ")" << endl;
}

class xz_rect : public hitable {
public:
    xz_rect() { }
    xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, material *mat)
    : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) { }
    
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = aabb(vec3(x0, k-0.0001, z0), vec3(x1, k+0.0001, z1));
        return true;
    }
    virtual void print() const;

    float x0, x1, z0, z1, k; // on y = k plane
    material *mat_ptr;
};

bool xz_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    float t = (k - r.origin().y) / r.direction().y;
    if(t < t_min || t > t_max) {
        return false;
    }
    float x = r.at(t).x;
    float z = r.at(t).z;
    if(x < x0 || x > x1 || z < z0 || z > z1) {
        return false;
    }
    rec.t = t;
    rec.p = r.at(t);
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.mat_ptr = mat_ptr;
    rec.normal = vec3(0,1,0);
    return true;
}

void xz_rect::print() const {
    cout << "xz-rect " << to_string(hid) << " where y = " << k << ", ("
    << x0 << ", " << z0 << ") - (" << x1 << ", " << z1 << ")" << endl;
}

class yz_rect : public hitable {
public:
    yz_rect() { }
    yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, material *mat)
    : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) { }
    
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = aabb(vec3(k-0.0001, y0, z0), vec3(k+0.0001, y1, z1));
        return true;
    }
    virtual void print() const;

    float y0, y1, z0, z1, k; // on x = k plane
    material *mat_ptr;
};

bool yz_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    float t = (k - r.origin().x) / r.direction().x;
    if(t < t_min || t > t_max) {
        return false;
    }
    float y = r.at(t).y;
    float z = r.at(t).z;
    if(y < y0 || y > y1 || z < z0 || z > z1) {
        return false;
    }
    rec.t = t;
    rec.p = r.at(t);
    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.mat_ptr = mat_ptr;
    rec.normal = vec3(1,0,0);
    return true;
}

void yz_rect::print() const {
    cout << "yz-rect " << to_string(hid) << " where x = " << k << ", ("
    << y0 << ", " << z0 << ") - (" << y1 << ", " << z1 << ")" << endl;
}

#endif