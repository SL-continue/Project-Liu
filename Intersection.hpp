#pragma once

#include <glm/glm.hpp>

#include "Ray.hpp"

class Ray;
class Material;
class GeometryNode;

struct Intersection {
    
    Intersection() : intersected(false), point(glm::vec3(0.0f)), normal(glm::vec3(0.0f)), intersected_id(-1), m(nullptr), isLight(false), light_color(glm::vec3 (0.0f)) {}

    Intersection(const Intersection& other) : intersected(other.intersected), point(other.point), normal(other.normal), intersected_id(-1), m(other.m),isLight(other.isLight),light_color(other.light_color) {}

    ~Intersection() {};

    bool intersected;
    int intersected_id;
    
    glm::vec3 point; // Intersection point
    glm::vec3 normal; // Intersection normal
    bool isLight; // intersect with light
    glm::vec3 light_color;
    Material * m;
    glm::vec3 ambient;
};