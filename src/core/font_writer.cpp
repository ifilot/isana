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

/**
 * @brief       FontWriter constructor
 *
 * @return      FontWriter instance
 */
FontWriter::FontWriter() {
    this->base_font_size = 32;
    this->display_charmap = true;
    this->is_cached = false;

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

void FontWriter::reset_lines() {
    this->lines.clear();
}

void FontWriter::add_line(float x, float y, const std::string& line) {
    this->lines.push_back(TextLine(x,y,line));
}

/**
 * @brief Draw the characters on the screen
 */
void FontWriter::draw() {
    glm::mat4 projection = glm::ortho(0.0f,
                                      (float)Screen::get().get_width(),
                                      0.0f,
                                      (float)Screen::get().get_height());

    this->positions.clear();
    this->texture_coordinates.clear();
    this->indices.clear();

    for(std::list<TextLine>::const_iterator iterator = lines.begin(), end = lines.end(); iterator != end; ++iterator) {
        this->render_line((*iterator).x, (*iterator).y, (*iterator).line);
    }

    if(this->display_charmap) {
        this->add_charmap_to_screen();
    }

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

void FontWriter::render_line(float x, float y, const std::string& line) {
    float wx = x;
    float wy = y;

    const float font_pt = 8.0f;
    const float scale = font_pt / (float)base_font_size;

    for(unsigned int i=0; i<line.size(); i++) {
        unsigned int c = (unsigned int)line.at(i);

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
}

/**
 * @brief Load the characters on the GPU
 */
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

void FontWriter::add_charmap_to_screen() {
    float tscale = 1.0f;
    if(this->texture_width > 200) {
        tscale = 200.0f / (float)this->texture_width;
    }
    tscale = 1.0f;
    const float mx = (float)Screen::get().get_width() - this->texture_width * tscale - 10;
    const float my = 0;

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
}

/**
 * @brief Place a font in a texture and store the positions
 */
void FontWriter::generate_character_map() {

    std::vector<std::vector<bool>> char_bitmaps(255);
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

        if(FT_New_Face( this->library, "./assets/fonts/sourcecodepro-regular.ttf", 0, &face )) {
            std::cerr << "FT_New_Face failed (there is probably a problem with your font file)" << std::endl;
        }

        FT_Set_Char_Size( face, base_font_size * 64, base_font_size * 64, 128, 128);

        if(FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO)) {
            std::cerr << "Error loading char." << std::endl;
        }

        const unsigned int width = face->glyph->bitmap.width + 2 * this->font_padding;
        const unsigned int height = face->glyph->bitmap.rows + 2 * this->font_padding;

        this->glyphs[i].width = face->glyph->bitmap.width;
        this->glyphs[i].height = face->glyph->bitmap.rows;
        this->glyphs[i].horizontal_bearing = face->glyph->metrics.horiBearingX / 64;
        this->glyphs[i].vertical_bearing = face->glyph->metrics.horiBearingY / 64;
        this->glyphs[i].horizontal_advance =  face->glyph->metrics.horiAdvance / 64;

        char_bitmaps[i] = this->unpack_mono_bitmap(face->glyph->bitmap);

        FT_Done_Face(face);

        temp_x += width;
        temp_y = std::max(temp_y, height);

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
    std::vector<uint8_t> expanded_data(img_width * img_height, 0.0);
    temp_y = 0;
    unsigned int gy = 0;
    unsigned int gx = 0;

    counter = 0;
    for(unsigned int i=32; i<=126; i++) {
        counter++;
        const unsigned int width = this->glyphs[i].width + 2 * this->font_padding;
        const unsigned int height = this->glyphs[i].height + 2 * this->font_padding;

        this->glyphs[i].x = gx;
        this->glyphs[i].y = gy;

        std::vector<uint8_t> distance_field(width * height, 0.0f);
        this->calculate_distance_field(distance_field,
                                       char_bitmaps[i],
                                       width,
                                       height);

        for(unsigned int k=0; k<height; k++) {
            for(unsigned int l=0; l<width; l++){
                expanded_data[(l+gx)+(k+gy)*img_width] = distance_field[l + width*k];
            }
        }

        this->glyphs[i].tx1 = (float)gx / (float)img_width;
        this->glyphs[i].tx2 = (float)(gx + width) / (float)img_width;
        this->glyphs[i].ty1 = (float)(gy + height) / (float)img_height;
        this->glyphs[i].ty2 = (float)(gy) / (float)img_height;

        gx += width;
        temp_y = std::max(temp_y, height);

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
        &expanded_data[0]
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

std::vector<bool> FontWriter::unpack_mono_bitmap(FT_Bitmap bitmap) {
    std::vector<bool> result( (bitmap.rows + 2 * this->font_padding) * (bitmap.width + 2 * this->font_padding), false);

    for (int y = 0; y < bitmap.rows; y++) {
        for (int byte_index = 0; byte_index < bitmap.pitch; byte_index++) {

            unsigned char byte_value = bitmap.buffer[y * bitmap.pitch + byte_index];

            int num_bits_done = byte_index * 8;

            int rowstart = (y + this->font_padding) * (bitmap.width + 2 * this->font_padding) + byte_index * 8 + this->font_padding;

            for (int bit_index = 0; bit_index < std::min(8, (int)(bitmap.width - num_bits_done)); bit_index++) {
                int bit = byte_value & (1 << (7 - bit_index));

                result[rowstart + bit_index] = bit;
            }
        }
    }

    return result;
}

void FontWriter::calculate_distance_field(std::vector<uint8_t>& distance_field, const std::vector<bool>& data, int width, int height) {
    static const int sample_depth = 10;
    const float max_dist = std::sqrt((float)(2 * sample_depth * sample_depth));

    for(int k=0; k<height; k++) {
        for(int l=0; l<width; l++){

            float distance = max_dist;
            for(int j=k-sample_depth; j<=k+sample_depth; j++) {
                for(int i=l-sample_depth; i<=l+sample_depth; i++) {
                    if(i >= 0 && i < width && j >= 0 && j < height) {
                        if(data[l + width*k] != data[i + width*j]) {
                            float dist = std::sqrt((float)((i-l)*(i-l)) + (float)((j-k)*(j-k)));
                            std::min(distance, dist);
                        }
                    }
                }
            }

            if(data[l + width*k]) { // inside
                distance_field[l+k*width] = (0.5f + (distance / max_dist) * 0.5f) * 255.0f;
            } else { // outside
                distance_field[l+k*width] = (0.5f - (distance / max_dist) * 0.5f) * 255.0f;
            }
        }
    }
}
