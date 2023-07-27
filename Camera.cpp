#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/io.hpp>

#include "cs488-framework/MathUtils.hpp"


Camera::Camera(glm::vec3 eye, glm::vec3 view, glm::vec3 up, double fovy, int width, int height) : m_lookfrom(eye), m_direction(view), m_up(up), m_fovy(fovy), m_width(width), m_height(height) {

    double focal_length = 1.0;
    double aspect_ratio = m_width / m_height;
    double w_height = 2.0 * focal_length * glm::tan(degreesToRadians(fovy/2));
    double w_width = aspect_ratio * w_height;
    
    // get world coordinates of pixels
    glm::vec3 view_z = glm::normalize(m_direction);
    glm::vec3 view_x = glm::normalize(glm::cross(m_up, view_z));
    glm::vec3 view_y = glm::normalize(glm::cross(view_z, view_x));
    
    glm::mat4 trans (1.0f);
    trans = glm::translate(trans, glm::vec3(- (double)m_height / 2, -(double)m_width / 2, focal_length));
    
    trans = glm::scale(glm::vec3(-w_width / m_width, -w_height / m_height, 1.0)) * trans;
    
    
    glm::mat4 view_to_world = glm::mat4(glm::vec4(view_x, 0.0f),
                                        glm::vec4(view_y, 0.0f),
                                        glm::vec4(view_z, 0.0f),
                                        glm::vec4(m_lookfrom, 1.0f));
                                        
    m_screentoworld = view_to_world * trans;

}

Ray Camera::castRay(glm::vec2 pixel_pos){
    glm::vec3 pixel = glm::vec3(m_screentoworld * glm::vec4(pixel_pos, 0.0f, 1.0f));
    // std::cout << pixel_pos << pixel << std::endl;
    return Ray(m_lookfrom, pixel);
}