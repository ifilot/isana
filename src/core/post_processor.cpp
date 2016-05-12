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

#include "post_processor.h"

PostProcessor::PostProcessor() {
    this->msaa = 4;

    // RENDER BUFFER

    glGenRenderbuffers(1, &this->depth_msaa);
    glBindRenderbuffer(GL_RENDERBUFFER, this->depth_msaa);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, this->msaa, GL_DEPTH24_STENCIL8, Screen::get().get_width(), Screen::get().get_height());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // TEXTURE

    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, &this->texture_msaa);
    glBindTexture(GL_TEXTURE_2D, this->texture_msaa);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, this->msaa, GL_RGB, Screen::get().get_width(), Screen::get().get_height(), GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Screen::get().get_width(), Screen::get().get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    // FRAME BUFFER

    glGenFramebuffers(1, &this->frame_buffer_ms);
    glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer_ms);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, this->texture_msaa, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->depth_msaa);

    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "glCheckFramebufferStatus: error " << status << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &this->frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);

    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "glCheckFramebufferStatus: error " << status << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->positions.push_back(glm::vec2(-1, -1));
    this->positions.push_back(glm::vec2( 1, -1));
    this->positions.push_back(glm::vec2( 1,  1));
    this->positions.push_back(glm::vec2(-1,  1));

    this->indices.push_back(0);
    this->indices.push_back(1);
    this->indices.push_back(2);
    this->indices.push_back(0);
    this->indices.push_back(2);
    this->indices.push_back(3);


    // load the mesh into memory
    unsigned int size = this->indices.size();

    // generate a vertex array object and store it in the pointer
    glGenVertexArrays(1, &this->m_vertex_array_object);
    glBindVertexArray(this->m_vertex_array_object);

    // generate a number of buffers (blocks of data on the GPU)
    glGenBuffers(2, this->m_vertex_array_buffers);

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
     * INDICES_VB
     */

    // bind a buffer identified by INDICES_VB and interpret this buffer as an array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertex_array_buffers[1]);
    // fill the buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

    this->shader = new Shader("assets/shaders/postproc");
    this->shader->add_attribute(ShaderAttribute::POSITION, "position");
    this->shader->add_uniform(ShaderUniform::TEXTURE, "text", 1);
    this->shader->set_texture_id(2); // corresponds to GL_TEXTURE2
    this->shader->bind_uniforms_and_attributes();

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}

void PostProcessor::bind_frame_buffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer_ms);
    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "glCheckFramebufferStatus: error " << status << std::endl;
    }
}

void PostProcessor::unbind_frame_buffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::draw() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->frame_buffer_ms);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer);
    glBlitFramebuffer(0, 0, Screen::get().get_width(), Screen::get().get_height(), 0, 0, Screen::get().get_width(), Screen::get().get_height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    glBindVertexArray(this->m_vertex_array_object);

    this->shader->link_shader();
    this->shader->set_uniform(0, NULL); // set texture id

    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

void PostProcessor::window_reshape() {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Screen::get().get_width(), Screen::get().get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, this->depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, Screen::get().get_width(), Screen::get().get_height());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

PostProcessor::~PostProcessor() {
    glDeleteRenderbuffers(1, &this->depth);
    glDeleteTextures(1, &this->texture);
    glDeleteFramebuffers(1, &this->frame_buffer);
    glDeleteBuffers(1, &this->m_vertex_array_object);
}
