#ifndef CAMERA_H_
#define CAMERA_H_
#include "ray.h"
#include "common.h"

class camera {
public:
    camera() {}

    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect,
        float aperture, float focus_dist, float t0, float t1) : 
        lens_radius(aperture/2), time_0(t0), time_1(t1) {

        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2); // canvas is 1 away
        float half_width = aspect * half_height;
        w = normalized(lookfrom - lookat);
        u = normalized(cross(vup, w));
        v = cross(w, u);
        origin = lookfrom;
        lower_left_corner = origin
            - u * focus_dist * half_width
            - v * focus_dist * half_height
            - w * focus_dist;
        horizontal = 2 * u * focus_dist * half_width;
        vertical   = 2 * v * focus_dist * half_height;
    }

    ray get_ray(float i, float j) {
        vec3 rd = lens_radius * random_in_disk();
        float time = time_0 + drand48()*(time_1 - time_0);
        vec3 offset = u * rd.x + v * rd.y; // ray can pass through any point in lens
        // all possible ray exactly hit target on the focus plane focus_dist from lens
        return ray(origin + offset, lower_left_corner + i*horizontal + j*vertical - origin - offset, time);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
    float time_0, time_1; // shutter open/close time
};

#endif
