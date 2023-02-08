#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

vec3 random_in_unit_disk() {
    /*
    randomly sample points in a unit square, then reject them if they're not
    in the unit circle contained in the unit square.
    */
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), 0) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

class camera {
    public:
        camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) {
            // vfov = vertical field of view from top-to-bottom in degrees (not just from camera normal to top)
            // aspect = aspect ratio (between vertical and horizontal)
            lens_radius = aperture / 2;
            float theta = vfov * M_PI / 180;
            float half_height = tan(theta / 2);
            float half_width = aspect * half_height;
            origin = lookfrom;
            w = unit_vector(lookfrom - lookat); // w points away from where the camera is pointing
            u = unit_vector(cross(vup, w)); // cross of vup (true up in scene) and w gives vector along camera x axis
            v = cross(w, u); // gives vector along camera y axis
            //lower_left_corner = vec3(-half_width, -half_height, -1.0);
            // all vectors now that we're no longer nicely aligned with our world frame anymore
            lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
            horizontal = 2 * half_width * focus_dist * u;
            vertical = 2 * half_height * focus_dist * v;
        }
        
        ray get_ray(float s, float t) {
            // little change of notation since u and v already used
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();
            return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
        }

        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        float lens_radius;
};

#endif