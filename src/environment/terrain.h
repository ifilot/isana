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

#include "accessoires/perlin_noise.h"
#include "core/object.h"
#include "ui/console.h"

/**
 * @brief      single triangle in terrain object
 */
class TerrainTriangle {
private:
    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;

    glm::vec3 n1;
    glm::vec3 n2;
    glm::vec3 n3;

    glm::vec3 normal;
    glm::vec3 color;

public:

    /**
     * @brief      TerrainTriangle constructor
     *
     * @param[in]  _p1   vector p1
     * @param[in]  _p2   vector p2
     * @param[in]  _p3   vector p3
     */
    TerrainTriangle(const glm::vec3& _p1, const glm::vec3& _p2, const glm::vec3& _p3,
                    const glm::vec3& _n1, const glm::vec3& _n2, const glm::vec3& _n3);

    /**
     * @brief      Set the color.
     *
     * @param[in]  _color  color
     */
    inline void set_color(const glm::vec3& _color) {
        this->color = _color;
    }

    /**
     * @brief      get p1
     *
     * @return     p1
     */
    inline const glm::vec3& get_p1() const {
        return this->p1;
    }


    /**
     * @brief      get p2
     *
     * @return     p2
     */
    inline const glm::vec3& get_p2() const {
        return this->p2;
    }


    /**
     * @brief      get pe
     *
     * @return     p3
     */
    inline const glm::vec3& get_p3() const {
        return this->p3;
    }

    /**
     * @brief      get n1
     *
     * @return     n1
     */
    inline const glm::vec3& get_n1() const {
        return this->n1;
    }


    /**
     * @brief      get n2
     *
     * @return     n2
     */
    inline const glm::vec3& get_n2() const {
        return this->n2;
    }


    /**
     * @brief      get ne
     *
     * @return     n3
     */
    inline const glm::vec3& get_n3() const {
        return this->n3;
    }


    /**
     * @brief      Get the color.
     *
     * @return     color.
     */
    inline const glm::vec3& get_color() const {
        return this->color;
    }


    /**
     * @brief      Get the normal.
     *
     * @return     Normal.
     */
    inline const glm::vec3& get_normal() const {
        return this->normal;
    }


    /**
     * @brief      Get the height.
     *
     * @param[in]  x     global x coordinate
     * @param[in]  y     global y coordinate
     *
     * @return     Height.
     */
    float get_height(float x, float y) const;

private:

    /**
     * @brief      Calculate the normal vector
     */
    void calculate_normal();
};

class Terrain {
private:
    Object* ter;                            //!< objects representing the map

    unsigned int width;                     //!< width of the map in units
    unsigned int height;                    //!< height of the map in units

    unsigned int sample_interval;           //!< sample interval for the height map

    std::vector<TerrainTriangle> triangles; //!< vector holding all terrain triangles
    std::vector<float> heights;             //!< height map

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

    /**
     * @brief      Get the height.
     *
     * @param[in]  x     global coordinate x
     * @param[in]  y     global coordinate y
     *
     * @return     Height.
     */
    float get_height(float x, float y);

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

    Terrain(Terrain const&)          = delete;
    void operator=(Terrain const&)  = delete;
};

#endif //_TERRAIN_H
