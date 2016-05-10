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

#ifndef _ARMATURE_H
#define _ARMATURE_H

#include <iostream>
#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

/**
 * @brief      Bone class as nested class in Armature
 */
class Bone {
private:
    const Bone* parent;         //! < pointer to parent
    glm::mat4 matrix_offset;    //! < matrix to transform from model space to this bone space
    glm::mat4 matrix_frame;     //! < matrix to transform from this bone space to the parent's bone
                                //! space
    std::string name;           //! < bone name

    std::vector<float> weights; //! < vector holding weights

public:

    /**
     * @brief      default constructor
     *
     * @param[in]  offset  offset matrix to transform from model space to
     *                     this bone space
     * @param[in]  frame   frame matrix to transform from this frame to the
     *                     parent's frame
     * @param[in]  parent  pointer to the parent Bone (NULL if this is root)
     */
    Bone(const glm::mat4& frame, const std::string& _name, const Bone* _parent);

    inline const std::string& get_name() const {
        return this->name;
    }

    inline void set_offset_matrix(const glm::mat4& _offset_matrix) {
        this->matrix_offset = _offset_matrix;
    }

    inline void set_weights(const std::vector<float>& _weights) {
        this->weights = _weights;
    }

    inline const std::vector<float>& get_weights() {
        return this->weights;
    }

    inline unsigned int get_weights_size() const {
        return this->weights.size();
    }

    inline const Bone* get_parent() const {
        return this->parent;
    }

private:
};

/**
 * @brief      Armature class that handles skeletal animation
 */
class Armature {

private:
    std::vector<Bone*> bones;        //!< vector holding all bones in armature

public:
    Armature();                     //!< default constructor

    Bone* add_bone(const glm::mat4& frame, const std::string& _name, const Bone* parent_id);

    void print_bone_list() const;

    std::string get_bone_path(const Bone*) const;

    inline unsigned int get_nr_bones() const {
        return this->bones.size();
    }

    inline Bone* get_bone_by_idx(unsigned int idx) const {
        return this->bones[idx];
    }

    std::vector<float> get_weights_vector() const;

private:

};

#endif //_ARMATURE_H
