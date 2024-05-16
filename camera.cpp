#include "camera.h"
#include "color.h"
#include "config.h"
#include "material.h"
#include "vec3.h"
#include <atomic>
#include <bits/chrono.h>
#include <chrono>
#include <cstring>
#include <iterator>
#include <thread>
#include <vector>

vec3 sample_square() {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit
    // square.
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}
std::vector<color> camera::render_line(const hittable& world, int y) const {
    std::vector<color> colors(image_width);
    for (int i = 0; i < image_width; i++) {
        color pixel_color(0, 0, 0);
        for (int sample = 0; sample < SAMPLES_PER_PIXEL; sample++) {
            vec3 offset = sample_square();
            auto pixel_center = pixel00_loc +
                                ((i + offset.x()) * pixel_delta_u) +
                                ((y + offset.y()) * pixel_delta_v);
            ray r{camera_center, pixel_center - camera_center};
            pixel_color += ray_color(r, world, BOUNCES);
        }
        colors[i] = pixel_samples_scale * pixel_color;
    }
    return colors;
}
void camera::task(const hittable& world, std::vector<color>& image_colors, std::atomic_int& next_line) const {
    for (int line = next_line.fetch_add(1); line < image_height; line = next_line.fetch_add(1)) {
        auto line_colors = render_line(world, line);
        int offset = line * image_width;
        std::memcpy(image_colors.data() + offset, line_colors.data(), std::size(line_colors) * sizeof(vec3));
    }
}

using namespace std::chrono_literals;

void camera::render(const hittable& world) {
    std::atomic_int line_counter = 0;
    std::vector<std::thread> threads;
    std::vector<color> colors (image_height * image_width);
    for (int i = 0; i < THREADS; i++) {
        std::thread t {&camera::task, this, std::cref(world), ref(colors), ref(line_counter)};
        threads.push_back(std::move(t));
    }
    for (int current = line_counter.load(); current < image_height; current = line_counter.load()) {
        std::clog << "\rScanlines remaining: " << (image_height - current) << ' '
                  << std::flush;
        std::this_thread::sleep_for(500ms);
    }

    for (std::thread& t : threads) {
        t.join();
    }
    write_colors_buf(std::cout, colors, image_width, image_height);
    std::clog << "Done" << std::endl;
}

color sky_color(vec3 unit_dir) {
    double a = (unit_dir.y() + 1.0) * 0.5;
    vec3 sun = unit_vector(vec3(3, 4, -1));
    if (dot(unit_dir, sun) > 0.9) {
        return color(1, 1, 1);
    }
    return (1.0 - a) * color(0.1, 0.1, 0.1) + a * color(0.05, 0.07, 0.1);
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
    return sky_color(unit);
}
