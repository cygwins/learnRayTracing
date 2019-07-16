#ifndef SPHERE_H_
#define SPHERE_H_
#include "hitable.h"

class sphere : public hitable {
public:
    sphere() {}
    sphere(vec3 cen, float r, material *mat) : center(cen), radius(r), mat_ptr(mat) {}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    vec3 center;
    float radius;
    material *mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    // point P is on sphere(C, R) <==>
    // dot((P-C),(P-C)) == R*R <==>
    // dot((A+tB-C),(A+tB-C)) == R*R <==>
    // dot(B,B)*t*t + 2*dot(B,A-C)*t + dot(A-C,A-C)-R*R == 0
    vec3 ac = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(r.direction(), ac);
    float c = dot(ac, ac) - radius * radius;
    float discriminant = b * b - a * c;
    if(discriminant > 0) {
        float t = (-b - sqrt(discriminant)) / a;
        if(t > t_min && t < t_max) {
            rec.t = t;
            rec.p = r.at(t);
            // Note: negative radius leads to inward normal,
            // where normalized(rec.p - center) is always outward. (same with the other point)
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        t = (-b + sqrt(discriminant)) / a; // two possible solution
        if(t > t_min && t < t_max) {
            rec.t = t;
            rec.p = r.at(t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(float t0, float t1, aabb& box) const {
    float r = abs(radius);
    box = aabb(center - r, center + r);
    return true;
}


class moving_sphere : public hitable {
public:
    moving_sphere() {}
    moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *mat) :
        center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(mat) {}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    vec3 center(float time) const;

    vec3 center0, center1;
    float time0, time1;
    float radius;
    material *mat_ptr;
};

vec3 moving_sphere::center(float time) const {
    return (time1 - time)/(time1 - time0)*center0 + (time - time0)/(time1 - time0)*center1;
}

bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 ac = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float b = dot(r.direction(), ac);
    float c = dot(ac, ac) - radius * radius;
    float discriminant = b * b - a * c;
    if(discriminant > 0) {
        float t = (-b - sqrt(discriminant)) / a;
        if(t > t_min && t < t_max) {
            rec.t = t;
            rec.p = r.at(t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        t = (-b + sqrt(discriminant)) / a; // two possible solution
        if(t > t_min && t < t_max) {
            rec.t = t;
            rec.p = r.at(t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb& box) const {
    return true;
}

#endif
