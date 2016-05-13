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

#include "console.h"

Console::Console() {
    this->square_mesh.load_square_mesh();
    this->square_mesh.static_load();

    this->shader = new Shader("assets/shaders/console");
    this->shader->add_attribute(ShaderAttribute::POSITION, "position");
    this->shader->add_uniform(ShaderUniform::VEC4, "back_color", 1);
    this->shader->bind_uniforms_and_attributes();
    this->shader->set_uniform(0, &glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)[0]);
}

void Console::draw() {
    this->square_mesh.bind();
    shader->link_shader();
    this->square_mesh.draw();
    this->square_mesh.unbind();
}
