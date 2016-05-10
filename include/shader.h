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

#ifndef _SHADER_H
#define _SHADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "camera.h"

class ShaderUniform {
public:
    ShaderUniform(unsigned int _type, const std::string& _name, unsigned int _size);

    void specify_value(float* val);

    void bind_uniform(GLuint program_id);

    inline const std::string& get_name() const {
        return this->name;
    }

    inline unsigned int get_type() const {
        return this->type;
    }

    inline unsigned int get_size() const {
        return this->size;
    }

    enum {
        MAT4,
        VEC3,
        TEXTURE,
        UINT,
        FLOAT,
        FRAME_MATRIX,
        OFFSET_MATRIX,

        NUM_VAR_TYPES
    };

private:
    unsigned int type;                           //<! type of the variable
    std::string name;                            //<! name of the variable
    unsigned int size;                           //<! size of the uniform
};

class ShaderAttribute {
public:
    ShaderAttribute(unsigned int _type, const std::string& _name);

    enum {
        POSITION,
        NORMAL,
        COLOR,
        TEXTURE_COORDINATE,
        WEIGHT,

        NUM_ATTR_TYPES
    };

    inline const std::string& get_name() const {
        return this->name;
    }

    inline unsigned int get_type() const {
        return this->type;
    }

private:
    unsigned int type;                           //<! type of the variable
    std::string name;                            //<! name of the variable
};

class Shader{
public:
    Shader(const std::string& filename);

    void add_uniform(unsigned int type, std::string name, unsigned int size);

    void add_attribute(unsigned int type, std::string name);

    void bind_uniforms_and_attributes();

    void set_uniform(unsigned int uniform_id, const float* val);

    void link_shader();

    inline bool is_loaded() const {
        return flag_loaded;
    }

    virtual ~Shader();

protected:

private:
    static const unsigned int NUM_SHADERS = 2;  // set number of shaders
    Shader(const Shader& other) {}              // initialization constructor
    void operator=(const Shader& other) {}      // copy constructor

    GLuint m_program;                           // reference pointer to the program
    GLuint m_shaders[NUM_SHADERS];              // reference array to the shaders

    std::vector<ShaderAttribute> shader_attributes;

    std::vector<ShaderUniform> shader_uniforms; //<! vector holding shader uniforms
    std::vector<GLuint> m_uniforms;             // reference array to the uniforms

    bool flag_loaded;
};

#endif //_SHADER_H
