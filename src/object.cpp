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

#include "object.h"

ObjectProperty::ObjectProperty(const std::string& _name, unsigned int size) {
    this->name = _name;
    this->val.resize(size, 0.0f);
    this->val_size = size;
}

void ObjectProperty::set_value(const float* _val) {
    for(unsigned int i=0; i<this->val_size; i++) {
        this->val[i] = _val[i];
    }
}

Object::Object(Shader* _shader, const Mesh* _mesh) {
    /* load the default shader */
    this->shader = _shader;
    this->mesh = _mesh;
    this->scale = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);

    this->add_property("model", 16);
    this->add_property("view", 16);
    this->add_property("mvp", 16);
}

void Object::draw() {
    glm::mat4 view = Camera::get().get_view();
    glm::mat4 projection = Camera::get().get_projection();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), this->position) * this->rotation * this->scale;
    glm::mat4 mvp = projection * view * model;

    this->properties[0].set_value(glm::value_ptr(model));
    this->properties[1].set_value(glm::value_ptr(view));
    this->properties[2].set_value(glm::value_ptr(mvp));

    this->mesh->bind();

    this->shader->link_shader();
    for(unsigned int i=0; i<this->properties.size(); i++) {
        this->shader->set_uniform(i, this->properties[i].get_value());
    }

    this->mesh->draw();

    this->mesh->unbind();
}

void Object::load() {
    // load uniforms
    for(unsigned int i=0; i<this->properties.size(); i++) {
        if(this->properties[i].get_size() == 1) {
            this->shader->add_uniform(ShaderUniform::TEXTURE, this->properties[i].get_name());
        }
        if(this->properties[i].get_size() == 3) {
            this->shader->add_uniform(ShaderUniform::VEC3, this->properties[i].get_name());
        }
        if(this->properties[i].get_size() == 16) {
            this->shader->add_uniform(ShaderUniform::MAT4, this->properties[i].get_name());
        }
    }

    // load attributes (depend on mesh)
    unsigned int mesh_type = this->mesh->get_type();
    if(mesh_type & Mesh::MESH_POSITIONS) {
        this->shader->add_attribute(ShaderAttribute::POSITION, "position");
    }
    if(mesh_type & Mesh::MESH_NORMALS) {
        this->shader->add_attribute(ShaderAttribute::NORMAL, "normal");
    }
    if(mesh_type & Mesh::MESH_COLORS) {
        this->shader->add_attribute(ShaderAttribute::COLOR, "color");
    }
    if(mesh_type & Mesh::MESH_TEXTURE_COORDINATES) {
        this->shader->add_attribute(ShaderAttribute::TEXTURE_COORDINATE, "texture_coordinate");
    }

    if(!this->shader->is_loaded()) {
        // corresponding mesh needs to be bound when vertex array gets loaded
        this->mesh->bind();
        this->shader->bind_uniforms_and_attributes();
        this->mesh->unbind();
    }
}

unsigned int Object::add_property(const std::string& _name, unsigned int size) {
    this->properties.push_back(ObjectProperty(_name, size));
    return this->properties.size() - 1;
}

void Object::set_property_value(unsigned int prop_id, const float* val) {
    this->properties[prop_id].set_value(val);
}
