#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable  {
    public:
        sphere() {}
        sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        vec3 center;
        float radius;
        material *mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        rec.mat_ptr=mat_ptr; // set material for hit record (was causing segfaults without this!)
        float temp = (-b - sqrt(discriminant))/a;
        // object must be nearer than nearest so far, and not behind the camera
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            vec3 tmp = rec.p - center;
            rec.normal = tmp / radius;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        // object must be nearer than nearest so far, and not behind the camera
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            vec3 tmp = rec.p - center;
            rec.normal = tmp / radius;
            return true;
        }
    }
    return false;
}


#endif