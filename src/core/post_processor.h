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

#ifndef _POST_PROCESSOR_H
#define _POST_PROCESSOR_H

#include <GL/glew.h>
#include <iostream>
#include <vector>

#include "screen.h"
#include "shader.h"

/**
 * @class PostProcessor class
 *
 * @brief class handling the post_processor
 *
 */
class PostProcessor {
private:
    GLuint frame_buffer_msaa;         //!< OpenGL reference to multi sampling frame buffer object
    GLuint frame_buffer_p;            //!< OpenGL reference to primary frame buffer object
    GLuint frame_buffer_s;            //!< OpenGL reference to secondary frame buffer object

    GLuint texture_msaa;              //!< OpenGL reference to multisampling texture
    GLuint texture_p;                 //!< OpenGL reference to primary texture
    GLuint texture_s;                 //!< OpenGL reference to secondary texture

    GLuint depth_msaa;                //!< OpenGL reference to multisampling render buffer
    GLuint depth_p;                   //!< OpenGL reference to primary render buffer
    GLuint depth_s;                   //!< OpenGL reference to secondary render buffer

    GLuint frame_buffer_active;       //!< OpenGL reference to currently active frame buffer
    GLuint texture_active;            //!< OpenGL reference to currently active texture
    GLuint depth_active;              //!< OpenGL reference to currently active render buffer

    GLuint frame_buffer_passive;      //!< OpenGL reference to currently passive frame buffer
    GLuint texture_passive;           //!< OpenGL reference to currently passive frame texture
    GLuint depth_passive;             //!< OpenGL reference to currently passive frame render buffer

    unsigned int msaa;                //!< amount of supersampling

    Shader* shader_default;           //!< default shader
    Shader* shader_invert;            //!< shader that inverts the colors
    Shader* shader_blur_h;            //!< shader that performs horizontal blur
    Shader* shader_blur_v;            //!< shader that performs vertical blur

    GLuint m_vertex_array_object;           //!< vertex array for rendering texture
    GLuint m_vertex_array_buffers[2];       //!< vertex buffers for rendering single texture

    std::vector<glm::vec2> positions;       //!< positions for rendering a texture
    std::vector<unsigned int> indices;      //!< indices for rendering a texture

    unsigned int filter_flags;              //!< keeps track of what filters need to be applied

public:
    /**
     * @fn          get
     *
     * @brief       get a reference to the post_processor
     *
     * @return      reference to the post_processor object (singleton pattern)
     */
    static PostProcessor& get() {
        static PostProcessor post_processor_instance;
        return post_processor_instance;
    }

    static const unsigned int FILTER_BLUR = 1 << 0;
    static const unsigned int FILTER_INVERT = 1 << 1;

    /**
     * @brief       enables a filter
     *
     * @param[in]   filter bit (see above)
     */
    inline void enable_filter(unsigned int bit) {
        this->filter_flags |= bit;
    }

    /**
     * @brief       disables a filter
     *
     * @param[in]   filter bit (see above)
     */
    inline void disable_filter(unsigned int bit) {
        this->filter_flags &= ~bit;
    }

    /**
     * @brief      bind the msaa frame buffer
     */
    void bind_frame_buffer();

    /**
     * @brief      unbind all frame buffers
     */
    void unbind_frame_buffer();

    /**
     * @brief      draw the result of the post processing to the screen
     */
    void draw();

    /**
     * @brief      updates all render buffers and textures
     */
    void window_reshape();

    /**
     * @brief      class destructor
     */
    ~PostProcessor();

private:
    /**
     * @brief       post_processor constructor
     *
     * @return      post_processor instance
     */
    PostProcessor();

    /**
     * @brief      blit the content of the msaa fbo to the primary fbo
     */
    void resample_buffer();

    /**
     * @brief      perform series of filter passes on the active texture
     */
    void apply_filters();

    /**
     * @brief      perform single filter pass on the active texture
     *
     * @param[in]   pointer to the Shader class containing the filter
     */
    void pass(Shader* shader);

    /**
     * @brief      perform blurring
     */
    void blur();

    /**
     * @brief      swap passive and active buffers
     */
    void swap_active_buffer();

    /**
     * @brief      perform a texture render (used for the filters)
     */
    void render(Shader* shader);

    /**
     * @brief      create the multisampling fbo
     */
    void create_msaa_buffer(GLuint* render_buffer, GLuint* texture, GLuint* frame_buffer);

    /**
     * @brief      create the renderbuffer objects
     */
    void create_buffer(GLuint* render_buffer, GLuint* texture, GLuint* frame_buffer);

    /**
     * @brief      set width and height to msaa buffers
     */
    void set_msaa_buffer(GLuint texture, GLuint frame_buffer);

    /**
     * @brief      set with and height to regular buffers
     */
    void set_buffer(GLuint texture, GLuint frame_buffer);

    /**
     * @brief      create a shader for filtering
     */
    void create_shader(Shader*& shader, const std::string& filename);

    PostProcessor(PostProcessor const&)          = delete;
    void operator=(PostProcessor const&)  = delete;
};

#endif // _POST_PROCESSOR_H
