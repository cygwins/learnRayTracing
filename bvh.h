#ifndef BVH_H_
#define BVH_H_
#include "hitable_list.h"

class bvh_node : public hitable { // bvh = bounding volumn hierarchy
public:
    bvh_node() {}
    bvh_node(vector<hitable*> l, float t0, float t1);
    bvh_node(vector<hitable*> l, float t0, float t1, float ratio);
    bvh_node(hitable_list *l, float t0, float t1) : bvh_node(l->list, t0, t1) {}
    bvh_node(hitable_list *l, float t0, float t1, float ratio) : bvh_node(l->list, t0, t1, ratio) {}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, aabb& box) const;
    virtual void print() const;

    hitable *left;
    hitable *right;
    aabb box;
};

bool hitable_compare_i(int axis, const hitable *a, const hitable *b, float t0, float t1);

bvh_node::bvh_node(vector<hitable*> list, float t0, float t1) { // random axis median
    // int axis = int(3*drand48());
    int axis = 1; // y
    cout << axis;
    sort(list.begin(), list.end(), [axis, t0, t1](hitable *a, hitable *b){
        return hitable_compare_i(axis, a, b, t0, t1);});
    if(list.size() <= 2) {
        left = list.size() > 0 ? list[0] : nullptr;
        right = list.size() > 1 ? list[1] : nullptr;
        depth = 1;
    } else {
        vector<hitable*> l = vector<hitable*>(list.begin(), list.begin() + list.size() / 2);
        vector<hitable*> r = vector<hitable*>(list.begin() + list.size() / 2, list.end());
        left = new bvh_node(l, t0, t1);
        right = new bvh_node(r, t0, t1);
        depth = left->depth > right->depth ? left->depth + 1 : right->depth + 1;
    }
    aabb box_left, box_right;
    if((left && !left->bounding_box(t0, t1, box_left)) ||
       (right && !right->bounding_box(t0, t1, box_right))) {
        cerr << "no bounding_box constructing bvh_node" << endl;
    }
    box = right ? surrounding_box(box_left, box_right) : box_left;
}

bvh_node::bvh_node(vector<hitable*> list, float t0, float t1, float ratio) { // axis with max var
    // ratio = diff dist to both ends : gap width
    // split tree at max (gap - diff)
    // if the ratio is big, tree values depth more and tends to split more evenly
    // if the ratio is small, tree values separation more, gap between two branches is maxed
    int N = list.size();
    if(N == 0) {
        cerr << "empty list constructing bvh_node" << endl;
    }
    vector<aabb> aabbs(N);
    vec3 avg = 0;
    for(int i = 0; i < N; ++i) {
        if(list[i]->bounding_box(t0,t1,aabbs[i])) {
            avg += aabbs[i].center;
        }
    }
    avg /= N;
    vec3 var = 0;
    for(int i = 0 ; i < N; ++i) {
        vec3 diff = aabbs[i].center - avg;
        var += diff * diff;
    }
    var /= N;
    int axis = 0; // choose the axis with maximum variance
    float max_var = var[0];
    if(var[1] > max_var) { axis = 1; max_var = var[1]; }
    if(var[2] > max_var) { axis = 2; max_var = var[2]; }

    //cout << endl << var << " choosing axis " << axis << endl;
    cout << axis;

    sort(list.begin(), list.end(), [axis, t0, t1](hitable *a, hitable *b){
        return hitable_compare_i(axis, a, b, t0, t1);});
    for(int i = 0; i < N; ++i) { // update aabbs after list is sorted
        list[i]->bounding_box(t0,t1,aabbs[i]);
    }

    if(N <= 2) {
        left = list[0];
        right = N > 1 ? list[1] : nullptr;
        depth = 1;
    } else {
        int mid = 1; // choose the mid point with maximum gap between 0~mid-1 & mid~N-1

        //// gap := right_box.min_corner[axis] - left_box.max_corner[axis]
        // vector<aabb> left_all(N), right_all(N);
        // left_all[0] = aabbs[0];
        // right_all[N-1] = aabbs[N-1];
        // for(int i = 1; i < N; ++i) { // incrementally bounding box till left/right most
        //     left_all[i] = surrounding_box(left_all[i-1], aabbs[i]);
        //     right_all[N-1-i] = surrounding_box(right_all[N-i], aabbs[N-1-i]);
        // }
        // float max_gap = right_all[1].min_corner[axis] - left_all[0].max_corner[axis];

        // gap := right_box.center - left_box.center
        float max_gap = aabbs[1].center[axis] - aabbs[0].center[axis];
        max_gap -= ratio * fabs(mid - (N / 2.0)); // maximized in the middle
        for(int i = 2; i < N; ++i) {
            // float gap = right_all[i].min_corner[axis] - left_all[i-1].max_corner[axis];
            float gap = aabbs[i].center[axis] - aabbs[i-1].center[axis];
            gap -= ratio * fabs(i - (N / 2.0));
            if(gap > max_gap) {
                max_gap = gap;
                mid = i;
            }
        }
        // cout << "choosing mid point at " << mid << " / " << N << " of gap " << max_gap << endl << endl;
        vector<hitable*> l = vector<hitable*>(list.begin(), list.begin() + mid);
        vector<hitable*> r = vector<hitable*>(list.begin() + mid, list.end());
        left = new bvh_node(l, t0, t1, ratio);
        right = new bvh_node(r, t0, t1, ratio);
        depth = left->depth > right->depth ? left->depth + 1 : right->depth + 1;
        // for(int i = 0; i < depth; ++i) { cout << " "; } cout << "DEPTH" << depth << endl;
        // for(int i = 0; i < depth; ++i) { cout << " "; } cout << "LEFT DEPTH " << left->depth << endl;
        // for(int i = 0; i < depth; ++i) { cout << " "; } cout << "RIGHT DEPTH " << right->depth << endl;
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

// compare bounding boxes by the axis of their centers
bool box_compare_x(const aabb &a, const aabb &b) {
    return a.center.x < b.center.x;
}

bool box_compare_y(const aabb &a, const aabb &b) {
    return a.center.y < b.center.y;
}

bool box_compare_z(const aabb &a, const aabb &b) {
    return a.center.z < b.center.z;
}

bool box_compare_i(int axis, const aabb &a, const aabb &b) {
    return a.center[axis] < b.center[axis];
}

bool hitable_compare_i(int axis, const hitable *a, const hitable *b, float t0, float t1) {
    aabb aa, bb;
    if(!a->bounding_box(t0,t1,aa) || !b->bounding_box(t0,t1,bb)) {
        cerr << "no bounding_box comparing hitable" << endl;
    }
    return box_compare_i(axis, aa, bb);
}

#endif