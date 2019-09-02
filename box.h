#ifndef BOX_H_
#define BOX_H_
#include "hitable_list.h"
#include "material.h"
#include "aarect.h"
using std::to_string;
using std::cout;
using std::endl;

class box : public hitable {
public:
    box() {}
    box(const vec3 &c0, const vec3 &c1, material *mat) : min_corner(c0), max_corner(c1), mat_ptr(mat) {
        facets = new hitable_list;
        facets->add(new xy_rect(c0.x, c1.x, c0.y, c1.y, c1.z, mat));
        facets->add(new flip_normals(new xy_rect(c0.x, c1.x, c0.y, c1.y, c0.z, mat)));
        facets->add(new xz_rect(c0.x, c1.x, c0.z, c1.z, c1.y, mat));
        facets->add(new flip_normals(new xz_rect(c0.x, c1.x, c0.z, c1.z, c0.y, mat)));
        facets->add(new yz_rect(c0.y, c1.y, c0.z, c1.z, c1.x, mat));
        facets->add(new flip_normals(new yz_rect(c0.y, c1.y, c0.z, c1.z, c0.x, mat)));
    }

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        return facets->hit(r, t_min, t_max, rec);
    }
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = aabb(min_corner, max_corner);
        return true;
    }
    virtual void print() const {
        cout << "Box " << to_string(hid) << ", ("<< min_corner << ") - (" << max_corner << ")" << endl;
    }

    vec3 min_corner, max_corner;
    material *mat_ptr;
    hitable_list *facets;
};

#endif