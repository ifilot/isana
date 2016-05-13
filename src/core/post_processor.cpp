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

/**
 * @brief       post_processor constructor
 *
 * @return      post_processor instance
 */
PostProcessor::PostProcessor() {
    this->msaa = 4;
    this->filter_flags = 0x00000000;
    this->square_mesh.load_square_mesh();
    this->square_mesh.static_load();

    glActiveTexture(GL_TEXTURE2);

    // msaa buffer
    this->create_msaa_buffer(&this->depth_msaa, &this->texture_msaa, &this->frame_buffer_msaa);

    // primary buffer
    this->create_buffer(&this->depth_p, &this->texture_p, &this->frame_buffer_p);

    // secondary buffer
    this->create_buffer(&this->depth_s, &this->texture_s, &this->frame_buffer_s);

    this->create_shader(this->shader_default, "assets/shaders/postproc");
    this->create_shader(this->shader_invert, "assets/filters/invert");

    // set blur shaders
    const float blur_radius = 1.0f;
    this->create_shader(this->shader_blur_h, "assets/filters/blur");
    const float width = (float)Screen::get().get_width();
    this->shader_blur_h->set_uniform(1, &width);
    this->shader_blur_h->set_uniform(2, &blur_radius);
    this->shader_blur_h->set_uniform(3, &glm::vec2(1,0)[0]);

    this->create_shader(this->shader_blur_v, "assets/filters/blur");
    const float height = (float)Screen::get().get_height();
    this->shader_blur_v->set_uniform(1, &height);
    this->shader_blur_v->set_uniform(2, &blur_radius);
    this->shader_blur_v->set_uniform(3, &glm::vec2(0,1)[0]);

    // unbind vertex array
    glBindVertexArray(0);
}

/**
 * @brief      bind the msaa frame buffer
 */
void PostProcessor::bind_frame_buffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->frame_buffer_msaa);
    glEnable(GL_MULTISAMPLE);
    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "glCheckFramebufferStatus: error " << status << std::endl;
        std::cerr << __FILE__ << "(" << __LINE__ << ")" << std::endl;
    }

    this->frame_buffer_active = this->frame_buffer_p;
    this->texture_active = this->texture_p;
    this->depth_active = this->depth_p;

    this->frame_buffer_passive = this->frame_buffer_s;
    this->texture_passive = this->texture_s;
    this->depth_passive = this->depth_s;
}

/**
 * @brief      unbind all frame buffers
 */
void PostProcessor::unbind_frame_buffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief      draw the result of the post processing to the screen
 */
void PostProcessor::draw() {
    // resample the buffer from msaa to regular
    this->resample_buffer();

    // apply filtering operations
    this->apply_filters();

    // render the output to the screen
    this->render(this->shader_default);
}

/**
 * @brief      updates all render buffers and textures
 */
void PostProcessor::window_reshape() {
    this->set_msaa_buffer(this->texture_msaa, this->depth_msaa);
    this->set_buffer(this->texture_p, this->depth_p);
    this->set_buffer(this->texture_s, this->depth_s);

    this->shader_blur_h->link_shader();
    const float width = (float)Screen::get().get_width();
    this->shader_blur_h->set_uniform(1, &width);

    this->shader_blur_h->link_shader();
    const float height = (float)Screen::get().get_height();
    this->shader_blur_v->set_uniform(1, &height);
}

/**
 * @brief      class destructor
 */
PostProcessor::~PostProcessor() {
    glDeleteRenderbuffers(1, &this->depth_p);
    glDeleteTextures(1, &this->texture_p);
    glDeleteFramebuffers(1, &this->frame_buffer_p);

    glDeleteRenderbuffers(1, &this->depth_s);
    glDeleteTextures(1, &this->texture_s);
    glDeleteFramebuffers(1, &this->frame_buffer_s);

    glDeleteRenderbuffers(1, &this->depth_msaa);
    glDeleteTextures(1, &this->texture_msaa);
    glDeleteFramebuffers(1, &this->frame_buffer_msaa);
}

/**
 * @brief      blit the content of the msaa fbo to the primary fbo
 */
void PostProcessor::resample_buffer() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->frame_buffer_msaa);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_p);
    glBlitFramebuffer(0, 0, Screen::get().get_width(), Screen::get().get_height(), 0, 0, Screen::get().get_width(), Screen::get().get_height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief      perform series of filter passes on the active texture
 */
void PostProcessor::apply_filters() {
    if(this->filter_flags & FILTER_BLUR) {
        this->blur();
    }

    if(this->filter_flags & FILTER_INVERT) {
        this->pass(this->shader_invert);
    }
}

/**
 * @brief      perform single filter pass on the active texture
 *
 * @param[in]   pointer to the Shader class containing the filter
 */
void PostProcessor::pass(Shader* shader) {
    //set buffer (draw to passive buffer)
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frame_buffer_passive);

    // perform draw
    this->render(shader);

    // unset buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    // swap buffers
    this->swap_active_buffer();
}

/**
 * @brief      perform blurring
 */
void PostProcessor::blur() {
    this->pass(this->shader_blur_h);
    this->pass(this->shader_blur_v);
}

/**
 * @brief      swap passive and active buffers
 */
void PostProcessor::swap_active_buffer() {
    if(this->frame_buffer_active == this->frame_buffer_p) {

        this->frame_buffer_active = this->frame_buffer_s;
        this->texture_active = this->texture_s;
        this->depth_active = this->depth_s;

        this->frame_buffer_passive = this->frame_buffer_p;
        this->texture_passive = this->texture_p;
        this->depth_passive = this->depth_p;

    } else {

        this->frame_buffer_active = this->frame_buffer_p;
        this->texture_active = this->texture_p;
        this->depth_active = this->depth_p;

        this->frame_buffer_passive = this->frame_buffer_s;
        this->texture_passive = this->texture_s;
        this->depth_passive = this->depth_s;
    }
}

/**
 * @brief      perform a texture render (used for the filters)
 */
void PostProcessor::render(Shader* shader) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, this->texture_active);

    this->square_mesh.bind();

    shader->link_shader();
    shader->set_uniform(0, NULL); // set texture id

    this->square_mesh.draw();

    this->square_mesh.unbind();

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

/**
 * @brief      create the multisampling fbo
 */
void PostProcessor::create_msaa_buffer(GLuint* render_buffer, GLuint* texture, GLuint* frame_buffer) {
    glGenRenderbuffers(1, render_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, *render_buffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, this->msaa, GL_DEPTH24_STENCIL8, Screen::get().get_width(), Screen::get().get_height());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *texture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, this->msaa, GL_RGBA, Screen::get().get_width(), Screen::get().get_height(), GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glGenFramebuffers(1, frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *frame_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, *texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *render_buffer);
    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "glCheckFramebufferStatus: error " << status << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief      create the renderbuffer objects
 */
void PostProcessor::create_buffer(GLuint* render_buffer, GLuint* texture, GLuint* frame_buffer) {
    glGenRenderbuffers(1, render_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, *render_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, Screen::get().get_width(), Screen::get().get_height());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Screen::get().get_width(), Screen::get().get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *frame_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *render_buffer);
    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "glCheckFramebufferStatus: error " << status << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief      set width and height to msaa buffers
 */
void PostProcessor::set_msaa_buffer(GLuint texture, GLuint frame_buffer) {
    // resize regular buffer
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, this->msaa, GL_RGBA, Screen::get().get_width(), Screen::get().get_height(), GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, frame_buffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, this->msaa, GL_DEPTH24_STENCIL8, Screen::get().get_width(), Screen::get().get_height());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/**
 * @brief      set with and height to regular buffers
 */
void PostProcessor::set_buffer(GLuint texture, GLuint frame_buffer) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Screen::get().get_width(), Screen::get().get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, frame_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, Screen::get().get_width(), Screen::get().get_height());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/**
 * @brief      create a shader for filtering
 */
void PostProcessor::create_shader(Shader*& shader, const std::string& filename) {
    shader = new Shader(filename);
    shader->add_attribute(ShaderAttribute::POSITION, "position");
    shader->add_uniform(ShaderUniform::TEXTURE, "text", 1);
    shader->set_texture_id(2); // corresponds to GL_TEXTURE2

    if(filename.compare("assets/filters/blur") == 0) {
        shader->add_uniform(ShaderUniform::FLOAT, "resolution", 1);
        shader->add_uniform(ShaderUniform::FLOAT, "radius", 1);
        shader->add_uniform(ShaderUniform::VEC2, "dir", 1);
    }

    this->square_mesh.bind();
    shader->bind_uniforms_and_attributes();
    this->square_mesh.unbind();
}
