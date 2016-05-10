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

#include "armature.h"

/**
 * @brief      default constructor
 *
 * @param[in]  frame    frame matrix to transform from this frame to the
 *                      parent's frame
 * @param[in]  _name    name of the bone
 * @param[in]  _parent  pointer to the parent Bone (NULL if this is root)
 *
 */
Bone::Bone(const glm::mat4& frame, const std::string& _name, const Bone* _parent) {
    this->matrix_frame = frame;
    this->name = _name;
    this->parent = _parent;
}

/**
 * @brief      Armature constructor
 */
Armature::Armature() {

}

/**
 * @brief      add a bone to the armature
 *
 * @param[in]  frame      matrix defining the frame transformation
 * @param[in]  _name      name of the bone
 * @param[in]  parent_id  pointer to the parent bone (NULL is none)
 *
 * @return     pointer to the bone instance
 */
Bone* Armature::add_bone(const glm::mat4& frame, const std::string& _name, const Bone* _parent) {
    this->bones.push_back(new Bone(frame, _name, _parent));

    return this->bones.back();
}

/**
 * @brief      Get the weights vector.
 *
 * @return     The weights vector.
 */
std::vector<float> Armature::get_weights_vector() const {
    const unsigned int nr_bones = this->get_nr_bones();
    const unsigned int nr_vertices = this->bones[0]->get_weights_size();

    std::vector<float> weights(nr_bones * nr_vertices, 0.0f);

    for(unsigned int i=0; i<nr_vertices; i++) {
        for(unsigned int j=0; j<nr_bones; j++) {
            weights[i * nr_bones + j] = this->bones[j]->get_weights()[i];
        }
    }

    return weights;
}

/**
 * @brief      print list of bones
 */
void Armature::print_bone_list() const {
    for(unsigned int i=0; i<this->bones.size(); i++) {
        std::cout << get_bone_path(bones[i]) << std::endl;
    }
}

/**
 * @brief      get the path of a bone
 *
 * @param[in]  bone  pointer to bone instance
 *
 * @return     string containin bone path.
 */
std::string Armature::get_bone_path(const Bone* bone) const {
    if(bone->get_parent() == NULL) {
        return bone->get_name();
    }
    return this->get_bone_path(bone->get_parent()) + " > " + bone->get_name();
}
