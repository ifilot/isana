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

#include "object.h"

/**
 * @brief      ObjectProperty constructor
 *
 * @param[in]  _name  The name
 * @param[in]  _type  The type
 * @param[in]  _size  The size
 */
ObjectProperty::ObjectProperty(const std::string& _name, unsigned int _type, unsigned int _size) {
    this->name = _name;

    switch(_type) {
        case ShaderUniform::MAT4:
            this->base_size = 16;
            break;
        case ShaderUniform::VEC4:
            this->base_size = 4;
            break;
        case ShaderUniform::VEC3:
            this->base_size = 3;
            break;
        case ShaderUniform::VEC2:
            this->base_size = 2;
            break;
        case ShaderUniform::TEXTURE:
            this->base_size = 1;
            break;
        case  ShaderUniform::UINT:
            this->base_size = 1;
            break;
        case  ShaderUniform::FLOAT:
            this->base_size = 1;
            break;
        case  ShaderUniform::FRAME_MATRIX:
            this->base_size = 16;
            break;
        case  ShaderUniform::OFFSET_MATRIX:
            this->base_size = 16;
            break;
        default:
            this->base_size = 1;
        break;
    }

    this->val.resize(_size * base_size, 0.0f);
    this->size = _size;
    this->type = _type;
}

/**
 * @brief      Set the value of the ObjectProperty
 *
 * @param[in]  val   value to set
 */
void ObjectProperty::set_value(const float* _val) {
    for(unsigned int i=0; i<this->size * this->base_size; i++) {
        this->val[i] = _val[i];
    }
}

/**
 * @brief      Object constructor
 *
 * @param      shader  pointer to shader
 * @param[in]  _mesh   pointer to mesh
 */
Object::Object(Shader* _shader, const Mesh* _mesh) {
    /* load the default shader */
    this->is_rigged = false;
    this->shader = _shader;
    this->mesh = _mesh;
    this->scale = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);
    this->angle = 0.0;
    this->rig_idx = 0;

    this->add_property("model", ShaderUniform::MAT4, 1);
    this->add_property("view", ShaderUniform::MAT4, 1);
    this->add_property("mvp", ShaderUniform::MAT4, 1);
    this->add_property("ambient_light", ShaderUniform::VEC4, 1);

    if(this->mesh->get_type() & Mesh::MESH_TEXTURE_COORDINATES) {
        this->add_property("tex", ShaderUniform::TEXTURE, 1);
    }

    if(this->mesh->get_type() & Mesh::MESH_ARMATURE) {
        this->is_rigged = true;
        this->rig_idx = this->add_property("armature", ShaderUniform::MAT4, this->mesh->get_armature()->get_nr_bones());
        this->mesh->get_armature()->set_bone_transformation(2, glm::rotate(glm::mat4(1.0), (float)M_PI / 2.0f, glm::vec3(0,0,1)));
        this->mesh->get_armature()->set_bone_transformation(1, glm::rotate(glm::mat4(1.0), float(rand() * M_PI), glm::vec3(0,0,1)));
        this->mesh->get_armature()->build_frame_matrices();
        this->mesh->get_armature()->build_glsl_matrices();
    }
}

/**
 * @brief      draw the object
 */
void Object::draw() {
    glm::mat4 view = Camera::get().get_view();
    glm::mat4 projection = Camera::get().get_projection();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), this->position) * this->rotation * this->scale;
    glm::mat4 mvp = projection * view * model;
    glm::vec4 sky_color = Sky::get().get_sky_color();

    this->properties[0].set_value(glm::value_ptr(model));
    this->properties[1].set_value(glm::value_ptr(view));
    this->properties[2].set_value(glm::value_ptr(mvp));
    this->properties[3].set_value(glm::value_ptr(sky_color));

    this->mesh->bind();

    this->shader->link_shader();
    for(unsigned int i=0; i<this->properties.size(); i++) {
        this->shader->set_uniform(i, this->properties[i].get_value());
    }

    this->mesh->draw();

    this->mesh->unbind();
}

/**
 * @brief      load the object into memory
 */
void Object::load() {
    // load uniforms
    for(unsigned int i=0; i<this->properties.size(); i++) {
        this->shader->add_uniform(this->properties[i].get_type(), this->properties[i].get_name(), this->properties[i].get_size());
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
    if(mesh_type & Mesh::MESH_ARMATURE) {
        this->shader->add_attribute(ShaderAttribute::WEIGHT, "weights");
    }

    if(!this->shader->is_loaded()) {
        // corresponding mesh needs to be bound when vertex array gets loaded
        this->mesh->bind();
        this->shader->bind_uniforms_and_attributes();
        this->mesh->unbind();
    }
}

/**
 * @brief      update the object
 *
 * @param[in]  dt    time step
 */
void Object::update(double dt) {
    if(this->mesh->get_type() & Mesh::MESH_ARMATURE) {
        angle += dt;

        if(angle > 2.0 * M_PI) {
            angle -= 2.0 * M_PI;
        }
        glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0), (float)this->angle, glm::vec3(0,0,1));
        this->mesh->get_armature()->set_bone_transformation(2, rot_mat);
        this->mesh->get_armature()->build_glsl_matrices();
        this->properties[this->rig_idx].set_value(&this->mesh->get_armature()->get_glsl_matrices()[0][0][0]);
    }
}

/**
 * @brief      add property to the object
 *
 * @param[in]  _name  name of the ObjectProperty
 * @param[in]  _type  variable type (derived from ShaderUniform types)
 * @param[in]  _size  size of the object
 *
 * @return     index of the ObjectProperty
 */
unsigned int Object::add_property(const std::string& _name, unsigned int type, unsigned int size) {
    this->properties.push_back(ObjectProperty(_name, type, size));
    return this->properties.size() - 1;
}

/**
 * @brief      set the value of an ObjectProperty
 *
 * @param[in]  prop_id  index of the property
 * @param[in]  val      pointer to memory holding values
 */
void Object::set_property_value(unsigned int prop_id, const float* val) {
    this->properties[prop_id].set_value(val);
}
