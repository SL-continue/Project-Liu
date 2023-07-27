// Termm--Fall 2020

#include "GeometryNode.hpp"
#include <cmath>

double pi = glm::pi<double>();

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

bool GeometryNode::intersect(const Ray& ray, Intersection& i) const {

	Ray ray_local (invtrans * ray.origin, invtrans * (ray.origin + ray.direction));
	// Ray ray_local (ray.origin, (ray.origin + ray.direction));

	// std::cout << "ray_local: " << ray_local << std::endl;
	Intersection j;
	bool intersected = m_primitive->intersect(ray_local, j);
	if (intersected){
		// std::cout << m_name << std::endl;
		i = j;
		i.point = trans * i.point;
		i.normal = glm::normalize(glm::transpose(invtrans) * i.normal);
		i.m = m_material;
		i.intersected_id = id;
		i.ambient = this->ambient;
		i.intersected = true;
	}

	return (intersected || SceneNode::intersect(ray, i));
}

void GeometryNode::divide_into_patches(uint n){
	std::vector<Plane*>  patches = ((Plane*) m_primitive)->divide_into_patches(n);
	for (Plane* patch : patches){
		GeometryNode* patch_node = new GeometryNode(this->m_name, patch, m_material);
		patch_node->set_transform(trans);
		this->parent->add_child(patch_node);
		patch_node->setMaterial(m_material);
	}
	this->parent->remove_child(this);
}

std::vector<double> GeometryNode::formfactor_montecarlo(SceneNode* root, size_t patch_number){
	std::vector<double> patch_hits (patch_number, 0);
	// change ray_numbers
	size_t ray_number = 10000;
	for (size_t count = 1; count <= ray_number; count++){
		// generate random rays
		double theta = glm::acos(1.0 - glm::linearRand(0.0f, 1.0f));
		double phi = 2 * pi * glm::linearRand(0.0f, 1.0f);
		
		glm::vec3 randomDirection;
		randomDirection.x = sin(theta) * cos(phi);
		randomDirection.y = cos(theta);
		randomDirection.z = sin(theta) * sin(phi);
		// convert to world coordinates
		glm::vec3 local_pos = this->m_primitive->get_pos();
		Ray ray (this->trans * local_pos, this->trans * (local_pos + randomDirection));
		Intersection j;
		bool intersected = root->intersect(ray, j);
		// should hit for all rays
		if (intersected){
			patch_hits[j.intersected_id] += 1.0;
		}
	}
	for (size_t i = 0; i < patch_hits.size(); i ++){
		patch_hits[i] = patch_hits[i]  / ray_number;
	}

	return patch_hits;
}

double length_sqr(glm::vec3 vec){
	return pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2);
}

std::vector<double> GeometryNode::formfactor_hemicube(SceneNode* root, size_t patch_number){
	std::vector<double> form_factor (patch_number, 0);
	double delta_sum = 0.0;
	// delta form factor for top surface
	// the side length of the cube is 2 (for similfying calculation)
	float cell_size = 0.1;
	glm::vec3 start_pos (-0.95f, 0.0f, -0.95f);
	glm::vec3 local_pos = this->m_primitive->get_pos();
	for (size_t row = 0; row < 20; row++){
		for (size_t col = 0; col < 20; col++){
			// center of small patch on surface
			// this is centered by original point
			glm::vec3 direction (start_pos.x + row * cell_size, 1.0f, start_pos.z + col * cell_size);
			// delta form factor = 1 / pi * (x^2 + y^2 + z^2)^2 delta_A
			// area is 0.1 * 0.1 = 0.01
			double delta_formfactor = 1.0 / (pi * pow(length_sqr(direction), 2)) * 0.01;

			// translate the direction on the primitive and convert ray to world frame
			Ray ray(this->trans * local_pos, this->trans * (local_pos + direction));
			Intersection j;
			bool intersected = root->intersect(ray, j);
			if (intersected){
				form_factor[j.intersected_id] += delta_formfactor;
				delta_sum += delta_formfactor;
			}
		}	
	}

	// side walls
	// height is 1, length is 2
	// 10 * 20 cells
	// left and right
	for (int f = 0; f < 2; f++){
		for (size_t row = 0; row < 10; row ++){
			for (size_t col = 0; col < 20; col ++){
				glm::vec3 direction = (f == 0)? glm::vec3(- 1.0, 0.05f + row * cell_size, -0.95f + col * cell_size) : glm::vec3(1.0, 0.05f + row * cell_size, -0.95f + col * cell_size);
				double delta_formfactor = direction.y / (pi * pow(length_sqr(direction), 2)) * 0.01;
				Ray ray(this->trans * local_pos, this->trans * (local_pos + direction));
				Intersection j;
				bool intersected = root->intersect(ray, j);
				if (intersected){
					form_factor[j.intersected_id] += delta_formfactor;
					delta_sum += delta_formfactor;
				}
			}
		}
	}

	// front and back
	for (int f = 0; f < 2; f++){
		for (size_t row = 0; row < 10; row ++){
			for (size_t col = 0; col < 20; col ++){
				glm::vec3 direction = (f == 0)? glm::vec3(-0.95f + col * cell_size, 0.05f + row * cell_size, - 1.0) : glm::vec3(-0.95f + col * cell_size, 0.05f + row * cell_size, 1.0);
				double delta_formfactor = direction.y / (pi * pow(length_sqr(direction), 2)) * 0.01;
				Ray ray(this->trans * local_pos, this->trans * (local_pos + direction));
				Intersection j;
				bool intersected = root->intersect(ray, j);
				if (intersected){
					form_factor[j.intersected_id] += delta_formfactor;
					delta_sum += delta_formfactor;
				}
			}
		}
	}

	for (size_t i = 0; i < form_factor.size(); i ++){
		form_factor[i] = form_factor[i]  / delta_sum;
	}
	return form_factor;
}

void GeometryNode::set_colour(const glm::vec3& colour){
	ambient = colour;
}