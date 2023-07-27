// Termm--Fall 2020

#include "SceneNode.hpp"
#include "GeometryNode.hpp"

#include "cs488-framework/MathUtils.hpp"

#include <iostream>
#include <sstream>
using namespace std;

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <eigen3/Eigen/Dense>

using namespace glm;


// Static class variable
unsigned int SceneNode::nodeInstanceCount = 0;

// helper functions
std::vector<std::vector<double>> readVectorFromFile(const std::string& filename) {
    std::vector<std::vector<double>> data;

    std::ifstream inputFile(filename);

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::vector<double> row;
            std::stringstream ss(line);
            std::string cell;

            while (std::getline(ss, cell, ',')) {
                // Convert the cell (string) to a double and add it to the row
                row.push_back(std::stod(cell));
            }

            data.push_back(row);
        }

        inputFile.close();
        std::cout << "Data read successfully from " << filename << std::endl;
    } else {
        std::cerr << "Error opening the file " << filename << std::endl;
    }

    return data;
}


void saveVectorToFile(const std::vector<std::vector<double>>& data, const std::string& filename) {
    std::ofstream outputFile(filename);

    if (outputFile.is_open()) {
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                outputFile << row[i];
                if (i < row.size() - 1) {
                    outputFile << ",";
                }
            }
            outputFile << "\n";
        }

        outputFile.close();
        std::cout << "Data saved successfully to " << filename << std::endl;
    } else {
        std::cerr << "Error opening the file " << filename << std::endl;
    }
}


//---------------------------------------------------------------------------------------
SceneNode::SceneNode(const std::string& name)
  : m_name(name),
	m_nodeType(NodeType::SceneNode),
	trans(mat4()),
	invtrans(mat4()),
	m_nodeId(nodeInstanceCount++)
{
	parent = NULL;
}

//---------------------------------------------------------------------------------------
// Deep copy
SceneNode::SceneNode(const SceneNode & other)
	: m_nodeType(other.m_nodeType),
	  m_name(other.m_name),
	  trans(other.trans),
	  invtrans(other.invtrans)
{
	for(SceneNode * child : other.children) {
		this->children.push_front(new SceneNode(*child));
	}
}

//---------------------------------------------------------------------------------------
SceneNode::~SceneNode() {
	for(SceneNode * child : children) {
		delete child;
	}
}

