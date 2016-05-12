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

#ifndef _SCREEN_H
#define _SCREEN_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

/**
 * @class Screen class
 *
 * @brief class handling screen settings
 *
 */
class Screen {
private:
    unsigned int width;       //!< width of the screen
    unsigned int height;      //!< height of the screen

public:
    /**
     * @fn          get
     *
     * @brief       get a reference to the screen
     *
     * @return      reference to the screen object (singleton pattern)
     */
    static Screen& get() {
        static Screen screen_instance;
        return screen_instance;
    }

    inline unsigned int get_width() const {
        return this->width;
    }

    inline unsigned int get_height() const {
        return this->height;
    }

    inline void set_width(unsigned int _width) {
        this->width = _width;
    }

    inline void set_height(unsigned int _height) {
        this->height = _height;
    }

private:
    Screen();

    // Singleton pattern
    Screen(Screen const&)          = delete;
    void operator=(Screen const&)  = delete;
};


#endif //_SCREEN_H
