// Termm--Fall 2020

#include "Primitive.hpp"
#include "polyroots.hpp"

#include <iostream>
#include <algorithm>

Primitive::~Primitive()
{
}

bool Primitive::intersect(const Ray& ray, Intersection& i) const {
    return false;
}


Plane::~Plane()
{
}

Plane::Plane() : nh_plane(glm::vec3(0.0, 0.0, 0.0), 1.0) {}

bool Plane::intersect(const Ray& ray, Intersection& j) const
{
  return nh_plane.intersect(ray, j);
}


NonhierPlane::~NonhierPlane() {}

bool NonhierPlane::intersect(const Ray& ray, Intersection& i) const
{
  glm::vec3 normal(0.0, 1.0, 0.0);

  double denom = glm::dot(normal, ray.direction);
  // std::cout << ray<< " " << denom << std::endl;
  
  // consider only rays going from front
  if (denom > 0){
    return false;
  }
  if(fabs(denom) < std::numeric_limits<double>::epsilon()) return false;

  double t = glm::dot(normal, m_pos-ray.origin) / denom;
  // std::cout << "t: " << t << ray << std::endl;
  if(t < 0) return false;

  glm::vec3 P = ray.origin + t*ray.direction;

  double size = m_size / 2.0;
  if(P[0] < (m_pos[0]-size) || P[0] > (m_pos[0]+size)) return false;
  if(P[2] < (m_pos[2]-size) || P[2] > (m_pos[2]+size)) return false;

  i.point = P;
  i.normal = normal;
  i.intersected = true;

  return true;
}

std::vector<Plane*> Plane::divide_into_patches(uint n) {
    std::vector<Plane*> patches;

    // size of each smaller patch
    double patchSize = nh_plane.get_size() / n;

    // top-left corner
    double startX = nh_plane.get_pos().x - (nh_plane.get_size() / 2.0) + (patchSize / 2.0);
    double startZ = nh_plane.get_pos().z - (nh_plane.get_size() / 2.0) + (patchSize / 2.0);

    // Divide the plane into n * n smaller patches
    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < n; ++j) {
            // position of the top-left corner of each smaller patch
            glm::vec3 patchPos = glm::vec3(startX + i * patchSize, nh_plane.get_pos().y, startZ + j * patchSize);

            // Create a new NonhierPlane representing the smaller patch
            NonhierPlane patch (patchPos, patchSize);

            // Add the smaller patch to the vector of patches
            patches.push_back(new Plane(patch));
        }
    }

    return patches;
}