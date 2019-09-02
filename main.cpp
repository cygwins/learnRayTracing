#include <iostream>
#include <fstream>
#include <string>
#include <cfloat>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "timer.h"
#include "camera.h"
#include "bvh.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "aarect.h"
#include "box.h"
using std::cout;
using std::endl;
using std::flush;
using std::ifstream;
using std::ofstream;
using std::string;

void PPMHeader(ofstream &ofs, int x, int y) {
    ofs << "P3" << endl << x << " " << y << endl << 255 << endl;
}

vec3 color(const ray& r, hitable_list *list, int depth) {
    hit_record rec;
    if(list->hit(r, 0.001, FLT_MAX, rec)) {
        if(depth == 0) {
            // cout << rec.whom << endl;
        }
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return emitted + attenuation * color(scattered, list, depth + 1);
        } else { // exceed max iteration limit
            return emitted;
        }
    } else {
        if(depth == 0) {
            // cout << "back ground" << endl;
        }
        ////// from greenish ground to white sky
        // vec3 dir = normalized(r.direction());
        // float t = 0.5*(dir.y + 1.0);
        // return t*vec3(0.7, 0.95, 1) + (1.0-t)*vec3(1, 1, 1);
        //////
        return vec3(0,0,0); // dark background
    }
}

vec3 color0(const ray& r, hitable_list *list) { // no scatter
    hit_record rec;
    if(list->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation;
        } else {
            return vec3(0,0,0);
        }
    } else {
        vec3 dir = normalized(r.direction());
        float t = 0.5*(dir.y + 1.0);
        return t*vec3(0.7, 0.9, 1) + (1.0-t)*vec3(1, 1, 1); // from greenish ground to white sky
    }
}

hitable_list* myballs() {
    // bvh_node::HID = 0;
    hitable_list *balls = new hitable_list;
    // the ground is a giant ball (earth?)
    texture *checker = new checker_texture( new constant_texture(vec3(0.2, 0.3, 0.1)),
                                            new constant_texture(vec3(0.9, 0.9, 0.9)));
    balls->add(new sphere( vec3(0, -1000, 0), 1000, new lambertian(checker) ));
    
    // three big ball of each material
    balls->add(new sphere( vec3(0, 1, 0), 1.0, new dielectric(1.5) ));
    balls->add(new sphere( vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))) ));
    balls->add(new sphere( vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0) ));
    for (int a = -11; a < 11; ++a) { for (int b = -11; b < 11; ++b) {
        float mat = drand48();
        float radius = 0.2 + 0.05*drand48();
        vec3 center(a + 0.9*drand48(), radius, b + 0.9*drand48());
        if((center - vec3(4, 0.2, 0)).length() <= 0.9) {
            continue;
        }
        if(mat < 0.6) { // diffuse
            balls->add(new moving_sphere(center, center + vec3(0,0.5*drand48(),0), 0, 1, radius, new lambertian(new constant_texture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))) ));
        } else if(mat < 0.85) { // metal
            balls->add(new sphere(center, radius, new metal(vec3(0.6+0.4*drand48(), 0.6+0.4*drand48(), 0.6+0.4*drand48()), 0.4*drand48()) ));
        } else { // glass
            balls->add(new sphere(center, radius, new dielectric(1.5) ));
        }
    }}
    cout << "Random scene generated. Total ball number: " << balls->list.size() << endl;
    return balls;
}

hitable_list* marble_balls() {
    texture *pertext = new noise_texture(4);
    hitable_list *balls = new hitable_list;
    balls->add(new sphere( vec3(0, -1000, 0), 1000, new lambertian(pertext) ));
    balls->add(new sphere( vec3(0, 2, 0), 2, new lambertian(pertext) ));
    balls->add(new sphere( vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4,4,4))) ));
    balls->add(new xy_rect( 3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4,4,4))) ));
    return balls;
}

hitable_list* earth() {
    hitable_list *balls = new hitable_list;
    int nx, ny, nn;
    unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
    material *earth_mat = new lambertian(new image_texture(tex_data, nx, ny));
    material *marble_mat = new lambertian(new noise_texture(4));
    balls->add(new sphere( vec3(0, -1000, 0), 1000, marble_mat ));
    balls->add(new sphere( vec3(0, 2, 0), 2, earth_mat ));
    return balls;
}

