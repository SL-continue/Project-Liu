#include <glm/ext.hpp>
#include "Renderer.hpp"
#include "cs488-framework/MathUtils.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

double epsilon = std::numeric_limits<double>::epsilon();

glm::vec3 light_ray(const Ray& ray, const Intersection& i, const Light* light, const glm::vec3& light_pos)
{
  PhongMaterial* material = dynamic_cast<PhongMaterial*>(i.m);
  glm::vec3 point = i.point;
  glm::vec3 normal = i.normal;
  glm::vec3 light_colour = light->colour;
    
  glm::vec3 point_to_light = light_pos - point;
  double distance_to_light = glm::length(point_to_light); 
  point_to_light = glm::normalize(point_to_light);

  double diffuse_brightness = std::max(0.0, (double) glm::dot(normal, point_to_light)); 

  glm::vec3 diffuse = diffuse_brightness * material->m_kd * light_colour;

  glm::vec3 point_to_eye = glm::normalize(ray.origin - point);

  glm::vec3 halfway = glm::normalize(point_to_light + point_to_eye);

  double specular_brightness = (diffuse_brightness > 0) ? pow(std::max((double)(0.0), (double) (glm::dot(normal, halfway))), material->m_shininess) : 0.0;

  glm::vec3 specular = specular_brightness * material->m_ks * light_colour;

  return light->attenuation(distance_to_light) * (diffuse + specular);
}


glm::vec3 shadow_ray(const Ray& ray, const SceneNode * root, const Light* light, const glm::vec3& light_pos, const glm::vec3& point, const Intersection& i)
{
  Ray shadow(point, light_pos);
  Intersection shadow_intersection;
  
  if(root->intersect(shadow, shadow_intersection) && glm::distance(shadow_intersection.point, shadow.origin) < glm::distance(light_pos, shadow.origin)) return glm::vec3(0.0, 0.0, 0.0);

  return light_ray(ray, i, light, light_pos);
}


glm::vec3 trace_ray(const Ray& ray, const SceneNode* root, const std::list<Light*>& lights){
    // black background
    glm::vec3 colour (0.0f, 0.0f, 0.0f);
    Intersection i;

    bool intersected = root->intersect(ray, i);
    
    if (intersected) {
        glm::vec3 n = i.normal;
        glm::vec3 p = i.point + epsilon * n;

        PhongMaterial* material = dynamic_cast<PhongMaterial*>(i.m);
        // ambient colour already calculated in radiosity call of root node
        // colour = (0.2) * material->m_kd + i.ambient * material->m_kd;
        colour = i.ambient * material->m_kd;

        if (colour != glm::vec3(0.0f, 0.0f, 0.0f)){
            for (auto light : lights){
                glm::vec3 shade_color = shadow_ray(ray, root, light, light->get_pos(), p, i);
                colour += shade_color;
            }
        }

        // no reflection considered here
    }
    colour.x = glm::min(1.0f, colour.x);
    colour.y = glm::min(1.0f, colour.y);
    colour.z = glm::min(1.0f, colour.z);
    return colour;

}


void render(Image& img, Camera& camera, SceneNode* root, std::list<Light*> lights){
    size_t h = img.height();
	size_t w = img.width();

    int num = w * h;
    int ss_samples = 1;
    int percent = 0;
    int increm = 0;

    double spacing = 1.0 / ss_samples;
    for (uint y = 0; y < h; y ++) {
        for (uint x = 0; x < w; x++) {
            increm ++;
            if (increm == num / 100){
                percent += 1;
                // std::cout << percent << "%" << std::endl;
                increm = 0;
            }
            glm::vec3 pixel_color(0.0f);

            for(uint p = 0; p < ss_samples; p++)
            {
                for(uint q = 0; q < ss_samples; q++)
                {
                glm::vec2 pixel_pos((double)x + p * spacing, (double)y + q * spacing);

                Ray ray = camera.castRay(pixel_pos);

                pixel_color = pixel_color + trace_ray(ray, root, lights);
                }
            }

            double n = ss_samples * ss_samples;
            pixel_color = pixel_color / n;
            img(x, y, 0) = pixel_color.r;
            img(x, y, 1) = pixel_color.g;
            img(x, y, 2) = pixel_color.b;
            // std::cout << pixel_color << std::endl;
        }
    }
}



void Renderer(
    	SceneNode * root,

		// Image to write to, set to a given width and height  
		Image & image,

		// Viewing parameters  
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters  
		// const glm::vec3 & ambient,
		const std::list<Light *> & lights,
        bool hemicube = false
){

    size_t h = image.height();
	size_t w = image.width();
	Camera camera(eye, view, up, fovy, w, h);

    glm::vec3 rot_axis = glm::vec3(1,0,0);
    glm::mat4 r1 = glm::rotate((float) degreesToRadians(90.0), rot_axis);
    
    glm::mat4 r2 = glm::rotate((float) degreesToRadians(-90.0), rot_axis);

    root->flatten();


    if (hemicube){
        root->radiosity_hemicube(lights);
    }else{
        root->radiosity_montecarlo(lights);
    }

    render(image, camera, root, lights);

    if (hemicube){
        image.savePng("Hemicube" + root->m_name + ".png");
    }else {
        image.savePng("MonteCarlo" + root->m_name + ".png");
    }
}