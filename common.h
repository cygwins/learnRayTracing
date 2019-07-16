#ifndef COMMON_H_
#define COMMON_H_
#include "vec3.h"

vec3 random_in_sphere() {
    vec3 p;
    do {
        p = 2*vec3(drand48(), drand48(), drand48()) - 1;
    } while (p.length() >= 1.0);
    return p;
}

vec3 random_in_disk() {
    vec3 p;
    do {
        p = 2*vec3(drand48(), drand48(), 0) - vec3(1,1,0);
    } while (p.length() >= 1.0);
    return p;
}

#endif