hitable_list* cornell_box() {
    hitable_list *boxes = new hitable_list;
    material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
    material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
    boxes->add(new flip_normals(new yz_rect(0, 555, 0, 555, 555, green)));
    boxes->add(new yz_rect(0, 555, 0, 555, 0, red));
    boxes->add(new xz_rect(213, 343, 227, 332, 554, light));
    boxes->add(new xz_rect(0, 555, 0, 555, 0, white));
    boxes->add(new flip_normals(new xy_rect(0, 555, 0, 555, 555, white)));
    boxes->add(new box(vec3(130, 0, 65), vec3(295, 165, 230), white));
    boxes->add(new box(vec3(265, 0, 295), vec3(430, 330, 460), white));
    return boxes;
}

void draw(ofstream& ofs, bool show_time = true) {
    timer draw_timer;
    draw_timer.begin();
    // basic config
    int PIX_WIDTH = 600;
    int PIX_HEIGHT = 450;
    int ANTIALIAS_N = 40;

    PPMHeader(ofs, PIX_WIDTH, PIX_HEIGHT);

    float width_height_ratio = float(PIX_WIDTH) / float(PIX_HEIGHT);

    // set up stage
    hitable_list *world = new hitable_list;

    hitable_list *scene = cornell_box(); // change scene here
    bvh_node *bvh = new bvh_node(scene, 0, 1);
    world->add(bvh);

    //world_bvh->print();

    // set up camera
    //vec3 lookfrom(13,2,3), lookat(0,0,0);
    vec3 lookfrom(278, 278, -800), lookat(278,278,0);
    float dist_to_focus = 10; // (lookfrom - lookat).length();
    float aperture = 0.0;
    float vfov = 40; // virtical field of view
    camera cam(lookfrom, lookat, vec3(0,1,0), vfov, width_height_ratio, aperture, dist_to_focus, 0, 1);

    int percent = -1, tmp_per, percentPerChar = 5; // display progress while rendering
    // traverse pixel-wise
    for (int i = PIX_HEIGHT - 1; i >= 0; --i) {
        for (int j = 0; j < PIX_WIDTH; ++j) {
            vec3 col(0,0,0);
            // cout << "ray " << i << ", " << j << " hit ";
            for (int k = 0; k < ANTIALIAS_N; ++k) {
                float u = float(j + drand48()) / float(PIX_WIDTH); // u, v in [0, 1] from lower_left_corner
                float v = float(i + drand48()) / float(PIX_HEIGHT);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0); // balls vs world : list vs bvh
                //col += color0(r, world); // crippled color showing no reflection
            }
            col /= ANTIALIAS_N;
            col = sqrt(col); // vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2])); // gamma correction
            ofs << vec3i(col * 255.99) << endl;
        }
        if(show_time) { // (PIX_HEIGHT - i) % 2 == 0 || i == PIX_HEIGHT - 1) {
            tmp_per = (PIX_HEIGHT - i) * 100 / PIX_HEIGHT;
            if(tmp_per != percent) {
                percent = tmp_per;
                cout << "[";
                for(int i = 0; i < percent/percentPerChar; ++i) {
                    cout << "=";
                }
                for(int i = percent/percentPerChar; i < 100/percentPerChar; ++i) {
                    cout << " ";
                }
                cout << "] (" << percent << "%) ";
                draw_timer.show_elapsed();
                draw_timer.show_eta(float(PIX_HEIGHT - i)/float(PIX_HEIGHT));
                cout << "       \r" << flush;
            }
        }
    }
    draw_timer.end();
    if(show_time) draw_timer.show_summary();
}

int main(int argc, char const *argv[]) {
    string filename = "out.ppm";
    if(argc >= 2) {
        filename = argv[1];
    }
    ofstream ofs;
    ofs.open(filename);
    cout << "Begin. Output file: " << filename << endl;
    draw(ofs);
    return 0;
}