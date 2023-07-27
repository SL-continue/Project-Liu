#pragma once

#include "GeometryNode.hpp"
#include "PhongMaterial.hpp"
#include "SceneNode.hpp"

#include <list>

SceneNode* scene1(int patch_number);

std::list<Light*> scene1_lights();

SceneNode* scene2(int patch_number);

std::list<Light*> scene2_lights();

std::list<Light*> scene3_lights();