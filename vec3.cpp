#include "vec3.h"

vec3 vec3::random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}
vec3 vec3::random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) >
        0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}