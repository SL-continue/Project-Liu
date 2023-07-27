#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"

class Camera {
public:
  	Camera(glm::vec3 eye, glm::vec3 view, glm::vec3 up, double fovy, int width, int height);

  	Ray castRay(glm::vec2 pixel_pos);

  	const glm::vec3 getPos() const { return m_lookfrom; }
  	const glm::vec3 getDir() const { return m_direction; }

  	int width()  const { return m_width;  }
  	int height() const { return m_height; }

private:
	glm::mat4 m_screentoworld;
	glm::vec3 m_lookfrom;
	glm::vec3 m_direction;
	glm::vec3 m_up;

	double m_fovy;
	int m_width, m_height;
};
