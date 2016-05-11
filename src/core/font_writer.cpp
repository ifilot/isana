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

    this->generate_character_map();
}

void FontWriter::generate_character_map() {

    static const unsigned int char_size = 100;
    static const unsigned int charmap_width = 10 * char_size;
    static const unsigned int charmap_height = 10 * char_size;

    uint8_t* expanded_data = new uint8_t[ 2 * charmap_width * charmap_height];

    for(unsigned int i=32; i<=126; i++) {
        char c = (char)i;
        std::cout << c << std::endl;

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
        FT_Glyph_Metrics& metrics = face->glyph->metrics;

        const unsigned int width = bitmap.width;
        const unsigned int height = bitmap.rows;

        const unsigned int gx = char_size * ((i-32) % 10);
        const unsigned int gy = char_size * ((i-32) / 10);

        for(unsigned int k=0; k<height; k++) {
            for(unsigned int l=0; l<width; l++){
                expanded_data[2*((l+gx)+(k+gy)*charmap_width)] =
                expanded_data[2*((l+gx)+(k+gy)*charmap_width)+1] =
                        (l>=bitmap.width || k>=bitmap.rows) ?
                        0 : bitmap.buffer[l + bitmap.width*k];
            }
        }

        FT_Done_Face(face);
    }

    int code = 0;
    FILE *fp = NULL;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;

    const char* filename = "test.png";
    const char* title = "char";

    // Open file for writing (binary mode)
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s for writing\n", filename);
        code = 1;
        //goto finalise;
    }

    // Initialize write structure
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fprintf(stderr, "Could not allocate write struct\n");
        code = 1;
        //goto finalise;
    }

    // Initialize info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fprintf(stderr, "Could not allocate info struct\n");
        code = 1;
        //goto finalise;
    }

    // Setup Exception handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Error during png creation\n");
        code = 1;
        //goto finalise;
    }

    png_init_io(png_ptr, fp);

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, info_ptr, charmap_width, charmap_height,
            8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Set title
    if (title != NULL) {
        png_text title_text;
        title_text.compression = PNG_TEXT_COMPRESSION_NONE;
        title_text.key = (char*)"Title";
        title_text.text = (char*)title;
        png_set_text(png_ptr, info_ptr, &title_text, 1);
    }

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for one row (4 bytes per pixel - RGBA)
    row = (png_bytep) malloc(4 * charmap_width * sizeof(png_byte));

    // Write image data
    unsigned int x, y;
    for (y=0 ; y<charmap_height ; y++) {
        for (x=0 ; x<charmap_width ; x++) {
            setRGBA(&(row[x*4]), expanded_data[(y*charmap_width + x)*2]);
        }
        png_write_row(png_ptr, row);
    }

    // End write
    png_write_end(png_ptr, NULL);
}
