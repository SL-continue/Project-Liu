#include "Scene.hpp"

SceneNode* scene1(int patch_number){
    SceneNode *root = new SceneNode("Scene1");
    

    std::vector<GeometryNode*> nodes (6);
    nodes[0] = new GeometryNode ("top", new Plane());
    nodes[1] = new GeometryNode ("bottom", new Plane());
    nodes[2] = new GeometryNode ("right", new Plane());
    nodes[3] = new GeometryNode ("left", new Plane());
    nodes[4] = new GeometryNode ("front", new Plane());
    nodes[5] = new GeometryNode ("back", new Plane());

    PhongMaterial* material = new PhongMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0);
    // PhongMaterial* material2 = new PhongMaterial(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0);
    // PhongMaterial* material3 = new PhongMaterial(glm::vec3(1.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0);

    nodes[0]->setMaterial(material);
    nodes[1]->setMaterial(material);
    nodes[2]->setMaterial(material);
    nodes[3]->setMaterial(material);
    nodes[4]->setMaterial(material);
    nodes[5]->setMaterial(material);

    for (auto node : nodes){
        root->add_child(node);
        node->m_primitive->enlarge(2.0);
        // node->setMaterial(material);
        // node->translate(glm::vec3( 0.0f,-2.0f, 0.0f));
    }


    nodes[0]->rotate('x', 180.0f);
    nodes[2]->rotate('z', 90.0f);
    nodes[3]->rotate('z', -90.0f);
    nodes[4]->rotate('x', -90.0f);
    nodes[5]->rotate('x', 90.0f);

    nodes[0]->translate(glm::vec3( 0.0f, 1.0f, 0.0f));
    nodes[1]->translate(glm::vec3( 0.0f,-1.0f, 0.0f));
    nodes[2]->translate(glm::vec3( 1.0f, 0.0f, 0.0f));
    nodes[3]->translate(glm::vec3(-1.0f, 0.0f, 0.0f));
    nodes[4]->translate(glm::vec3( 0.0f, 0.0f, 1.0f));
    nodes[5]->translate(glm::vec3( 0.0f, 0.0f,-1.0f));

    // for (int i = 0; i < 6; i++){
    //     std::cout << glm::transpose(nodes[i]->invtrans) * glm::vec3(0.0f,1.0f,0.0f) << std::endl;
    //     std::cout << glm::transpose(nodes[i]->invtrans) *  << std::endl;
    // }
    for (auto node : nodes){
        node->divide_into_patches(patch_number);
    }
    return root;
}

std::list<Light*> scene1_lights(){
    std::list<Light*> lights;
    AreaLight * light1 = new AreaLight(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
    light1->set_normal(glm::vec3(0.0f, -1.0f, 0.0f));
    lights.push_back(light1);
    // std::cout << "Light " << light1->colour << std::endl;

    return lights;
}

std::list<Light*> scene2_lights(){
    std::list<Light*> lights;
    AreaLight * light1 = new AreaLight(glm::vec3(-1.0f, 0.0f, 0.0f), 1.0f);
    light1->set_normal(glm::vec3(1.0f, 0.0f, 0.0f));
    lights.push_back(light1);
    return lights;
}


std::list<Light*> scene3_lights(){
    std::list<Light*> lights;
    AreaLight * light1 = new AreaLight(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
    light1->set_normal(glm::vec3(0.0f, -1.0f, 0.0f));
    AreaLight * light2 = new AreaLight(glm::vec3(-1.0f, 0.0f, 0.0f), 1.0f);
    light2->set_normal(glm::vec3(1.0f, 0.0f, 0.0f));
    lights.push_back(light1);
    lights.push_back(light2);
    // std::cout << "Light " << light1->colour << std::endl;

    return lights;
}




SceneNode* scene2(int patch_number){
    SceneNode *root = scene1(patch_number);
    root->m_name = "Scene2";

    std::vector<GeometryNode*> nodes (6);
    nodes[0] = new GeometryNode ("cube_top", new Plane());
    nodes[1] = new GeometryNode ("cube_bottom", new Plane());
    nodes[2] = new GeometryNode ("cube_right", new Plane());
    nodes[3] = new GeometryNode ("cube_left", new Plane());
    nodes[4] = new GeometryNode ("cube_front", new Plane());
    nodes[5] = new GeometryNode ("cube_back", new Plane());

    PhongMaterial* material = new PhongMaterial(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0);
    // PhongMaterial* material2 = new PhongMaterial(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0);
    // PhongMaterial* material3 = new PhongMaterial(glm::vec3(1.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0);

    nodes[0]->setMaterial(material);
    nodes[1]->setMaterial(material);
    nodes[2]->setMaterial(material);
    nodes[3]->setMaterial(material);
    nodes[4]->setMaterial(material);
    nodes[5]->setMaterial(material);

    for (auto node : nodes){
        root->add_child(node);
        node->m_primitive->enlarge(0.5);
    }

    nodes[1]->rotate('x', 180.0f);
    nodes[2]->rotate('z', -90.0f);
    nodes[3]->rotate('z', 90.0f);
    nodes[4]->rotate('x', 90.0f);
    nodes[5]->rotate('x', -90.0f);

    nodes[0]->translate(glm::vec3( 0.0f, -0.5f,  0.0f));
    nodes[1]->translate(glm::vec3( 0.0f, -1.0f,  0.0f));
    nodes[2]->translate(glm::vec3( 0.25f,-0.75f, 0.0f));
    nodes[3]->translate(glm::vec3(-0.25f,-0.75f, 0.0f));
    nodes[4]->translate(glm::vec3( 0.0f, -0.75f, 0.25f));
    nodes[5]->translate(glm::vec3( 0.0f, -0.75f,-0.25f));

    for (auto node : nodes){
        node->divide_into_patches(patch_number);
    }
    return root;
}