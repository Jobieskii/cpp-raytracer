#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "config.h"

#include "hittable.h"
#include "vec3.h"
#include <atomic>

class camera {
  public:
    camera(int width, double aspect_ratio, double focal_length, point3 camera_center)
        : image_width(width), image_height(image_width / aspect_ratio),
          aspect_ratio(aspect_ratio), viewport_height(2.0), camera_center(camera_center) {
        viewport_width = viewport_height * (double(image_width) / image_height);
        viewport_u = vec3(viewport_width, 0, 0);
        viewport_v = vec3(0, -viewport_height, 0);

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        viewport_upper_left = camera_center - vec3(0, 0, focal_length) -
                              viewport_u / 2 - viewport_v / 2;
        pixel00_loc =
            viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }
    void render(const hittable& world);

  private:
    double viewport_width, viewport_height, aspect_ratio, focal_length;
    int image_width, image_height;
    vec3 viewport_u, viewport_v, pixel_delta_u, pixel_delta_v,
        viewport_upper_left, pixel00_loc;
    point3 camera_center;
    static constexpr double pixel_samples_scale = 1.0 / SAMPLES_PER_PIXEL;

    std::vector<color> render_line(const hittable& world, int y) const;
    void task(const hittable& world, std::vector<color>& image_colors, std::atomic_int& next_line) const;
    color ray_color(const ray& r, const hittable& world, int max_depth) const;
};

#endif