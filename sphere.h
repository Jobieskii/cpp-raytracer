#ifndef SPHERE_H
#define SPHERE_H

#include "config.h"
#include "hittable.h"
#include "material.h"
#include <memory>

class sphere : public hittable {
  public:
    sphere(const point3& center, double radius, std::shared_ptr<material> mat) : center(center), radius(fmax(0,radius)), mat(mat) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

  private:
    point3 center;
    std::shared_ptr<material> mat;
    double radius;
};

#endif