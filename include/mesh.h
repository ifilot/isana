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

#ifndef _MESH_H
#define _MESH_H

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>

#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "armature.h"

class Mesh {
private:
    Armature* armature;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec2> texture_coordinates;
    std::vector<unsigned int> indices;

public:
    Mesh();
    Mesh(const std::string& filename);
    void center();

    void set_indices(const std::vector<unsigned int>& _indices);
    void set_positions(const std::vector<glm::vec3>& _positions);
    void set_normals(const std::vector<glm::vec3>& _normals);
    void set_colors(const std::vector<glm::vec3>& _normals);

    unsigned int get_nr_indices() const;
    unsigned int get_nr_positions() const;
    unsigned int get_nr_normals() const;
    unsigned int get_nr_colors() const;
    unsigned int get_nr_texture_coordinates() const;

    const glm::vec3* get_positions_start() const;
    const glm::vec3* get_normals_start() const;
    const glm::vec3* get_colors_start() const;
    const glm::vec2* get_texture_coordinates_start() const;

    const unsigned int* get_indices_start() const;

    void static_load();
    void draw() const;
    void bind() const;
    void unbind() const;

    unsigned int get_type() const;

    unsigned int get_bone_size() const;

    static const unsigned int MESH_POSITIONS           = 1 << 0;
    static const unsigned int MESH_NORMALS             = 1 << 1;
    static const unsigned int MESH_COLORS              = 1 << 2;
    static const unsigned int MESH_TEXTURE_COORDINATES = 1 << 3;
    static const unsigned int MESH_ARMATURE            = 1 << 4;

    inline const Armature* get_armature() const {
        return this->armature;
    }

private:
    void load_mesh_from_file(const std::string& filename);

    void load_mesh_from_obj_file(const std::string& filename);

    void load_mesh_from_x_file(const std::string& filename);

    glm::mat4 read_frame_transform_matrix(std::ifstream* f);

    glm::mat4 read_matrix(std::ifstream* f);

    enum {
        POSITION_VB,
        NORMAL_VB,
        COLOR_VB,
        TEXTURE_VB,
        WEIGHTS_VB,
        INDICES_VB,

        NUM_BUFFERS
    };

    GLuint m_vertex_array_object;
    GLuint m_vertex_array_buffers[NUM_BUFFERS];
};


#endif //_MESH_H
