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

#ifndef _OBJECT_H
#define _OBJECT_H

#include <vector>
#include <iostream>

#include "shader.h"
#include "mesh.h"


/**
 * @brief      a property that will be parsed to the shader as a uniform
 */
class ObjectProperty {
public:
    ObjectProperty(const std::string& _name, unsigned int size);

    void set_value(const float* val);

    inline const float* get_value() const {
        return &this->val[0];
    }

    inline const std::string& get_name() const {
        return this->name;
    }

    inline unsigned int get_size() const {
        return this->val_size;
    }

private:
    std::vector<float> val;
    unsigned int val_size;
    std::string name;
};

class Object {
public:
    Object() {}
    Object(Shader* shader, const Mesh* _mesh);

    void draw();

    void load();

    unsigned int add_property(const std::string& _name, unsigned int size);

    void set_property_value(unsigned int prop_id, const float* val);

    inline void set_position(glm::vec3 _position) {
        this->position = _position;
    }

    inline void set_scale_matrix(const glm::mat4& _scale) {
        this->scale = _scale;
    }

    inline void set_rotation_matrix(const glm::mat4& _rotation) {
        this->rotation = _rotation;
    }

    inline const glm::vec3& get_position() const {
        return this->position;
    }

private:
    Shader* shader;
    const Mesh*   mesh;

    std::vector<ObjectProperty> properties;

    glm::mat4 scale;
    glm::mat4 rotation;
    glm::vec3 position;

    enum {
        POSITION_VB,
        NORMAL_VB,
        COLOR_VB,
        INDICES_VB,

        NUM_BUFFERS
    };

    GLuint m_vertex_array_object;
    GLuint m_vertex_array_buffers[NUM_BUFFERS];
};


#endif //_OBJECT_H
