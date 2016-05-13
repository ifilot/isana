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

#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "core/shader.h"
#include "core/font_writer.h"
#include "core/mesh.h"

class Console {
private:
    Shader* shader;
    Mesh square_mesh;

public:
    /**
     * @fn          get
     *
     * @brief       get a reference to the camera
     *
     * @return      reference to the camera object (singleton pattern)
     */
    static Console& get() {
        static Console console_instance;
        return console_instance;
    }

    void draw();

private:
    Console();

    Console(Console const&)          = delete;
    void operator=(Console const&)  = delete;
};

#endif //_CONSOLE_H
