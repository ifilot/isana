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
    this->base_font_size = 12;

    if(FT_Init_FreeType(&this->library)) {
        std::cerr << "FT_Init_FreeType failed" << std::endl;
    }

    this->glyphs.resize(255);
    this->generate_character_map();

    this->shader = new Shader("assets/shaders/text");
    this->shader->add_attribute(ShaderAttribute::TEXTURE_COORDINATE, "position");
    this->shader->add_attribute(ShaderAttribute::TEXTURE_COORDINATE, "texture_coordinate");
    this->shader->add_uniform(ShaderUniform::MAT4, "projection", 1);
    this->shader->add_uniform(ShaderUniform::VEC3, "textcolor", 1);
    this->shader->add_uniform(ShaderUniform::TEXTURE, "text", 1);
    this->shader->set_texture_id(1); // corresponds to GL_TEXTURE1

    glGenVertexArrays(1, &this->m_vertex_array_object);
    glBindVertexArray(this->m_vertex_array_object);
    this->shader->bind_uniforms_and_attributes();
    glBindVertexArray(0);

    FT_Done_FreeType(this->library);
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

        FT_Set_Char_Size( face, base_font_size * 64, base_font_size * 64, 128, 128);

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

    // store char map sizes
    this->texture_width = img_width;
    this->texture_height = img_height;

    // create a texture from the stored information, set the base value
    // to zero
    uint8_t* expanded_data = new uint8_t[img_width * img_height];
    for(unsigned int i=0; i<img_width * img_height; i++) {
        expanded_data[i] = 0;
    }
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

        this->glyphs[i].tx1 = (float)gx / (float)img_width;
        this->glyphs[i].tx2 = (float)(gx + width) / (float)img_width;
        this->glyphs[i].ty1 = (float)(gy + height) / (float)img_height;
        this->glyphs[i].ty2 = (float)(gy) / (float)img_height;

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

    static const std::string test_string = "The Quick Brown Fox jumped over the Lazy Dog";

    this->positions.clear();
    this->texture_coordinates.clear();
    this->indices.clear();

    float wx = 10.0f;
    float wy = 10.0f;

    const float font_pt = 12.0f;
    const float scale = font_pt / (float)base_font_size;

    for(unsigned int i=0; i<test_string.size(); i++) {
        unsigned int c = (unsigned int)test_string.at(i);

        const float fx = wx + (float)this->glyphs[c].horizontal_bearing * scale;
        const float fy = wy - (float)(this->glyphs[c].height - this->glyphs[c].vertical_bearing) * scale;

        this->positions.push_back(glm::vec2(fx, fy));
        this->positions.push_back(glm::vec2(fx + (float)this->glyphs[c].width * scale, fy));
        this->positions.push_back(glm::vec2(fx + (float)this->glyphs[c].width * scale, fy + (float)this->glyphs[c].height * scale));
        this->positions.push_back(glm::vec2(fx, fy + (float)this->glyphs[c].height * scale));

        this->texture_coordinates.push_back(glm::vec2(this->glyphs[c].tx1, this->glyphs[c].ty1));
        this->texture_coordinates.push_back(glm::vec2(this->glyphs[c].tx2, this->glyphs[c].ty1));
        this->texture_coordinates.push_back(glm::vec2(this->glyphs[c].tx2, this->glyphs[c].ty2));
        this->texture_coordinates.push_back(glm::vec2(this->glyphs[c].tx1, this->glyphs[c].ty2));

        this->indices.push_back(this->positions.size()-4);
        this->indices.push_back(this->positions.size()-3);
        this->indices.push_back(this->positions.size()-2);
        this->indices.push_back(this->positions.size()-4);
        this->indices.push_back(this->positions.size()-2);
        this->indices.push_back(this->positions.size()-1);

        wx += this->glyphs[c].horizontal_advance * scale;
    }

    float tscale = 1.0f;
    if(this->texture_width > 200) {
        tscale = 200.0f / (float)this->texture_width;
    }
    const float mx = 800 - this->texture_width * tscale - 10;
    const float my = 600 - this->texture_height * tscale  - 10;

    this->positions.push_back(glm::vec2(mx, my));
    this->positions.push_back(glm::vec2(mx + this->texture_width * tscale , my));
    this->positions.push_back(glm::vec2(mx + this->texture_width * tscale , my + this->texture_height * tscale ));
    this->positions.push_back(glm::vec2(mx, my + this->texture_height * tscale ));

    this->texture_coordinates.push_back(glm::vec2(0,1));
    this->texture_coordinates.push_back(glm::vec2(1,1));
    this->texture_coordinates.push_back(glm::vec2(1,0));
    this->texture_coordinates.push_back(glm::vec2(0,0));

    this->indices.push_back(this->positions.size()-4);
    this->indices.push_back(this->positions.size()-3);
    this->indices.push_back(this->positions.size()-2);
    this->indices.push_back(this->positions.size()-4);
    this->indices.push_back(this->positions.size()-2);
    this->indices.push_back(this->positions.size()-1);

    this->static_load();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    this->shader->link_shader();
    this->shader->set_uniform(0, &projection[0][0]);
    this->shader->set_uniform(1, &glm::vec3(0,0,0)[0]);
    this->shader->set_uniform(2, NULL);

    // load the vertex array
    glBindVertexArray(m_vertex_array_object);

    // draw the mesh using the indices
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
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
