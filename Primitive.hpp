// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "Intersection.hpp"
#include <vector>

class Primitive {
public:
  virtual ~Primitive();
  virtual glm::vec3 get_pos() const = 0;
  virtual double get_size() const = 0;
  virtual void enlarge(double factor) = 0;
  virtual bool intersect(const Ray&, Intersection&) const = 0;
};

class NonhierPlane : public Primitive {
public:
  NonhierPlane(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  
  virtual ~NonhierPlane();

  virtual bool intersect(const Ray&, Intersection&) const;
  glm::vec3 get_pos() const { return m_pos; }
  double get_size() const { return m_size; }
  void enlarge(double factor) { m_size *= factor; }

private:
  // m_pos is the position of the center
  // all planes considered squares
  // normal always (0,1,0) in local coordinates
  glm::vec3 m_pos;
  double m_size;
};

class Plane : public Primitive {
  NonhierPlane nh_plane;
public:
  Plane();
  Plane(NonhierPlane nh_plane) : nh_plane (nh_plane){};
  virtual ~Plane();
  virtual bool intersect(const Ray&, Intersection&) const;
  std::vector<Plane*> divide_into_patches(uint n);
  glm::vec3 get_pos() const { return nh_plane.get_pos(); }
  double get_size() const { return nh_plane.get_size(); }
  void enlarge(double factor) { nh_plane.enlarge(factor); }
};

