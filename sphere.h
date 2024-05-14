#ifndef SPHERE_H
#define SPHERE_H

#include "config.h"
#include "hittable.h"

class sphere : public hittable {
  public:
    sphere(const point3& center, double radius) : center(center), radius(fmax(0,radius)) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

  private:
    point3 center;
    double radius;
};

#endif