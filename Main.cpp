// Termm--Winter 2021

#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <vector>
#include <map>
#include "Renderer.hpp"
#include "SceneNode.hpp"
#include "Scene.hpp"

int main(int argc, char** argv)
{
  // SceneNode* root = scene1();
  int hemi = 0;
  int patch_number = 4;
  int scene_number = 1;
  int light_number = 1;

  switch (argc) {
    case 2:
      hemi = std::stoi(argv[1]);
      break;
    case 3:
      hemi = std::stoi(argv[1]);
      patch_number = std::stoi(argv[2]);
      break;
    case 4:
      hemi = std::stoi(argv[1]);
      patch_number = std::stoi(argv[2]);
      scene_number = std::stoi(argv[3]);
      break;
    case 5:
      hemi = std::stoi(argv[1]);
      patch_number = std::stoi(argv[2]);
      scene_number = std::stoi(argv[3]);
      light_number = std::stoi(argv[4]);
      break;
  }


  bool use_hemicube = (hemi == 0)? false : true;
  
  SceneNode* root = nullptr;
  std::list<Light*> lights;
  
  if (scene_number == 1){
    root = scene1(patch_number);
  }else if (scene_number == 2){
    root = scene2(patch_number);
  }

  switch (light_number)
  {
  case 1:
    lights = scene1_lights();
    break;
  case 2:
    lights = scene2_lights();
    break;
  case 3:
    lights = scene3_lights();
    break;
  }

  Image img (256,256);

  Renderer(root, img, glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 80.0, lights, hemi);

  delete root;
  for (auto light : lights){
    delete light;
  }
  return 0;
}
