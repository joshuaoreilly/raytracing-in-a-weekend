#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"

vec3 random_in_unit_sphere() {
    /*
    randomly sample points in a unit cube whose base touches the contact point
    (the 'origin' in this method), then reject them if they're not in the unit
    sphere contained in the unit cube.
    Apparently mimics Lambertian diffuse surfaces well
    */
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 color(const ray& r, hitable *world) {
    hit_record rec;
    // t_min = 0.001 instead of 0.0, since floating point can cause intersection
    // with self at 0.0000...1
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        // bounce ray off of contact point rec.p with direction rec.normal and random
        // component random_in_unit_sphere
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        // recursion: continue to bounce the ray off objects until it misses
        // ray origin is now contact point, ray origin is now the direction it
        // bounced off and towards
        // the more it bounces, the more energy it loses (half with each bounce)
        return 0.5 * color( ray(rec.p, target - rec.p), world);
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        // white to blue background gradient
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    int nx = 200;
    int ny = 100;
    int ns = 100; // number of samples
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    hitable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    // really big sphere that'll look like ground when only looking at the top of it
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(list, 2);
    camera cam;
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                // drand48 gives value 0 <= x < 1, so we're sampling between current pixel and next in x and y
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                // doesn't actually do anything, since the contact point is now
                // contained in hit_record; maybe being kept around for future chapters?
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world);
            }
            col /= float(ns); // average (divide by number of samples)
            // gamma correction thingy
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            std:: cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}