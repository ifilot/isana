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
 * @brief       calculate the position of the camera from the angle and orientation
 *
 * @return      void
 */
void Camera::calculate_position() {
    glm::vec3 cam_vec(std::sin(this->angle),
                      -std::cos(this->angle),
                      std::sin(60.0f * 2.0f * M_PI / 360.0f));
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
    this->angle = 0.0f * 2.0f * M_PI; // orient to south
    this->distance = 50.0f;

    this->calculate_position();

    this->m_aspect = 1.0f;
    this->view = glm::mat4(1.0f);
    this->projection = glm::mat4(1.0f);
}
