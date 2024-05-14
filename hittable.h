#ifndef HITTABLE_H
#define HITTABLE_H

#include "config.h"
#include "interval.h"
#include "ray.h"
#include <memory>

class material;

class hit_record {
  public:
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat;
    double t;
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif