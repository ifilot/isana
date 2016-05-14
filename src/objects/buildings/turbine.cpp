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

#include "turbine.h"

void BuildingTurbine::update(double dt) {
    angle += dt;

    if(angle > 2.0 * M_PI) {
        angle -= 2.0 * M_PI;
    }

    glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0), (float)this->angle, glm::vec3(0,0,1));
    this->mesh->get_armature()->set_bone_transformation(2, rot_mat);
    this->mesh->get_armature()->build_glsl_matrices();
    this->properties[this->rig_idx].set_value(&this->mesh->get_armature()->get_glsl_matrices()[0][0][0]);
}
