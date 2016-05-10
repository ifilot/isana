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
    unsigned int prop_id = 0;

    TextureManager::get();

    // add custom shader
    this->add_shader("assets/shaders/coordinate_system");

    this->add_mesh("assets/meshes/sphere.mesh");
    this->objects.push_back(new Object(this->shaders.back(), this->meshes.back()));
    this->objects.back()->set_scale_matrix(glm::scale(glm::vec3(1.0) * 0.5f));
    prop_id = this->objects.back()->add_property("object_color", ShaderUniform::VEC3, 1);
    this->objects.back()->set_property_value(prop_id, &glm::vec3(0,0,0)[0]);
    this->objects.back()->load();

    this->add_mesh("assets/meshes/arrow.mesh");

    // z-coordinate
    this->objects.push_back(new Object(this->shaders.back(), this->meshes.back()));
    this->objects.back()->set_scale_matrix(glm::scale(glm::vec3(1.0) * 3.0f));
    prop_id = this->objects.back()->add_property("object_color", ShaderUniform::VEC3, 1);
    this->objects.back()->set_property_value(prop_id, glm::value_ptr(glm::vec3(0,0,1)));
    this->objects.back()->load();

    // y-coordinate
    this->objects.push_back(new Object(this->shaders.back(), this->meshes.back()));
    this->objects.back()->set_scale_matrix(glm::scale(glm::vec3(1.0) * 3.0f));
    this->objects.back()->set_rotation_matrix(glm::rotate(-(float)M_PI / 2.0f, glm::vec3(1.0f,0.0f,0.0f)));
    prop_id = this->objects.back()->add_property("object_color", ShaderUniform::VEC3, 1);
    this->objects.back()->set_property_value(prop_id, glm::value_ptr(glm::vec3(0,1,0)));
    this->objects.back()->load();

    // x-coordinate
    this->objects.push_back(new Object(this->shaders.back(), this->meshes.back()));
    this->objects.back()->set_scale_matrix(glm::scale(glm::vec3(1.0) * 3.0f));
    this->objects.back()->set_rotation_matrix(glm::rotate((float)M_PI / 2.0f, glm::vec3(0.0f,1.0f,0.0f)));
    prop_id = this->objects.back()->add_property("object_color", ShaderUniform::VEC3, 1);
    this->objects.back()->set_property_value(prop_id, glm::value_ptr(glm::vec3(1,0,0)));
    this->objects.back()->load();

    this->add_shader("assets/shaders/tree");
    this->add_mesh("assets/meshes/tree.mesh");

    this->objects.push_back(new Object(this->shaders.back(), this->meshes.back()));
    this->objects.back()->set_position(glm::vec3(25, 25, 2));
    this->objects.back()->load();

    this->add_shader("assets/shaders/turbine");
    this->add_mesh("assets/meshes/turbine.x");

    // setup 3x3 array of wind turbines

    for(unsigned int i=0; i<3; i++) {
        for(unsigned int j=0; j<3; j++) {

            float x = (float)(5 + i * 5);
            float y = (float)(28 + j * 5);

            float z = Terrain::get().get_height(x,y);

            this->objects.push_back(new Object(this->shaders.back(), this->meshes.back()));
            this->objects.back()->set_position(glm::vec3(x, y, z));
            this->objects.back()->load();
       }
    }
}

void ObjectsEngine::update(double dt) {
    for(unsigned int i=0; i<this->objects.size(); i++) {
        this->objects[i]->update(dt * (double)i / 10.0);
    }
}

void ObjectsEngine::draw() {
    for(unsigned int i=0; i<this->objects.size(); i++) {
        this->objects[i]->draw();
    }
}

unsigned int ObjectsEngine::add_shader(const std::string& filename) {
    this->shaders.push_back(new Shader(filename));

    return this->shaders.size() - 1;
}

unsigned int ObjectsEngine::add_mesh(const std::string& filename) {
    this->meshes.push_back(new Mesh(filename));

    std::cout << filename << std::endl;

    this->meshes.back()->static_load();

    return this->meshes.size() - 1;
}
