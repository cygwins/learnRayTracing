#ifndef HITABLE_LIST_H_
#define HITABLE_LIST_H_
#include <vector>
#include "hitable.h"
using std::vector;
using std::cerr;
using std::cout;
using std::endl;
using std::to_string;

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

class bvh_node : public hitable { // bvh = bounding volumn hierarchy
public:
    bvh_node() {}
    bvh_node(vector<hitable*> l, float t0, float t1);
    bvh_node(hitable_list *l, float t0, float t1) : bvh_node(l->list, t0, t1) {}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    virtual void print() const;

    hitable *left;
    hitable *right;
    aabb box;
};

bvh_node::bvh_node(vector<hitable*> list, float t0, float t1) {
    int axis = int(3*drand48());
    sort(list.begin(), list.end(), [axis](hitable *a, hitable *b){
        return hitable_compare_i(axis, a, b);});
    if(list.size() <= 2) {
        left = list.size() > 0 ? list[0] : nullptr;
        right = list.size() > 1 ? list[1] : nullptr;
    } else {
        vector<hitable*> l = vector<hitable*>(list.begin(), list.begin() + list.size() / 2);
        vector<hitable*> r = vector<hitable*>(list.begin() + list.size() / 2, list.end());
        left = new bvh_node(l, t0, t1);
        right = new bvh_node(r, t0, t1);
    }
    aabb box_left, box_right;
    if((left && !left->bounding_box(t0, t1, box_left)) ||
       (right && !right->bounding_box(t0, t1, box_right))) {
        cerr << "no bounding_box constructing bvh_node" << endl;
    }
    box = right ? surrounding_box(box_left, box_right) : box_left;
}

bool bvh_node::bounding_box(float t0, float t1, aabb& b) const {
    b = box;
    return true;
}

bool bvh_node::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    if(box.hit(r, t_min, t_max)) {
        hit_record left_rec, right_rec;
        bool hit_left = left ? left->hit(r, t_min, t_max, left_rec) : false;
        bool hit_right = right ? right->hit(r, t_min, t_max, right_rec) : false;
        if(hit_left && hit_right) {
            if(left_rec.t < right_rec.t) {
                rec = left_rec;
            } else {
                rec = right_rec;
            }
            return true;
        } else if(hit_left) {
            rec = left_rec;
            return true;
        } else if(hit_right) {
            rec = right_rec;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void bvh_node::print() const {
    cout << "bvh_node " << to_string(hid);
    cout << "box: <" << box.min_corner << ", " << box.max_corner << ">" << endl;
    if(left) {
        cout << endl << "left:" << endl;
        left->print();
    } else { cout << "no left." << endl; }
    if(right) {
        cout << endl << "right:" << endl;
        right->print();
    } else { cout << "no right." << endl; }
}

#endif
