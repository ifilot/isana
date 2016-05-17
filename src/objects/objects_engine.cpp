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

#include "objects/objects_engine.h"

ObjectsEngine::ObjectsEngine() {
    Console::get() << std::string(__FILE__) << ": Starting ObjectEngine class" << Console::endl;

    const unsigned int hq_tex_id = TextureManager::get().load_texture("assets/png/hq.png");
    this->add_shader("assets/shaders/turbine");
    this->add_mesh("assets/meshes/hq.x");

    this->objects.push_back(new BuildingHeadQuarters(this->shaders.back(), this->meshes.back(), hq_tex_id));
    this->objects.back()->set_position(glm::vec3(40, 50, Terrain::get().get_height(40,50)));
    this->objects.back()->load();

    const unsigned int turbine_tex_id = TextureManager::get().load_texture("assets/png/turbine.png");
    this->add_shader("assets/shaders/turbine");
    this->add_mesh("assets/meshes/turbine.x");

    // setup 3x3 array of wind turbines

    for(unsigned int i=0; i<10; i++) {
        for(unsigned int j=0; j<10; j++) {

            float x = (float)(50 + i * 5);
            float y = (float)(50 + j * 5);

            float z = Terrain::get().get_height(x,y);

            this->objects.push_back(new BuildingTurbine(this->shaders.back(), this->meshes.back(), turbine_tex_id));
            this->objects.back()->set_position(glm::vec3(x, y, z));
            this->objects.back()->load();
       }
    }
}

void ObjectsEngine::update(double dt) {
    for(unsigned int i=0; i<this->objects.size(); i++) {
        this->objects[i]->update(dt);
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

    Console::get() << std::string(__FILE__) + ": Loading mesh: " << filename << Console::endl;

    this->meshes.back()->static_load();

    return this->meshes.size() - 1;
}
