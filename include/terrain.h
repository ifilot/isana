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

#include "perlin_noise.h"
#include "object.h"

class Terrain {
private:
    ObjectColoredMesh* ter;                //!< objects representing the map

    std::vector<ObjectMesh> trees;

    unsigned int width;             //!< width of the map in units
    unsigned int height;            //!< height of the map in units

    unsigned int sample_interval;   //!< sample interval for the height map

    std::vector<float> heights;     //!< height map

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

    /**
     * @fn          get
     *
     * @brief       get a reference to the terrain
     *
     * @return      reference to the terrain object (singleton pattern)
     */
    void draw();

private:
    /**
     * @fn          Terrain
     *
     * @brief       terrain constructor
     *
     * @return      void
     */
    Terrain();

    /**
     * @fn          generate_terrain
     *
     * @brief       generate the vertices for the terrain using the height map
     *
     * @param mesh  pointer to a mesh object
     *
     * @return      void
     */
    void generate_terrain(Mesh* mesh);

    /**
     * @fn          generate_height_map
     *
     * @brief       generate the height map for the terrain
     *
     * @return      void
     */
    void generate_height_map();

    void generate_trees();

    /**
     * @fn          cubic_interpolate
     *
     * @brief       performs a 1D cubic interpolation
     *
     * @return      interpolated value
     */
    double cubic_interpolate (double p[4], double x);

    /**
     * @fn          bicubic_interpolate
     *
     * @brief       performs a 2D cubic interpolation
     *
     * @param p     matrix holding boundary values
     * @param x     fractional x coordinate [0,1]
     * @param y     fractional y coordinate [0,1]
     *
     * @return      interpolated value
     */
    double bicubic_interpolate (double p[4][4], double x, double y);

    /**
     * @fn          idx
     *
     * @brief       return the index of the height map
     *
     * @param i     x map coordinate
     * @param j     y map coordinate
     *
     * @return      void
     */
    unsigned int idx(unsigned int i, unsigned int j);

    float get_height(float x, float y);

    Terrain(Terrain const&)          = delete;
    void operator=(Terrain const&)  = delete;
};

#endif //_TERRAIN_H
