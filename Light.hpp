// Termm--Fall 2020

#pragma once

#include <iosfwd>

#include <glm/glm.hpp>

#include "Primitive.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

// Represents a simple point light.
class Light : public Primitive{
public:
  Light();
  
  virtual double attenuation(double distance) const;
  virtual glm::vec3 get_pos() const;
  virtual bool is_point() const { return true;}
  virtual bool intersect(const Ray& ray, Intersection& i) const;
  virtual double get_size() const { return 0;}
  virtual glm::vec3 get_colour() const { return colour; }
  virtual void enlarge(double factor) {}

  glm::vec3 colour;
  glm::vec3 position;
  double falloff[3];
};

class AreaLight : public Light {
public:
  AreaLight();
  AreaLight(const glm::vec3& pos, double length);
  AreaLight(const glm::vec3& pos, glm::vec3 colour, double length);
  AreaLight(AreaLight* other);

  virtual glm::vec3 get_pos() const;
  virtual double get_size() const { return length; }
  virtual glm::vec3 get_colour() const { return colour; }
  virtual bool is_point() const { return false;}
  virtual void enlarge(double factor) { length *= factor; }
  virtual void set_normal(glm::vec3 n){  normal = n; }

  virtual bool intersect(const Ray& ray, Intersection& j) const;

  double length;
  glm::vec3 colour;
  glm::vec3 position;
  double falloff[3];

  // normal vector in world space
  glm::vec3 normal;
};

std::ostream& operator<<(std::ostream& out, const Light& l);
