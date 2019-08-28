#ifndef HITABLE_LIST_H_
#define HITABLE_LIST_H_
#include <vector>
#include <algorithm>
#include "hitable.h"
#include "aabb.h"
using std::vector;
using std::cerr;
using std::cout;
using std::endl;
using std::to_string;
using std::sort;

class hitable_list : public hitable {
public:
    hitable_list() {}
    // hitable_list(int n) { list.resize(n); }
    // hitable_list(vector<hitable> *l) : list(l) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    virtual void print() const;
    inline void add(hitable *const h) { list.push_back(h); }
    vector<hitable*> list;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest = t_max;
    for(auto &h : list) {
        if(h->hit(r, t_min, t_max, temp_rec)) {
            hit_anything = true;
            if(temp_rec.t < closest) {
                closest = temp_rec.t;
                rec = temp_rec;
            }
        }
    }
    return hit_anything;
}

bool hitable_list::bounding_box(float t0, float t1, aabb& box) const {
    if(list.size() < 1) { return false; }
    aabb temp_box;
    bool first_true = list[0]->bounding_box(t0, t1, temp_box);
    if(!first_true) { return false; }
    box = temp_box;
    for (int i = 1; i < list.size(); ++i) {
        if(list[i]->bounding_box(t0, t1, temp_box)) {
            box = surrounding_box(box, temp_box);
        } else {
            return false;
        }
    }
    return true;
}

void hitable_list::print() const {
    cout << "hitable_list " << to_string(hid) << " of size " << list.size() << endl;
}

#endif
