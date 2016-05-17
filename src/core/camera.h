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

#ifndef _CAMERA_H
#define _CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "screen.h"

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

    inline float get_distance() const {
        return this->distance;
    }

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

    /**
     * @brief      pan the camera to the left
     */
    void pan_left();

    /**
     * @brief      pan the camera to the right
     */
    void pan_right();

    /**
     * @brief      pan the camera up
     */
    void pan_up();

    /**
     * @brief      pan the camera down
     */
    void pan_down();

    /**
     * @brief      rotate the camera in the clock-wise direction
     */
    inline void angle_cw() {
        this->theta -= 0.1f;
        if(this->theta < 2.0f * M_PI) {
            this->theta += 2.0f * M_PI;
        }
    }

    /**
     * @brief      reset the rotation angle
     */
    inline void reset_angle() {
        this->theta = 0.0f;
    }

    /**
     * @brief      rotate the camera in the counter-clock-wise direction
     */
    inline void angle_ccw() {
        this->theta += 0.1f;
        if(this->theta > 2.0f * M_PI) {
            this->theta -= 2.0f * M_PI;
        }
    }

    /**
     * @brief      zoom out
     */
    inline void zoom_out() {
        this->distance += 1.0;

        if(distance > 150.0) {
            distance = 150.0;
        }
    }

    /**
     * @brief      zoom in
     */
    inline void zoom_in() {
        this->distance -= 1.0;

        if(distance < 50.0) {
            distance = 50.0;
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
    float theta;            //!< rotation angle
    float phi;            //!< rotation angle

    glm::vec3 position;     //!< position of the camera
    glm::vec3 look_at;      //!< looking position

    float m_aspect;         //!< aspect ratio of the window

    /**
     * @brief       calculate the position of the camera from the angle and orientation
     *
     * @return      void
     */
    void calculate_position();

    Camera(Camera const&)          = delete;
    void operator=(Camera const&)  = delete;
};

#endif // _CAMERA_H