//---------------------------------------------------------------------------------------
void SceneNode::set_transform(const glm::mat4& m) {
	trans = m;
	invtrans = glm::inverse(m);
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_transform() const {
	return trans;
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_inverse() const {
	return invtrans;
}

//---------------------------------------------------------------------------------------
void SceneNode::add_child(SceneNode* child) {
	children.push_back(child);
	child->parent = this;
}

//---------------------------------------------------------------------------------------
void SceneNode::remove_child(SceneNode* child) {
	children.remove(child);
	child->parent = NULL;
}

//---------------------------------------------------------------------------------------
void SceneNode::rotate(char axis, float angle) {
	vec3 rot_axis;

	switch (axis) {
		case 'x':
			rot_axis = vec3(1,0,0);
			break;
		case 'y':
			rot_axis = vec3(0,1,0);
	        break;
		case 'z':
			rot_axis = vec3(0,0,1);
	        break;
		default:
			break;
	}
	mat4 rot_matrix = glm::rotate(degreesToRadians(angle), rot_axis);
	set_transform( rot_matrix * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::scale(const glm::vec3 & amount) {
	set_transform( glm::scale(amount) * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::translate(const glm::vec3& amount) {
	set_transform( glm::translate(amount) * trans );
}


//---------------------------------------------------------------------------------------
int SceneNode::totalSceneNodes() const {
	return nodeInstanceCount;
}

//---------------------------------------------------------------------------------------
std::ostream & operator << (std::ostream & os, const SceneNode & node) {

	//os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
	switch (node.m_nodeType) {
		case NodeType::SceneNode:
			os << "SceneNode";
			break;
		case NodeType::GeometryNode:
			os << "GeometryNode";
			break;
		case NodeType::JointNode:
			os << "JointNode";
			break;
	}
	os << ":[";

	os << "name:" << node.m_name << ", ";
	os << "id:" << node.m_nodeId;

	os << "]\n";
	return os;
}

bool SceneNode::intersect(const Ray& ray, Intersection& i) const{
	// Ray ray_local (invtrans * ray.origin, invtrans * ray.direction);

	bool intersected = false;
	for (auto child : children) {
		Intersection j;
		if (child->intersect(ray, j)) {
			if((!i.intersected) || glm::length(j.point-ray.origin) < glm::length(i.point-ray.origin)) i = j;
			intersected = true;
		}
	}

	// mcs to wcs
	// if (intersected){
	// 	i.point = trans * i.point;
	// 	i.normal = glm::transpose(invtrans) * i.normal;
	// }
	return intersected;
}

void SceneNode::flatten()
{
  std::list<SceneNode*> flattened_children;

  for(auto child : children)
  {
    child->set_transform(trans * child->trans);
    child->flatten();
    
    for(auto c_child : child->children)
    {
      flattened_children.push_back(c_child);
    }

    child->children.clear();
    flattened_children.push_back(child);
  }

  children.swap(flattened_children);
}



void SceneNode::radiosity(const std::list<Light*>& lights, std::vector<std::vector<double>> formfactor){
	std::map<int, glm::vec3> light_sources;
	for (auto light : lights){

		if (!light->is_point()){
			for (SceneNode* child : children){
				GeometryNode* geometry = dynamic_cast<GeometryNode*>(child);
				Primitive* prim = geometry->m_primitive;
				
				
				// if light source is on the same plane of the patch
				glm::vec3 local_light_pos = geometry->invtrans * light->get_pos();

				if (fabs(glm::dot(local_light_pos - prim->get_pos(), glm::vec3(0.0f, 1.0f, 0.0f))) < 0.01 && (1.0 - glm::dot(glm::normalize(geometry->invtrans * ((AreaLight*) light)->normal), glm::vec3(0.0f,1.0f,0.0f))) < 0.01){
					// if patch is in light source

					if (fabs(local_light_pos.x - prim->get_pos().x) < light->get_size() / 2 && fabs(local_light_pos.z - prim->get_pos().z) < light->get_size() / 2){
						if (m_name == "Scene1"){
							light_sources[geometry->id] = glm::vec3(50.0f, 50.0f, 50.0f);
						}else if (m_name == "Scene2"){
							light_sources[geometry->id] = glm::vec3(10.0f, 10.0f, 10.0f);
						}
					}
				}

			}
		}
	}
	
	// std::cout << "Light sources: " << std::endl;
	// for (auto & pair : light_sources){
	// 	std::cout << pair.first << ":" << pair.second << std::endl;
	// }

	// Let Bi represent the radiance of the light from patch i
	// Bi = Ei + pri\sum{1--n}Bj Fij
	// Bi - Pri\sum{1--n}Bj Fij = Ei
	// can construct matrix to solve for Bi
	// pri is reflectivity, which is assumed to be 0.5 here for all patches
	int patch_number = formfactor.size();
	double pri = 0.25;
	Eigen::MatrixXd A(patch_number, patch_number);
	// rgb
	Eigen::VectorXd E_r(patch_number);
	Eigen::VectorXd E_g(patch_number);
	Eigen::VectorXd E_b(patch_number);
	for (int i = 0; i < patch_number; i++) {
		for (int j = 0; j < patch_number; j++) {
			if (i == j){
				A(i, j) = 1.0 - pri * formfactor[i][j];
			}else {
				A(i, j) = -pri * formfactor[i][j];
			}
		}
	}
	// Light sources are assumed to be 1
	for (int index = 0; index < patch_number; index++){
		// if patch index is a light source
		if (light_sources.find(index) != light_sources.end()){
			glm::vec3 Emmision = light_sources[index];
			E_r(index) = Emmision.r;
			E_g(index) = Emmision.g;
			E_b(index) = Emmision.b;
		} else {
			E_r(index) = 0.0;
			E_g(index) = 0.0;
			E_b(index) = 0.0;
		}
	}

	
	Eigen::VectorXd B_r = A.fullPivLu().solve(E_r);
	Eigen::VectorXd B_g = A.fullPivLu().solve(E_g);
	Eigen::VectorXd B_b = A.fullPivLu().solve(E_b);

	std::vector<glm::vec3> B_colour (patch_number);
	for (size_t i = 0; i < patch_number; i++){
		glm::vec3 colour (B_r[i], B_g[i], B_b[i]);
		B_colour[i] = colour;
	}
	for (auto child : children){
		GeometryNode* geometry = (GeometryNode*) child;
		geometry->set_colour(B_colour[geometry->id]);
	}

}

void SceneNode::radiosity_hemicube(const std::list<Light*>& lights){
	std::vector<std::vector<double>> formfactor = formfactor_hemicube();
	radiosity(lights, formfactor);	
	return;
}

void SceneNode::radiosity_montecarlo(const std::list<Light*>& lights){
	std::vector<std::vector<double>> formfactor = formfactor_montecarlo();
	radiosity(lights, formfactor);	
	return;
}

std::vector<std::vector<double>> SceneNode::formfactor_hemicube(){
	size_t patch_number = children.size();
	std::vector<std::vector<double>> Formfactor (patch_number);
	int i = 0;
	for (SceneNode* child : children){
		GeometryNode* geometry = dynamic_cast<GeometryNode*>(child);
		geometry->id = i;
		i ++;
	}

	std::string scene_name = "FormFactors/" + m_name + "_" +  std::to_string(patch_number) + "_hemicube.csv";
	std::ifstream file(scene_name);
	// if file exists
	if (file.good()){
		return readVectorFromFile(scene_name);
	}

	std::cout << "Hemicube Start Timing..." << std::endl;
	auto startTime = std::chrono::high_resolution_clock::now();
	for (SceneNode* child : children){
		GeometryNode* geometry = dynamic_cast<GeometryNode*>(child);
		Formfactor[geometry->id] = geometry->formfactor_hemicube(this, patch_number);
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
	std::cout << "FormFactor cost: " << duration.count() << " microseconds" << std::endl;
	saveVectorToFile(Formfactor, scene_name);
	return Formfactor;
}

std::vector<std::vector<double>> SceneNode::formfactor_montecarlo(){
	size_t patch_number = children.size();
	std::vector<std::vector<double>> Formfactor (patch_number);
	int i = 0;
	for (SceneNode* child : children){
		GeometryNode* geometry = dynamic_cast<GeometryNode*>(child);
		geometry->id = i;
		i ++;
	}

	std::string scene_name = "FormFactors/" + m_name + "_" +  std::to_string(patch_number) + "_montecarlo.csv";
	std::ifstream file(scene_name);
	//if file exists
	if (file.good()){
		return readVectorFromFile(scene_name);
	}

	std::cout << "MonteCarlo Start Timing..." << std::endl;
	auto startTime = std::chrono::high_resolution_clock::now();

	for (SceneNode* child : children){
		GeometryNode* geometry = dynamic_cast<GeometryNode*>(child);
		Formfactor[geometry->id] = geometry->formfactor_montecarlo(this, patch_number);
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

	std::cout << "FormFactor cost: " << duration.count() << " microseconds" << std::endl;

	saveVectorToFile(Formfactor, scene_name);
	return Formfactor;
}

