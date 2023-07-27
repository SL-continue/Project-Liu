// Termm--Fall 2020

#pragma once

#include <vector>
#include <iostream>
#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include "PhongMaterial.hpp"
#include <glm/gtc/random.hpp>
#include <glm/gtx/io.hpp>

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );

	bool intersect(const Ray& ray, Intersection& i) const;
	void divide_into_patches(uint n);

	void set_colour(const glm::vec3& colour);

	std::vector<double> formfactor_hemicube(SceneNode* root, size_t patch_number);
	std::vector<double> formfactor_montecarlo(SceneNode* root, size_t patch_number);

	int id;
	Material *m_material;
	Primitive *m_primitive;
	glm::vec3 ambient;
};
