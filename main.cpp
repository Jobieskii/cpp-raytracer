#include "camera.h"
#include "color.h"
#include "config.h"
#include "hittable_list.h"
#include "interval.h"
#include "material.h"
#include "sphere.h"
#include "vec3.h"

#include <iostream>
#include <math.h>
#include <memory>

int main() {
    auto ground_mat = std::make_shared<lambertian>(color(0.8, 0.7, 0.1));
    auto mirror = make_shared<metal>(color(1.0, 1.0, 1.0), 0.1);
    auto mirror2 = make_shared<metal>(color(1.0, 1.0, 1.0), 0.5);
    auto red = make_shared<lambertian>(color(1.0, 0.0, 0.0));
    // World 
    auto sp = make_shared<sphere>(sphere(point3(0, 0, -1), 0.5, mirror));
    auto sp2 = make_shared<sphere>(sphere(point3(1, -0.25, -2), 0.5, mirror2));
    auto sp3 = make_shared<sphere>(sphere(point3(-1, 0.25, -0.5), 0.5, red));
    auto ground = make_shared<sphere>(sphere(point3(0, -150.5, -1), 150, ground_mat));
    hittable_list world = {sp, sp2, sp3, ground};
    // Render

    camera camera {1600, 16.0/9.0, 2.5, point3(0, 0, 2)};
    camera.render(world);
}
