#include "camera.h"
#include "color.h"
#include "config.h"
#include "material.h"

vec3 sample_square() {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit
    // square.
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

void camera::render(const hittable& world) {
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' '
                  << std::flush;
        for (int i = 0; i < image_width; i++) {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < SAMPLES_PER_PIXEL; sample++) {
                vec3 offset = sample_square();
                auto pixel_center = pixel00_loc +
                                    ((i + offset.x()) * pixel_delta_u) +
                                    ((j + offset.y()) * pixel_delta_v);
                ray r{camera_center, pixel_center - camera_center};
                pixel_color += ray_color(r, world, BOUNCES);
            }
            write_color(std::cout, pixel_samples_scale * pixel_color);
        }
    }
    std::clog << "Done" << std::endl;
}
color camera::ray_color(const ray& r, const hittable& world,
                        int max_depth) const {
    if (max_depth <= 0) {
        return color(0, 0, 0);
    }
    hit_record hit_rec;
    bool hit = world.hit(r, interval(0.001, infinity), hit_rec);
    if (hit) {
        ray scattered;
        color attenuation;
        if (hit_rec.mat->scatter(r, hit_rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, max_depth - 1);
        return color(0, 0, 0);
    }
    vec3 unit = unit_vector(r.direction());
    double a = (unit.y() + 1.0) * 0.5;
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
