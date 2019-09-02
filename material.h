#ifndef MATERIAL_H_
#define MATERIAL_H_
#include "hitable.h"
#include "texture.h"
#include "common.h"

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
    virtual vec3 emitted(float u, float v, vec3 &p) const { return vec3(0,0,0); }
};

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n) * n;
}

bool refract(const vec3& vi, const vec3& n, float ni_over_nt, vec3& refracted) { // TODO: new material with fuzz + refract
    vec3 vin = normalized(vi);
    float cosi = dot(vin, n);
    float cost2 = 1.0 - ni_over_nt*ni_over_nt*(1 - cosi*cosi); // ni_over_nt == sint_over_sini
    if(cost2 > 0) {
        refracted = ni_over_nt*(vin - n*cosi) - n*sqrt(cost2); // sint*tau + cost*n
        return true;
    } else { // Total internal reflection
        return false;
    }
}

float schlick(float cosi, float refract_idx, float ni = 1.0) { // Christophe Schlick's approximation
    float r0 = (ni - refract_idx) / (ni + refract_idx);
    r0 *= r0;
    return r0 + (1 - r0) * pow((1 - cosi), 5);
}

class lambertian : public material {
public:
    lambertian(texture *a) : albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 target = rec.p + rec.normal + random_in_sphere();
        scattered = ray(rec.p, target - rec.p, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

    texture *albedo;
};

class metal : public material {
public:
    metal(const vec3& a, float f = 0) : albedo(a) {
        if(f < 1) {
            fuzz = f;
        } else {
            fuzz = 1;
        }
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(normalized(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_sphere(), r_in.time());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0; // only reflect outter face
    }

    vec3 albedo;
    float fuzz;
};

class dielectric : public material {
public:
    dielectric(float idx = 1.0) : refract_idx(idx) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        attenuation = vec3(1.0, 1.0, 1.0); // pure glass no color
        float ni_over_nt;
        float ni = 1.0; // TODO: What if outside is not air(1.0)?
        vec3 outward_n;
        vec3 reflected = reflect(normalized(r_in.direction()), rec.normal);
        vec3 refracted;
        float reflect_prob;
        float cosi;
        if(dot(r_in.direction(), rec.normal) < 0) { // into this
            outward_n = rec.normal;
            ni_over_nt = ni / refract_idx;
            cosi = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        } else { // out from this
            outward_n = -rec.normal;
            ni_over_nt = refract_idx / ni;
            cosi = refract_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length(); // potential bug: ni != 1.0
        }
        if(refract(r_in.direction(), outward_n, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosi, refract_idx, ni);
        } else {
            reflect_prob = 1.0; // Total internal reflection
        }
        if(drand48() < reflect_prob) {
            scattered = ray(rec.p, reflected, r_in.time());
        } else {
            scattered = ray(rec.p, refracted, r_in.time());
        }
        return true;
    }

    float refract_idx;
};

class diffuse_light : public material {
public:
    diffuse_light(texture *a) : emit(a) { }
    
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const { return false; }
    virtual vec3 emitted(float u, float v, vec3 &p) const { return emit->value(u, v, p); }

    texture *emit;
};

#endif