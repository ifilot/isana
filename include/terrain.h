/**************************************************************************
#                                                                         #
#   This file is part of ISANA                                             #
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

#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "object.h"

class Terrain {
private:
    ObjectMesh* ter;

    unsigned int width;
    unsigned int height;

public:
    /**
     * @fn          get
     *
     * @brief       get a reference to the terrain
     *
     * @return      reference to the terrain object (singleton pattern)
     */
    static Terrain& get() {
        static Terrain terrain_instance;
        return terrain_instance;
    }

    void draw();
private:
    Terrain();

    void generate_terrain(Mesh* mesh);

    Terrain(Terrain const&)          = delete;
    void operator=(Terrain const&)  = delete;
};

#endif //_TERRAIN_H
