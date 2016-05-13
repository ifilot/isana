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

#ifndef _OBJECT_H
#define _OBJECT_H

#include <vector>
#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "core/shader.h"
#include "core/mesh.h"
#include "environment/sky.h"

/**
 * @brief      a property that will be parsed to the shader as a uniform
 */
class ObjectProperty {
public:
    /**
     * @brief      ObjectProperty constructor
     *
     * @param[in]  _name  The name
     * @param[in]  _type  The type
     * @param[in]  _size  The size
     */
    ObjectProperty(const std::string& _name, unsigned int _type, unsigned int _size);

    /**
     * @brief      Set the value of the ObjectProperty
     *
     * @param[in]  val   value to set
     */
    void set_value(const float* val);

    /**
     * @brief      get the value of the ObjectProperty
     *
     * @return     value
     */
    inline const float* get_value() const {
        return &this->val[0];
    }

    /**
     * @brief      get the name of the ObjectProperty
     *
     * @return     name
     */
    inline const std::string& get_name() const {
        return this->name;
    }

    /**
     * @brief      get the size of the object property
     *
     * @return     size
     */
    inline unsigned int get_size() const {
        return this->size;
    }

    /**
     * @brief      get the type of the object property
     *
     * @return     type
     */
    inline unsigned int get_type() const {
        return this->type;
    }

private:
    std::vector<float> val;         //!< vector holding values

    unsigned int size;              //!< size of the object property
    unsigned int base_size;         //!< size of the base variable (e.g. 16 for mat4 and 3 for vec3)

    std::string name;               //!< name of the ObjectProperty
    unsigned int type;              //!< type of the ObjectProperty (same types as ShaderUniform)
};

/**
 * @brief      Object class
 */
class Object {
private:
    Shader* shader;                 //!< pointer to shader object
    const Mesh* mesh;               //!< pointer to mesh object

    std::vector<ObjectProperty> properties;     //!< vector holding ObjectProperties

    glm::mat4 scale;                            //!< scale matrix of the object
    glm::mat4 rotation;                         //!< rotation matrix of the object
    glm::vec3 position;                         //!< position matrix of the object

    double angle;                               //!< to be removed
    bool is_rigged;                             //!< boolean whether object has an armature
    unsigned int rig_idx;                       //!< index of ObjectProperty that represents the armature

public:
    /*
     * @brief      Object constructor
     */
    Object() {}

    /**
     * @brief      Object constructor
     *
     * @param      shader  pointer to shader
     * @param[in]  _mesh   pointer to mesh
     */
    Object(Shader* shader, const Mesh* _mesh);

    /**
     * @brief      draw the object
     */
    void draw();

    /**
     * @brief      load the object into memory
     */
    void load();

    /**
     * @brief      update the object
     *
     * @param[in]  dt    time step
     */
    void update(double dt);

    /**
     * @brief      add property to the object
     *
     * @param[in]  _name  name of the ObjectProperty
     * @param[in]  _type  variable type (derived from ShaderUniform types)
     * @param[in]  _size  size of the object
     *
     * @return     index of the ObjectProperty
     */
    unsigned int add_property(const std::string& _name, unsigned int _type, unsigned int _size);

    /**
     * @brief      set the value of an ObjectProperty
     *
     * @param[in]  prop_id  index of the property
     * @param[in]  val      pointer to memory holding values
     */
    void set_property_value(unsigned int prop_id, const float* val);

    inline void set_position(glm::vec3 _position) {
        this->position = _position;
    }

    inline void set_scale_matrix(const glm::mat4& _scale) {
        this->scale = _scale;
    }

    inline void set_rotation_matrix(const glm::mat4& _rotation) {
        this->rotation = _rotation;
    }

    inline const glm::vec3& get_position() const {
        return this->position;
    }

private:

};


#endif //_OBJECT_H
