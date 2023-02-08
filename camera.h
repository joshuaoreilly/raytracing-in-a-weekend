#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera {
    public:
        camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) {
            // vfov = vertical field of view from top-to-bottom in degrees (not just from camera normal to top)
            // aspect = aspect ratio (between vertical and horizontal)
            vec3 u, v, w;
            float theta = vfov * M_PI / 180;
            float half_height = tan(theta / 2);
            float half_width = aspect * half_height;
            origin = lookfrom;
            w = unit_vector(lookfrom - lookat); // w points away from where the camera is pointing
            u = unit_vector(cross(vup, w)); // cross of vup (true up in scene) and w gives vector along camera x axis
            v = cross(w, u); // gives vector along camera y axis
            //lower_left_corner = vec3(-half_width, -half_height, -1.0);
            // all vectors now that we're no longer nicely aligned with our world frame anymore
            lower_left_corner = origin - half_width * u - half_height * v - w;
            horizontal = 2 * half_width * u;
            vertical = 2 * half_height * v;
        }
        
        ray get_ray(float s, float t) {
            // little change of notation since u and v already used
            return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
        }

        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif