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
#include <list>

#include "shader.h"
#include "screen.h"

/**
 * @class FontWriter
 *
 * @brief writes text to the screen
 *
 */
class FontWriter {
private:
    FT_Library library;         //!< FreeType library

    /**
     * @struct Glyph
     *
     * @brief contains information of a single character
     *
     */
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

    struct TextLine {
        unsigned int x;
        unsigned int y;
        std::string line;

        TextLine(unsigned int _x,
              unsigned int _y,
              const std::string& _line) :
                x(_x),
                y(_y),
                line(_line) {}

    };

    std::vector<Glyph> glyphs;      //!< vector holding collection of glyphs
    unsigned int base_font_size;    //!< base font size for the texture

    unsigned int texture_width;     //!< width of the texture texture
    unsigned int texture_height;    //!< height of the font texture
    GLuint texture;                 //!< OpenGL reference of the texture

    Shader* shader;                     //!< pointer to text shader object
    GLuint m_vertex_array_object;       //!< reference of the VAO
    GLuint m_vertex_array_buffers[3];   //!< reference to the VAB

    std::vector<unsigned int> indices;              //!< vector holding indices for the screen characters
    std::vector<glm::vec2> positions;               //!< vector holding screen character positions
    std::vector<glm::vec2> texture_coordinates;     //!< vector holding texture coordinates for the screen characters

    std::list<TextLine> lines;

    bool display_charmap;                           //!< flag whether to print charmap to screen
    bool is_cached;                                 //!< whether current lines are cached in memory

public:
    /**
     * @brief       get a reference to the FontWriter
     *
     * @return      reference to the FontWriter object (singleton pattern)
     */
    static FontWriter& get() {
        static FontWriter font_writer_instance;
        return font_writer_instance;
    }

    void reset_lines();

    void add_line(float x, float y, const std::string& line);

    /**
     * @brief Draw the characters on the screen
     */
    void draw();

    void render_line(float x, float y, const std::string& line);

    /**
     * @brief Load the characters on the GPU
     */
    void static_load();

private:
    /**
     * @brief       FontWriter constructor
     *
     * @return      FontWriter instance
     */
    FontWriter();

    /**
     * @brief Place a font in a texture and store the positions
     */
    void generate_character_map();

    /**
     * @brief calculate the signed distance field of a Glyph
     *
     * @param[in]   distance_field      array holding distance field values
     * @param[in]   data                vector holding monochromatic glyph
     * @param[in]   width               width of the char bitmap
     * @param[in]   height              height of the char bitmap
     *
     */
    void calculate_distance_field(uint8_t* distance_field, const std::vector<bool>& data, int width, int height);

    /**
     * @brief add vertices to display the charmap to the screen
     */
    void add_charmap_to_screen();

    FontWriter(FontWriter const&)          = delete;
    void operator=(FontWriter const&)  = delete;
};

#endif // _FONT_WRITER
