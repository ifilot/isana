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

#include "core/armature.h"

class Mesh {
private:
    Armature* armature;                                 //!< pointer to armature class
    std::vector<glm::vec3> positions;                   //!< vector holding positions
    std::vector<glm::vec3> normals;                     //!< vector holding vertex normals
    std::vector<glm::vec4> colors;                      //!< vector holding colors
    std::vector<glm::vec2> texture_coordinates;         //!< vector holding texture coordinates
    std::vector<unsigned int> indices;                  //!< vector holding set of indices

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

public:

    /**
     * @brief      Mesh constructor
     */
    Mesh();

    /**
     * @brief      Mesh constructor method using an input file
     *
     * @param[in]  filename  The filename
     */
    Mesh(const std::string& filename);

    /**
     * @brief      load the mesh on the GPU
     */
    void static_load();

    /**
     * @brief      load a default square mesh
     */
    void load_square_mesh();

    /**
     * @brief      draw the mesh
     */
    void draw() const;

    /**
     * @brief      bind the vertex attribute array
     */
    void bind() const;

    /**
     * @brief      unbind the vertex attribute array
     */
    void unbind() const;

    /**
     * @brief      get the number of indices
     *
     * @return     number of indices
     */
    inline unsigned int get_nr_indices() const {
        return this->indices.size();
    }

    /**
     * @brief      Get the number of positions
     *
     * @return     number of positions
     */
    inline unsigned int get_nr_positions() const {
        return this->positions.size();
    }

    /**
     * @brief      Get the number of normals
     *
     * @return     number of normals
     */
    inline unsigned int get_nr_normals() const {
        return this->normals.size();
    }

    /**
     * @brief      Get the number of colors
     *
     * @return     number of colors
     */
    inline unsigned int get_nr_colors() const {
        return this->colors.size();
    }

    /**
     * @brief      Get the number of texture coordinates
     *
     * @return     number of texture coordinates
     */
    inline unsigned int get_nr_texture_coordinates() const {
        return this->texture_coordinates.size();
    }

    /**
     * @brief      Set the indices.
     *
     * @param[in]  _indices  The indices
     */
    inline void set_indices(const std::vector<unsigned int>& _indices) {
        this->indices = _indices;
    }

    /**
     * @brief      Set the positions.
     *
     * @param[in]  _positions  The positions
     */
    inline void set_positions(const std::vector<glm::vec3>& _positions) {
        this->positions = _positions;
    }

    /**
     * @brief      Set the normals.
     *
     * @param[in]  _normals  The normals
     */
    inline void set_normals(const std::vector<glm::vec3>& _normals) {
        this->normals = _normals;
    }

    /**
     * @brief      Set the colors.
     *
     * @param[in]  _colors  The colors
     */
    inline void set_colors(const std::vector<glm::vec4>& _colors) {
        this->colors = _colors;
    }

    /**
     * @brief      get the start index of the indices
     *
     * @return     pointer to indices
     */
    inline const unsigned int* get_indices_start() const {
        return &this->indices[0];
    }

    /**
     * @brief      get the start index of the positions
     *
     * @return     pointer to positions
     */
    inline const glm::vec3* get_positions_start() const {
        return &this->positions[0];
    }

    /**
     * @brief      get the start index of the normals
     *
     * @return     pointer to normals
     */
    inline const glm::vec3* get_normals_start() const {
        return &this->normals[0];
    }

    /**
     * @brief      get the start index of the colors
     *
     * @return     pointer to colors
     */
    inline const glm::vec4* get_colors_start() const {
        return &this->colors[0];
    }

    /**
     * @brief      get the start index of the texture coordinates
     *
     * @return     pointer to texture coordinates
     */
    inline const glm::vec2* get_texture_coordinates_start() const {
        return &this->texture_coordinates[0];
    }

    /**
     * @brief      Get the mesh type.
     *
     * @return     mesh type
     */
    unsigned int get_type() const;

    static const unsigned int MESH_POSITIONS           = 1 << 0;    //!< has mesh positions
    static const unsigned int MESH_NORMALS             = 1 << 1;    //!< has mesh normals
    static const unsigned int MESH_COLORS              = 1 << 2;    //!< has mesh colors
    static const unsigned int MESH_TEXTURE_COORDINATES = 1 << 3;    //!< has mesh texture coordinates
    static const unsigned int MESH_ARMATURE            = 1 << 4;    //!< has mesh armature

    /**
     * @brief      Get the bone size.
     *
     * @return     Bone size.
     */
    unsigned int get_bone_size() const;

    /**
     * @brief      Get the armature.
     *
     * @return     pointer to armature instance
     */
    inline Armature* get_armature() const {
        return this->armature;
    }

    /**
     * @brief      center the vertex coordinates around the origin in model space
     */
    void center();

    ~Mesh();

private:
    /**
     * @brief      load Mesh from file
     *
     * @param[in]  filename  The filename
     */
    void load_mesh_from_file(const std::string& filename);

    /**
     * @brief      load Mesh from an .obj file
     *
     * @param[in]  filename  The filename
     */
    void load_mesh_from_obj_file(const std::string& filename);

    /**
     * @brief      load Mesh from an .x file
     *
     * @param[in]  filename  The filename
     */
    void load_mesh_from_x_file(const std::string& filename);

    /**
     * @brief      read frame transform matrix from ifstream
     *
     * @param      f     ifstream pointer
     *
     * @return     matrix
     */
    glm::mat4 read_frame_transform_matrix(std::ifstream* f);

    /**
     * @brief      read matrix from ifstream
     *
     * @param      f     ifstream pointer
     *
     * @return     matrix
     */
    glm::mat4 read_matrix(std::ifstream* f);
};


#endif //_MESH_H
