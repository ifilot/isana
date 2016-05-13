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

#include "sky.h"

Sky::Sky() {
    this->set_sky_color();
}

void Sky::update(double dt) {
    this->daytime += dt;

    if(this->daytime > 2.0 * M_PI) {
        this->daytime -= 2.0 * M_PI;
    }

    this->set_sky_color();
}

void Sky::set_sky_color() {
    static const glm::vec4 full_sunlight(249.f/255.f, 230.f/255.f, 174.f/255.f, 1.0f);
    static const glm::vec4 full_moonlight = glm::vec4(glm::vec3(1,1,1) - glm::vec3(full_sunlight), 1.0f);

    float alpha = std::cos(this->daytime);
    this->sky_color = alpha * full_sunlight + (1.0f - alpha) * full_moonlight;
}
