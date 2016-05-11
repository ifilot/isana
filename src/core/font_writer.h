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
        unsigned int horizontal_bearing;
        unsigned int vertical_bearing;
        unsigned int horizontal_advance;

        Glyph() :
            x(0),
            y(0),
            width(0),
            height(0),
            horizontal_bearing(0),
            vertical_bearing(0),
            horizontal_advance(0) {}
    };

    std::vector<Glyph> glyphs;

public:
    static FontWriter& get() {
        static FontWriter font_writer_instance;
        return font_writer_instance;
    }
private:
    FontWriter();

    void generate_character_map();

    inline void setRGBA(png_byte *ptr, float val) {
        ptr[0] = ptr[1] = ptr[2] = ptr[3] = val;
    }

    FontWriter(FontWriter const&)          = delete;
    void operator=(FontWriter const&)  = delete;
};

#endif // _FONT_WRITER
