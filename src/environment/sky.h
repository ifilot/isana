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

#ifndef _SKY_H
#define _SKY_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <iostream>

class Sky {
private:
    glm::vec4 sky_color;
    double daytime;

public:
    static Sky& get() {
        static Sky sky_instance;
        return sky_instance;
    }

    inline const glm::vec4& get_sky_color() const {
        return this->sky_color;
    }

    void update(double dt);

private:
    Sky();

    void set_sky_color();

    Sky(Sky const&)          = delete;
    void operator=(Sky const&)  = delete;
};

#endif // _SKY_H
