/**************************************************************************
#                                                                         #
#   This file is part of ISANA                                             #
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

#ifndef _CAMERA_H
#define _CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

/**
 * @class Camera class
 *
 * @brief class handling the camera
 *
 */
class Camera {
public:

    /**
     * @fn          get
     *
     * @brief       get a reference to the camera
     *
     * @return      reference to the camera object (singleton pattern)
     */
    static Camera& get() {
        static Camera camera_instance;
        return camera_instance;
    }

    /**
     * @brief       update the camera perspective matrix
     *
     * @return      void
     */
    void update();

    /**
     * @brief       get the view matrix
     *
     * @return      view matrix
     */
    glm::mat4 get_view();

    /**
     * @brief       get the projection matrix
     *
     * @return      projection matrix
     */
    const glm::mat4& get_projection() const;

    /**
     * @brief       get the camera position
     *
     * @return      camera position
     */
    const glm::vec3& get_position() const;

    /**
     * @brief       set the camera position
     *
     * @return      void
     */
    void set_position(const glm::vec3 _position);

    /**
     * @brief       set the aspect ratio of the screen (changes projection matrix)
     *
     * @return      void
     */
    void set_aspect_ratio(const float& aspect_ratio);

    inline void pan_left() {
        this->look_at -= glm::vec3(-1, 0, 0);
    }

    inline void pan_right() {
        this->look_at -= glm::vec3(1, 0, 0);
    }

    inline void pan_up() {
        this->look_at -= glm::vec3(0, 1, 0);
    }

    inline void pan_down() {
        this->look_at -= glm::vec3(0, -1, 0);
    }

    inline void angle_cw() {
        this->angle -= 0.1f;
        if(this->angle < 2.0f * M_PI) {
            this->angle += 2.0f * M_PI;
        }
    }

    inline void angle_ccw() {
        this->angle += 0.1f;
        if(this->angle > 2.0f * M_PI) {
            this->angle -= 2.0f * M_PI;
        }
    }

private:
    /**
     * @brief       camera constructor
     *
     * @return      camera instance
     */
    Camera();

    glm::mat4 projection;   //!< perspective matrix
    glm::mat4 view;         //!< view matrix

    float distance;         //!< distance of the camera with respect to looking position
    float angle;            //!< rotation angle

    glm::vec3 position;     //!< position of the camera
    glm::vec3 look_at;      //!< looking position

    float m_aspect;         //!< aspect ratio of the window

    void calculate_position();

    Camera(Camera const&)          = delete;
    void operator=(Camera const&)  = delete;
};

#endif // _CAMERA_H
