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

#include "font_writer.h"

FontWriter::FontWriter() {
    if(FT_Init_FreeType(&this->library)) {
        std::cerr << "FT_Init_FreeType failed" << std::endl;
    }

    this->glyphs.resize(255);
    this->generate_character_map();
}

void FontWriter::generate_character_map() {

    std::vector<std::vector<uint8_t>> char_bitmaps(255);
    unsigned int img_width = 0;         // total image width
    unsigned int img_height = 0;        // total image height

    unsigned int temp_x = 0;
    unsigned int temp_y = 0;
    unsigned int counter = 0;           // iteration counter

    // collect all data of the glyphs and store them in temporary vectors
    for(unsigned int i=32; i<=126; i++) {
        counter++;
        char c = (char)i;

        FT_Face face;

        if(FT_New_Face( this->library, "./assets/fonts/arial.ttf", 0, &face )) {
            std::cerr << "FT_New_Face failed (there is probably a problem with your font file)" << std::endl;
        }

        FT_Set_Char_Size( face, 96 * 64, 96 * 64, 96, 96);

        if(FT_Load_Glyph( face, FT_Get_Char_Index( face, c ), FT_LOAD_DEFAULT )) {
            std::cerr << "FT_Load_Glyph failed" << std::endl;
        }

        // Move The Face's Glyph Into A Glyph Object.
        FT_Glyph glyph;
        if(FT_Get_Glyph( face->glyph, &glyph )) {
            std::cerr << "FT_Load_Glyph failed" << std::endl;
        }

        FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
        FT_Bitmap& bitmap=bitmap_glyph->bitmap;

        const unsigned int width = bitmap.width;
        const unsigned int height = bitmap.rows;

        this->glyphs[i].width = face->glyph->metrics.width / 64;
        this->glyphs[i].height = face->glyph->metrics.height / 64;
        this->glyphs[i].horizontal_bearing = face->glyph->metrics.horiBearingX / 64;
        this->glyphs[i].vertical_bearing = face->glyph->metrics.horiBearingY / 64;
        this->glyphs[i].horizontal_advance =  face->glyph->metrics.horiAdvance / 64;

        char_bitmaps[i] = std::vector<uint8_t>(width * height, 0.0);
        for(unsigned int j=0; j<(width*height); j++) {
            char_bitmaps[i][j] = bitmap.buffer[j];
        }

        FT_Done_Face(face);

        temp_x += width + 5;
        temp_y = std::max(temp_y, height + 5);

        if(counter % 10 == 0) {
            counter = 0;

            img_width = std::max(img_width, temp_x);
            img_height += temp_y;

            temp_x = 0;
            temp_y = 0;
        }
    }

    if(counter % 10 != 0) {
        img_height += temp_y;
    }

    // create a texture from the stored information
    uint8_t* expanded_data = new uint8_t[ 2 * img_width * img_height];
    temp_y = 0;
    unsigned int gy = 0;
    unsigned int gx = 0;

    counter = 0;
    for(unsigned int i=32; i<=126; i++) {
        counter++;
        const unsigned int width = this->glyphs[i].width;
        const unsigned int height = this->glyphs[i].height;

        this->glyphs[i].x = gx;
        this->glyphs[i].y = gy;

        for(unsigned int k=0; k<height; k++) {
            for(unsigned int l=0; l<width; l++){
                expanded_data[2*((l+gx)+(k+gy)*img_width)] =
                expanded_data[2*((l+gx)+(k+gy)*img_width)+1] =
                        char_bitmaps[i][l + width*k];
            }
        }

        gx += width + 5;
        temp_y = std::max(temp_y, height + 5);

        if(counter % 10 == 0) {
            gx = 0;
            gy += temp_y;
            temp_y = 0;
        }
    }
}
