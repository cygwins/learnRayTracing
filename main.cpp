#include <iostream>
#include <fstream>
#include <string>
#include "float.h"
#include "timer.h"
#include "camera.h"
#include "hitable_list.h"
#include "material.h"
#include "sphere.h"
using std::cout;
using std::endl;
using std::flush;
using std::ifstream;
using std::ofstream;
using std::string;

void PPMHeader(ofstream &ofs, int x, int y) {
    ofs << "P3" << endl << x << " " << y << endl << 255 << endl;
}

vec3 color(const ray& r, hitable_list& world, int depth) {
    hit_record rec;
    if(world.hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        } else {
            return vec3(0,0,0); // exceed max iteration limit
        }
    } else {
        vec3 dir = normalized(r.direction());
        float t = 0.5*(dir.y + 1.0);
        return t*vec3(0.7, 0.95, 1) + (1.0-t)*vec3(1, 1, 1); // from greenish ground to white sky
    }
}

hitable_list* my_scene() {
    hitable_list *world = new hitable_list;
    world->add(new sphere( vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)) ));
    world->add(new sphere( vec3(0, 1, 0), 1.0, new dielectric(1.5) ));
    world->add(new sphere( vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)) ));
    world->add(new sphere( vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0) ));
    for (int a = -11; a < 11; ++a) { for (int b = -11; b < 11; ++b) {
        float mat = drand48();
        float radius = 0.2 + 0.05*drand48();
        vec3 center(a + 0.9*drand48(), radius, b + 0.9*drand48());
        if((center - vec3(4, 0.2, 0)).length() <= 0.9) {
            continue;
        }
        if(mat < 0.6) { // diffuse
            world->add(new moving_sphere(center, center + vec3(0,0.5*drand48(),0), 0, 1, radius, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())) ));
        } else if(mat < 0.85) { // metal
            world->add(new sphere(center, radius, new metal(vec3(0.6+0.4*drand48(), 0.6+0.4*drand48(), 0.6+0.4*drand48()), 0.4*drand48()) ));
        } else { // glass
            world->add(new sphere(center, radius, new dielectric(1.5) ));
        }
    }}
    cout << "Random scene generated. Total ball number: " << world->list.size() << endl;
    return world;
}

void draw(ofstream& ofs, bool show_time = true) {
    timer draw_timer;
    draw_timer.begin();
    // basic config
    int PIX_WIDTH = 400;
    int PIX_HEIGHT = 300;
    int ANTIALIAS_N = 2;

    PPMHeader(ofs, PIX_WIDTH, PIX_HEIGHT);

    float width_height_ratio = float(PIX_WIDTH) / float(PIX_HEIGHT);

    // set up stage
    // hitable_list *world = new hitable_list;
    // world->add(new sphere( vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)) ));
    // world->add(new sphere( vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0)) ));
    // world->add(new sphere( vec3(1, 0, -1), 0.5, new metal(vec3(0.9, 0.6, 0.3), 0.5) ));
    // world->add(new sphere( vec3(-1, 0, -1), 0.5, new dielectric(1.5) ));
    // world->add(new sphere( vec3(-1, 0, -1), -0.45, new dielectric(1.5) ));

    hitable_list* world = my_scene();

    // set up camera
    vec3 lookfrom(13,2,3), lookat(0,0,0);
    float dist_to_focus = 10; // (lookfrom - lookat).length();
    float aperture = 0.0;
    camera cam(lookfrom, lookat, vec3(0,1,0), 20, width_height_ratio, aperture, dist_to_focus, 0, 1);

    int percent = -1, tmp_per, progdisp = 5; // display progress while rendering
    // traverse pixel-wise
    for (int i = PIX_HEIGHT - 1; i >= 0; --i) {
        for (int j = 0; j < PIX_WIDTH; ++j) {
            vec3 col(0,0,0);
            for (int k = 0; k < ANTIALIAS_N; ++k) {
                float u = float(j + drand48()) / float(PIX_WIDTH); // u, v in [0, 1] from lower_left_corner
                float v = float(i + drand48()) / float(PIX_HEIGHT);
                ray r = cam.get_ray(u, v);
                col += color(r, *world, 0);
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
                for(int i = 0; i < percent/progdisp; ++i) {
                    cout << "=";
                }
                for(int i = percent/progdisp; i < 100/progdisp; ++i) {
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