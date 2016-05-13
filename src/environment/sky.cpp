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

    if(this->daytime > 24) {
        this->daytime -= 24;
    }

    this->set_sky_color();

    this->sky_color[3] = 1.0f;
}

void Sky::set_sky_color() {
    static const glm::vec4 full_sunlight(249.f/255.f, 230.f/255.f, 174.f/255.f, 1.0f);
    static const glm::vec4 dusk(249.f/255.f, 80.f/255.f, 60.f/255.f, 1.0f);
    static const glm::vec4 night(0.f/255.f, 0.f/255.f, 40.f/255.f, 1.0f);

    static const double interval = 1.0; // one hour
    static const double per = M_PI / 2.0;

    float alpha;

    if(this->daytime < 0.5 * interval) { // dawn
        alpha = std::cos((this->daytime) / 0.5 * per);
        this->sky_color = alpha * night + (1.0f - alpha) * dusk;
        return;
    }

    if(this->daytime < interval) { // dawn
        alpha = std::cos((this->daytime - 0.5 * interval) / 0.5 * per);
        this->sky_color = alpha * dusk + (1.0f - alpha) * full_sunlight;
        return;
    }

    if(this->daytime < interval * 12) { // day
        this->sky_color = full_sunlight;
        return;
    }

    if(this->daytime < 13 * interval) { // dusk
        alpha = std::cos((this->daytime - 12 * interval) / 1.0 * per);
        this->sky_color = alpha * full_sunlight + (1.0f - alpha) * dusk;
        return;
    }

    if(this->daytime < 14 * interval) { // dusk
        alpha = std::cos((this->daytime - 13 * interval) / 1.0 * per);
        this->sky_color = alpha * dusk + (1.0f - alpha) * night;
        return;
    }

    this->sky_color = night;

}
