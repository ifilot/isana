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

Bone::Bone(const glm::mat4& frame, const std::string& _name, const Bone* _parent) {
    this->matrix_frame = frame;
    this->name = _name;
    this->parent = _parent;
}

Armature::Armature() {

}

std::string Armature::get_bone_path(const Bone* bone) const {
    if(bone->get_parent() == NULL) {
        return bone->get_name();
    }
    return this->get_bone_path(bone->get_parent()) + " > " + bone->get_name();
}

Bone* Armature::add_bone(const glm::mat4& frame, const std::string& _name, const Bone* _parent) {
    this->bones.push_back(new Bone(frame, _name, _parent));

    return this->bones.back();
}


void Armature::print_bone_list() const {
    for(unsigned int i=0; i<this->bones.size(); i++) {
        std::cout << get_bone_path(bones[i]) << std::endl;
    }
}
