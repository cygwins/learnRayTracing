#ifndef MOVING_SPHERE_H_
#define MOVING_SPHERE_H_
#include "hitable.h"
#include "material.h"
using std::to_string;
using std::cout;
using std::endl;

class moving_sphere : public hitable {
public:
    moving_sphere() {}
    moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *mat) :
        center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(mat) {}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    virtual void print() const;

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
            rec.whom = "moving shpere " + to_string(hid);
            return true;
        }
        t = (-b + sqrt(discriminant)) / a; // two possible solution
        if(t > t_min && t < t_max) {
            rec.t = t;
            rec.p = r.at(t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            rec.whom = "moving shpere " + to_string(hid);
            return true;
        }
    }
    return false;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb& box) const {
    vec3 min_center = min(center(t0), center(t1));
    vec3 max_center = max(center(t0), center(t1));
    float r = fabs(radius);
    box = aabb(min_center - r, max_center + r);
    return true;
}

void moving_sphere::print() const {
    cout << "Moving sphere " << to_string(hid) << ", " << center(time0) << " at " << time0 << ", "
         << center(time1) << " at " << time1 << ", radius " << radius << endl;
}

#endif