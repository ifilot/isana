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

class Object {
public:
    Object() {}
    Object(Shader* shader, const Mesh* _mesh);

    virtual void draw() = 0;
    virtual void static_load() = 0;

    void parse_uniform(unsigned int id, const float* val);

    inline void set_position(glm::vec3 _position) {
        this->position = _position;
    }

    inline void set_scale_matrix(const glm::mat4& _scale) {
        this->scale = _scale;
    }

    inline const glm::vec3& get_position() const {
        return this->position;
    }

protected:
    Shader* shader;
    const Mesh*   mesh;

    glm::mat4 model;
    glm::mat4 scale;
    glm::vec3 position;
};

class ObjectTexturedMesh : public Object {
public:
    ObjectTexturedMesh(Shader* shader, const Mesh* _mesh);

    void draw();
    void static_load();
private:

    enum {
        POSITION_VB,
        NORMAL_VB,
        INDICES_VB,
        TEXTURE_COORDINATE_VB,

        NUM_BUFFERS
    };

    GLuint m_vertex_array_object;
    GLuint m_vertex_array_buffers[NUM_BUFFERS];
};

class ObjectLines : public Object {
public:
    ObjectLines(Shader* shader, const Mesh* _mesh);

    void draw();
    void static_load();
private:

    enum {
        POSITION_VB,
        INDICES_VB,

        NUM_BUFFERS
    };

    GLuint m_vertex_array_object;
    GLuint m_vertex_array_buffers[NUM_BUFFERS];
};

class ObjectMesh : public Object {
public:
    ObjectMesh(Shader* shader, const Mesh* _mesh);

    void draw();
    void static_load();
private:

    enum {
        POSITION_VB,
        NORMAL_VB,
        INDICES_VB,

        NUM_BUFFERS
    };

    GLuint m_vertex_array_object;
    GLuint m_vertex_array_buffers[NUM_BUFFERS];
};

#endif //_OBJECT_H
