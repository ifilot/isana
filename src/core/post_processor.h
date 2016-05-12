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
    GLuint frame_buffer;         //!< reference to frameBuffer object
    GLuint texture;              //!< reference to frameBuffer object
    GLuint depth;                //!< reference to frameBuffer object

    unsigned int msaa;             //!< amount of supersampling

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

    void bind_frame_buffer();

    void window_reshape();

    void unbind_frame_buffer();

    void draw();

    ~PostProcessor();

private:
    /**
     * @brief       post_processor constructor
     *
     * @return      post_processor instance
     */
    PostProcessor();

    Shader* shader;

    GLuint m_vertex_array_object;
    GLuint m_vertex_array_buffers[2];
    std::vector<glm::vec2> positions;
    std::vector<unsigned int> indices;

    PostProcessor(PostProcessor const&)          = delete;
    void operator=(PostProcessor const&)  = delete;
};

#endif // _POST_PROCESSOR_H
