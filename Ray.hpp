#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/io.hpp>

struct Ray{
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(const glm::vec3& ori,const glm::vec3& destination): origin(ori), direction(destination - ori) { }

    Ray(const Ray & ray) : origin(ray.origin), direction(ray.direction) { }

    glm::vec3 r_t(double t) const {
        return origin + (float) t * direction;
    }

    friend std::ostream & operator << (std::ostream & os, const Ray & ray) {
        os << ray.origin << " " << ray.direction << std::endl;
	    return os;
    }
};

inline glm::vec3 operator*(const glm::mat4& m, const glm::vec3& v) {
    glm::vec4 result = m * glm::vec4(v, 1.0f);
    return glm::vec3(result.x, result.y, result.z);
}

inline glm::vec3 operator*(const double& d, const glm::vec3& v) {
    return (float) d * v;
}

inline glm::vec3 operator/(const glm::vec3& v, const double& d) {
    return v / (float) d;
}

