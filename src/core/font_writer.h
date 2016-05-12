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

#ifndef _FONT_WRITER
#define _FONT_WRITER

#include <png.h>

#include <ft2build.h>
#include <freetype2/freetype.h>
#include <freetype2/ftglyph.h>
#include <freetype2/ftoutln.h>
#include <freetype2/fttrigon.h>

#include <iostream>

#include "shader.h"

class FontWriter {
private:
    FT_Library library;

    struct Glyph {
        unsigned int x;
        unsigned int y;
        unsigned int width;
        unsigned int height;
        float tx1;
        float ty1;
        float tx2;
        float ty2;
        int horizontal_bearing;
        int vertical_bearing;
        int horizontal_advance;

        Glyph() :
            x(0),
            y(0),
            width(0),
            height(0),
            tx1(0),
            ty1(0),
            tx2(0),
            ty2(0),
            horizontal_bearing(0),
            vertical_bearing(0),
            horizontal_advance(0) {}
    };

    std::vector<Glyph> glyphs;

    unsigned int texture_width;
    unsigned int texture_height;
    GLuint texture;

    Shader* shader;
    GLuint m_vertex_array_object;
    GLuint m_vertex_array_buffers[3];

    std::vector<unsigned int> indices;
    std::vector<glm::vec2> positions;
    std::vector<glm::vec2> texture_coordinates;

public:
    static FontWriter& get() {
        static FontWriter font_writer_instance;
        return font_writer_instance;
    }

    void draw();
    void static_load();

private:
    FontWriter();

    void generate_character_map();

    unsigned int base_font_size;

    FontWriter(FontWriter const&)          = delete;
    void operator=(FontWriter const&)  = delete;
};

#endif // _FONT_WRITER
