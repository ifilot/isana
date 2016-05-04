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

#include "objects_engine.h"

ObjectsEngine::ObjectsEngine() {
    this->add_shader("assets/shaders/tree");
    this->add_mesh("assets/meshes/tree.mesh");

    this->objects.push_back(new ObjectMesh(this->shaders.back(), this->meshes.back()));
    this->objects.push_back(new ObjectMesh(this->shaders.back(), this->meshes.back()));
    this->objects.back()->set_position(glm::vec3(25, 25, 2));
}

void ObjectsEngine::update() {

}

void ObjectsEngine::draw() {
    for(unsigned int i=0; i<this->objects.size(); i++) {
        this->objects[i]->draw();
    }
}

unsigned int ObjectsEngine::add_shader(const std::string& filename) {
    this->shaders.push_back(new Shader(filename));

    this->shaders.back()->add_uniform(ShaderUniform::MAT4, "model");
    this->shaders.back()->add_uniform(ShaderUniform::MAT4, "view");
    this->shaders.back()->add_uniform(ShaderUniform::MAT4, "mvp");

    this->shaders.back()->add_attribute(ShaderAttribute::POSITION, "position");
    this->shaders.back()->add_attribute(ShaderAttribute::NORMAL, "normal");

    this->shaders.back()->bind_uniforms_and_attributes();

    return this->shaders.size() - 1;
}

unsigned int ObjectsEngine::add_mesh(const std::string& filename) {
    this->meshes.push_back(new Mesh(filename));

    this->meshes.back()->static_load();

    return this->meshes.size() - 1;
}
