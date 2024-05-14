#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "config.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include <algorithm>
#include <cstdlib>

inline double fresnel(const ray& r_in, const hit_record& rec) {
    double val = std::clamp(std::fabs(dot(unit_vector(r_in.direction()), rec.normal)), 0.0001, 1.0);
    return val;
}

class material {
  public:
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         color& attenuation, ray& scattered) const {
        return false;
    }
};

class lambertian : public material {
  public:
    lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        auto scatter_direction = rec.normal + vec3::random_unit_vector();
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

  private:
    color albedo;
};

class metal : public material {
  public:
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = vec3::random_unit_vector() * fuzz * fresnel(r_in, rec) + unit_vector(reflected);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

  private:
    double fuzz;
    color albedo;
};

#endif