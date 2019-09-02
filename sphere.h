#ifndef SPHERE_H_
#define SPHERE_H_
#include "hitable.h"
#include "material.h"
using std::to_string;
using std::cout;
using std::endl;

class sphere : public hitable {
public:
    sphere() {}
    sphere(vec3 cen, float r, material *mat) : center(cen), radius(r), mat_ptr(mat) {}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    virtual void print() const;
    vec3 center;
    float radius;
    material *mat_ptr;
};

void get_sphere_uv(const vec3 &p, float &u, float &v) {
    float phi = atan2(p.z, p.x);
    float theta = asin(p.y);
    u = 1 - (phi + M_PI) / (2*M_PI); // cross(x, z) = -y, need to flip
    v = (theta + M_PI/2) / M_PI;
}

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
            get_sphere_uv(rec.normal, rec.u, rec.v);
            rec.mat_ptr = mat_ptr;
            rec.whom = "shpere " + to_string(hid) + " at " + to_string(center.x) + ","
                                 + to_string(center.y) + "," + to_string(center.z);
            return true;
        }
        t = (-b + sqrt(discriminant)) / a; // two possible solution
        if(t > t_min && t < t_max) {
            rec.t = t;
            rec.p = r.at(t);
            rec.normal = (rec.p - center) / radius;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            rec.mat_ptr = mat_ptr;
            rec.whom = "shpere " + to_string(hid) + " at " + to_string(center.x) + ","
                                 + to_string(center.y) + "," + to_string(center.z);
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(float t0, float t1, aabb& box) const {
    float r = fabs(radius);
    box = aabb(center - r, center + r);
    return true;
}

void sphere::print() const {
    cout << "Sphere " << to_string(hid) << " at " << center << ", radius " << radius << endl;
}

#endif
