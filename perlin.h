#ifndef TEXTURE_H_
#define TEXTURE_H_

class perlin {
public:
    static float
    float noise(const vec3 &p) const {
        float u = p.x - floor(p.x);
        float v = p.y - floor(p.y);
        float w = p.z - floor(p.z);
        int i = int(4*p.x) & 255;
        int j = int(4*p.y) & 255;
        int k = int(4*p.z) & 255;
        return ranfloat[perm_x[i] ^ perm_x[j] ^ perm_x[k]];
    }
};

#endif