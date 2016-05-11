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

    this->shader = new Shader("assets/shaders/text");
    this->shader->add_attribute(ShaderAttribute::TEXTURE_COORDINATE, "position");
    this->shader->add_attribute(ShaderAttribute::TEXTURE_COORDINATE, "texture_coordinate");
    this->shader->add_uniform(ShaderUniform::MAT4, "projection", 1);
    this->shader->add_uniform(ShaderUniform::VEC3, "color", 1);
    this->shader->add_uniform(ShaderUniform::TEXTURE, "texcoord", 1);

    glGenVertexArrays(1, &this->m_vertex_array_object);
    glBindVertexArray(this->m_vertex_array_object);
    this->shader->bind_uniforms_and_attributes();
    glBindVertexArray(0);
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

    this->texture_width = img_width;
    this->texture_height = img_height;

    // create a texture from the stored information
    uint8_t* expanded_data = new uint8_t[img_width * img_height];
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
                expanded_data[(l+gx)+(k+gy)*img_width] = char_bitmaps[i][l + width*k];
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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        this->texture_width,
        this->texture_height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        expanded_data
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FontWriter::draw() {
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

    static const std::string test_string = "The big brown fox jumped over the lazy lynx";

    this->positions.clear();
    this->texture_coordinates.clear();
    this->indices.clear();

    for(unsigned int i=0; i<test_string.size(); i++) {
        unsigned int c = (unsigned int)test_string.at(i);

        const unsigned int width = this->glyphs[c].width;
        const unsigned int height = this->glyphs[c].height;

        const unsigned int gx = this->glyphs[c].x;
        const unsigned int gy = this->glyphs[c].y;

        float x = 0;
        float y = 400;

        this->positions.push_back(glm::vec2(x,y));
        this->positions.push_back(glm::vec2(x+12,y));
        this->positions.push_back(glm::vec2(x+12,y+12));
        this->positions.push_back(glm::vec2(x,y+12));

        float tx = (float)gx / (float)this->texture_width;
        float ty = (float)gy / (float)this->texture_height;
        float ww = (float)width / (float)this->texture_width;
        float hh = (float)height / (float)this->texture_height;

        this->texture_coordinates.push_back(glm::vec2(tx,ty));
        this->texture_coordinates.push_back(glm::vec2(tx+ww,ty+hh));
        this->texture_coordinates.push_back(glm::vec2(tx+ww,ty+hh));
        this->texture_coordinates.push_back(glm::vec2(tx,ty+hh));

        this->indices.push_back(this->indices.size());
        this->indices.push_back(this->indices.size());
        this->indices.push_back(this->indices.size());
        this->indices.push_back(this->indices.size());

        x += 12;
    }

    this->static_load();

    this->shader->set_uniform(0, &projection[0][0]);
    this->shader->set_uniform(1, &glm::vec3(1,1,1)[0]);

    // load the vertex array
    glBindVertexArray(m_vertex_array_object);

    // draw the mesh using the indices
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}

void FontWriter::static_load() {
    // load the mesh into memory
    unsigned int size = this->indices.size();

    // generate a vertex array object and store it in the pointer
    glBindVertexArray(this->m_vertex_array_object);

    // generate a number of buffers (blocks of data on the GPU)
    glGenBuffers(3, this->m_vertex_array_buffers);

    /*
     * POSITIONS
     */

    // bind a buffer identified by POSITION_VB and interpret this buffer as an array
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[0]);
    // fill the buffer with data
    glBufferData(GL_ARRAY_BUFFER, this->positions.size() * 2 * sizeof(float), &this->positions[0][0], GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(0);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * TEXTURE COORDINATES
     */

     // up the vertex_id
    // bind a buffer identified by POSITION_VB and interpret this buffer as an array
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[1]);
    // fill the buffer with data
    glBufferData(GL_ARRAY_BUFFER, this->texture_coordinates.size() * 2 * sizeof(float), &this->texture_coordinates[0][0], GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(1);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * INDICES_VB
     */

    // bind a buffer identified by INDICES_VB and interpret this buffer as an array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertex_array_buffers[2]);
    // fill the buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}
