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

#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <stdlib.h>

// opengl libs
#include <GL/glew.h>

// boost libs
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <glm/glm.hpp>

// image libs
#include <png.h>

/*
 * Texture class
 * Handles reading a texture (image) file from the hard drive and placing it in
 * GPU memory. Can read from both PNG as well as JPG files.
 */
class Texture {
public:
    Texture(const std::string& filename);

    void bind(unsigned int unit);

    virtual ~Texture();
protected:
private:
    Texture(const Texture& other) {}
    void operator=(const Texture& other) {}
    GLuint png_texture_load(const char * file_name, unsigned int * width, unsigned int * height);

    GLuint m_texture;
};

class TextureManager {
public:
    static TextureManager& get() {
        static TextureManager instance;
        return instance;
    }

private:
    TextureManager();

    void load_image();

    Texture* texture;

    // Singleton pattern
    TextureManager(TextureManager const&)          = delete;
    void operator=(TextureManager const&)  = delete;
};

#endif //_TEXTURE_MANAGER_H
