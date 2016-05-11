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

#include "shader.h"

// create an empty shader
static GLuint create_shader(const std::string &text, GLenum shader_type);

// load a shader from a file
static std::string load_shader(const std::string& filename);

// check the shader for errors
static void check_shader_error(GLuint shader, GLuint flag, bool is_program, const std::string& error_message);

ShaderUniform::ShaderUniform(unsigned int _type, const std::string& _name, unsigned int _size) {
    this->type = _type;
    this->name = _name;
    this->size = _size;
}

ShaderAttribute::ShaderAttribute(unsigned int _type, const std::string& _name) {
    this->type = _type;
    this->name = _name;
}

Shader::Shader(const std::string& filename) {
    // constructs the program
    this->m_program = glCreateProgram();

    // set vertex shader
    this->m_shaders[0] = create_shader(load_shader(filename + ".vs"), GL_VERTEX_SHADER);

    // set fragment shader
    this->m_shaders[1] = create_shader(load_shader(filename + ".fs"), GL_FRAGMENT_SHADER);

    // attach all shaders
    for(unsigned int i = 0; i < NUM_SHADERS; i++) {
        glAttachShader(this->m_program, m_shaders[i]);
    }

    this->flag_loaded = false;
}

void Shader::add_uniform(unsigned int type, std::string name, unsigned int size) {
    this->shader_uniforms.push_back(ShaderUniform(type, name, size));
}

void Shader::add_attribute(unsigned int type, const std::string& name) {
    this->shader_attributes.push_back(ShaderAttribute(type, name));
}

void Shader::bind_uniforms_and_attributes() {
    for(unsigned int i=0; i<this->shader_attributes.size(); i++) {
        glBindAttribLocation(this->m_program, i, shader_attributes[i].get_name().c_str());
    }

    // links program and checks for errors
    glLinkProgram(this->m_program);
    check_shader_error(m_program, GL_LINK_STATUS, true, "Error: Program linking failed: ");

    // validates program
    glValidateProgram(this->m_program);
    check_shader_error(m_program, GL_VALIDATE_STATUS, true, "Error: Program validation failed: ");

    glUseProgram(this->m_program);

    m_uniforms.resize(shader_uniforms.size());
    for(unsigned int i=0; i<this->shader_uniforms.size(); i++) {
        m_uniforms[i] = glGetUniformLocation(this->m_program, shader_uniforms[i].get_name().c_str());
    }

    this->flag_loaded = true;
}

void Shader::set_uniform(unsigned int uniform_id, const float* val) {
    switch(this->shader_uniforms[uniform_id].get_type()) {
        case ShaderUniform::MAT4:
            glUniformMatrix4fv(m_uniforms[uniform_id], this->shader_uniforms[uniform_id].get_size(), GL_FALSE, val);
        break;
        case ShaderUniform::VEC3:
            glUniform3fv(m_uniforms[uniform_id], this->shader_uniforms[uniform_id].get_size(), val);
        break;
        case  ShaderUniform::TEXTURE:
            glUniform1i(m_uniforms[uniform_id], 0);
        break;
        case  ShaderUniform::FLOAT:
            glUniform1f(m_uniforms[uniform_id], val[0]);
        break;
        case  ShaderUniform::FRAME_MATRIX:
            glUniformMatrix4fv(m_uniforms[uniform_id], this->shader_uniforms[uniform_id].get_size(), GL_FALSE, val);
        break;
        case  ShaderUniform::OFFSET_MATRIX:
            glUniformMatrix4fv(m_uniforms[uniform_id], this->shader_uniforms[uniform_id].get_size(), GL_FALSE, val);
        break;
        default:
            // do nothing
        break;
    }
}

void Shader::link_shader() {
    glUseProgram(this->m_program);
}

/*
 * destructor function
 * Handles shape deconstruction
 */
Shader::~Shader() {
    // detach and delete the allocated shaders
    for(unsigned int i = 0; i < NUM_SHADERS; i++) {
        glDetachShader(this->m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }

    // finally delete the program
    glDeleteProgram(this->m_program);
}

static GLuint create_shader(const std::string& text, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);

    if(shader == 0) {
        std::cerr << "Error: Shader creation failed!" << std::endl;
    }

    const GLchar* shader_source_strings[1];
    GLint shader_source_strings_length[1];

    shader_source_strings[0] = text.c_str();
    shader_source_strings_length[0] = text.length();

    glShaderSource(shader, 1, shader_source_strings, shader_source_strings_length);
    glCompileShader(shader);

    check_shader_error(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");

    return shader;
}

static std::string load_shader(const std::string& filename) {
    std::ifstream file;
    file.open((filename).c_str());

    std::string output;
    std::string line;

    if(file.is_open()) {
        while(file.good()) {
            getline(file, line);
            output.append(line + "\n");
        }
    } else {
        std::cerr << "Unable to load shader: " << filename << std::endl;
    }

    return output;
}

static void check_shader_error(GLuint shader, GLuint flag, bool is_program, const std::string& error_message) {
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(is_program) {
        glGetProgramiv(shader, flag, &success);
    } else {
        glGetShaderiv(shader, flag, &success);
    }

    if(success == GL_FALSE) {
        if(is_program) {
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        } else {
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        }

        std::cerr << error_message << ": '" << error << "'" << std::endl;
    }
}
