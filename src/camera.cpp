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
    this->view = glm::lookAt(
                    this->position,              // cam pos
                    glm::vec3(0.0, 0.0, 0.0),    // look at
                    glm::vec3(0.0f, 0.0f ,1.0f)  // up
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
 * @brief       get the camera position
 *
 * @return      camera position
 */
void Camera::set_aspect_ratio(const float& aspect_ratio) {
    this->m_aspect = aspect_ratio;
}

/**
 * @brief       set the camera position
 *
 * @return      void
 */
void Camera::set_position(const glm::vec3 _position) {
    this->position = _position;
}

/**
 * @brief       set the aspect ratio of the screen (changes projection matrix)
 *
 * @return      void
 */
const glm::vec3& Camera::get_position() const {
    return this->position;
}

/**
 * @brief       camera constructor
 *
 * @return      camera instance
 */
Camera::Camera() {
    this->position = glm::vec3(0.0, 150.0, 150.0);
    this->m_aspect = 1.0f;
    this->view = glm::mat4(1.0f);
    this->projection = glm::mat4(1.0f);
}
