#include <iostream>
#include "ray.h"

float hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction()); // dot(B, B)
    float b = 2.0 * dot(oc, r.direction()); // 2.0 * dot(B, A-C)
    float c = dot(oc, oc) - radius*radius; // dot(A-C, A-C) - R*R
    float discriminant = b*b - 4*a*c;
    // if below 0, ray doesn't hit sphere
    // if single root (descriminant = 0), one intersection with sphere (perfectly tangential)
    // if two roots (descriminant > 0), two intersections (passes through sphere)
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

vec3 color(const ray& r) {
    float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1));
        return 0.5 * vec3(N.x()+1, N.y()+1, N.z()+1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0); // scales a -1.0<x<1.0 vector to 0.0<x<1.0 one
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
    int nx = 200;
    int ny = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            vec3 col = color(r);
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            std:: cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}