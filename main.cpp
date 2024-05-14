#include "camera.h"
#include "config.h"
#include "hittable_list.h"
#include "interval.h"
#include "sphere.h"
#include "vec3.h"

#include <iostream>
#include <math.h>

int main() {

    // World 
    auto sp = make_shared<sphere>(sphere(point3(0, 0, -1), 0.5));
    auto sp2 = make_shared<sphere>(sphere(point3(1, -0.25, -2), 0.5));
    auto sp3 = make_shared<sphere>(sphere(point3(0, -150.5, -1), 150));
    hittable_list world = {sp, sp2, sp3};
    // Render

    camera camera {400, 16.0/9.0, 1.0, 100, point3(0, 0, 0)};
    camera.render(world);
}
