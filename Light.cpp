// Termm--Fall 2020

#include <iostream>

#include <glm/ext.hpp>

#include "Light.hpp"

Light::Light()
  : colour(0.0, 0.0, 0.0),
    position(0.0, 0.0, 0.0)
{
  falloff[0] = 1.0;
  falloff[1] = 0.0;
  falloff[2] = 0.0;
}

double Light::attenuation(double distance) const{
  return (1.0 / (falloff[0] + falloff[1] * distance + falloff[2] * (distance * distance)));
}

glm::vec3 Light::get_pos() const{
  return position;
}

bool Light::intersect(const Ray& ray, Intersection& i) const
{
  glm::vec3 u = glm::normalize(position - ray.origin);
  glm::vec3 v = glm::normalize((ray.origin + 1.0*ray.direction) - ray.origin);

  if(fabs(glm::dot(u, v) - 1.0) > std::numeric_limits<double>::epsilon()) return false;

  i.point = position;
  i.normal = -u;
  i.isLight = true;
  i.light_color = colour;

  return true;
}

std::ostream& operator<<(std::ostream& out, const Light& l)
{
  out << "L[" << glm::to_string(l.colour) 
  	  << ", " << glm::to_string(l.position) << ", ";
  for (int i = 0; i < 3; i++) {
    if (i > 0) out << ", ";
    out << l.falloff[i];
  }
  out << "]";
  return out;
}


AreaLight::AreaLight()
  : colour(0.0, 0.0, 0.0),
    position(0.0, 0.0, 0.0),
    length(1.0)
{
  falloff[0] = 1.0;
  falloff[1] = 0.0;
  falloff[2] = 0.0;
  normal = glm::vec3(0.0f, 1.0f, 0.0f);
}

AreaLight::AreaLight(const glm::vec3& pos, double length)
  : position(pos), length(length), colour(1.0, 1.0, 1.0)
{  
  falloff[0] = 1.0;
  falloff[1] = 0.0;
  falloff[2] = 0.0;
  normal = glm::vec3(0.0f, 1.0f, 0.0f);
}

AreaLight::AreaLight(const glm::vec3& pos, glm::vec3 colour, double length) : position(pos), colour(colour), length(length) {
  falloff[0] = 1.0;
  falloff[1] = 0.0;
  falloff[2] = 0.0;
  normal = glm::vec3(0.0f, 1.0f, 0.0f);
}

AreaLight::AreaLight(AreaLight* light)
: position(light->position), length (light->length), colour(light->colour){
  falloff[0] = light->falloff[0];
  falloff[1] = light->falloff[1];
  falloff[2] = light->falloff[2];
} 

glm::vec3 AreaLight::get_pos() const {
  // double u = glm::linearRand(0.0, 1.0);
  // double v = glm::linearRand(0.0, 1.0);
  // return glm::vec3(position.x + length * (u - 0.5), position.y, position.z + length * (v - 0.5));
  return position;
}

bool AreaLight::intersect(const Ray& ray, Intersection& i) const
{
  glm::vec3 to_plane = position - ray.origin;
  glm::vec3 normal (0, -1, 0);
  double t = glm::dot(to_plane, normal) / glm::dot(ray.direction, normal);
  glm::vec3 point = ray.r_t(t);

  if (fabs(point.x - position.x) < 0.5 * length && fabs(point.z - position.z) < 0.5 * length){
    i.normal = normal;
    i.point = point;
    return true;
  }

  return false;
}
