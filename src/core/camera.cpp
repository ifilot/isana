/**************************************************************************
#                                                                         #
#   This file is part of ISANA                                            #
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation, version 3                               #
#                                                                         #
#   This program is distributed in the hope that it will be useful, but   #
#   WITHOUT ANY WARRANTY; without even the implied warranty of            #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     #
#   General Public License for more details.                              #
#                                                                         #
#   You should have received a copy of the GNU General Public License     #
#   along with this program; if not, write to the Free Software           #
#   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA             #
#   02110-1301, USA.                                                      #
#                                                                         #
#**************************************************************************/

#include "camera.h"

/**
 * @brief       update the camera perspective matrix
 *
 * @return      void
 */
void Camera::update() {
    this->projection = glm::perspective(glm::radians(45.0f), this->m_aspect, 0.1f, 300.0f);
}

/**
 * @brief       get the view matrix
 *
 * @return      view matrix
 */
glm::mat4 Camera::get_view() {
    this->calculate_position();

    this->view = glm::lookAt(
                    this->position,              // cam pos
                    this->look_at,               // look at
                    glm::vec3(0.0f, 0.0f, 1.0f)  // up
                );

    return view;
}

/**
 * @brief       get the projection matrix
 *
 * @return      projection matrix
 */
const glm::mat4& Camera::get_projection() const {
    return this->projection;
}

/**
 * @brief       set the camera aspect ratio
 *
 * @param       aspect_ratio    the aspect ratio
 *
 * @return      void
 */
void Camera::set_aspect_ratio(const float& aspect_ratio) {
    this->m_aspect = aspect_ratio;
}

/**
 * @brief      pan the camera to the left
 */
void Camera::pan_left() {
    static const glm::vec4 v(-5, 0, 0, 1);
    const glm::mat4 rot = glm::rotate(glm::mat4(1.0), this->theta, glm::vec3(0,0,1));
    this->look_at += glm::vec3(rot * v);
}

/**
 * @brief      pan the camera to the right
 */
void Camera::pan_right() {
    static const glm::vec4 v(5, 0, 0, 1);
    const glm::mat4 rot = glm::rotate(glm::mat4(1.0), this->theta, glm::vec3(0,0,1));
    this->look_at += glm::vec3(rot * v);
}

/**
 * @brief      pan the camera up
 */
void Camera::pan_up() {
    static const glm::vec4 v(0, -5, 0, 1);
    const glm::mat4 rot = glm::rotate(glm::mat4(1.0), this->theta, glm::vec3(0,0,1));
    this->look_at += glm::vec3(rot * v);
}

/**
 * @brief      pan the camera down
 */
void Camera::pan_down() {
    static const glm::vec4 v(0, 5, 0, 1);
    const glm::mat4 rot = glm::rotate(glm::mat4(1.0), this->theta, glm::vec3(0,0,1));
    this->look_at += glm::vec3(rot * v);
}

/**
 * @brief       set the camera position
 *
 * @param       position    the position
 *
 * @return      void
 */
void Camera::set_position(const glm::vec3 _position) {
    this->position = _position;
}

/**
 * @brief       get the camera position
 *
 * @return      camera position
 */
const glm::vec3& Camera::get_position() const {
    return this->position;
}

/**
 * @brief       calculate the position of the camera from the theta and orientation
 *
 * @return      void
 */
void Camera::calculate_position() {

    if(this->distance > 150.0f) {
        this->phi = 0.01f;
    } else if(this->distance < 50.0f) {
        this->phi = glm::radians(60.0f);
    } else {
        this->phi = glm::radians(60.0f - (this->distance - 55.0f) / 100.0f * 60.0f);
    }

    glm::vec3 cam_vec(std::sin(this->theta) * std::sin(this->phi),
                      -std::cos(this->theta) * std::sin(this->phi),
                      std::cos(this->phi)
                      );
    cam_vec *= this->distance;
    this->position = this->look_at + cam_vec;
}

/**
 * @brief       camera constructor
 *
 * @return      camera instance
 */
Camera::Camera() {
    this->look_at = glm::vec3(50.0f, 50.0f, 0.0f);
    this->theta = 0.0f * 2.0f * M_PI; // orient to south
    this->phi = 0.1f * 2.0f * M_PI; // orient to south
    this->distance = 50.0f;

    this->calculate_position();

    this->m_aspect = 1.0f;
    this->view = glm::mat4(1.0f);
    this->projection = glm::mat4(1.0f);
}
