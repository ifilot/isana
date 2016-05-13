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

#ifndef _VISUALIZER_H
#define _VISUALIZER_H

#include <boost/lexical_cast.hpp>

#include "core/display.h"
#include "core/shader.h"
#include "core/screen.h"

#include "environment/terrain.h"
#include "objects/objects_engine.h"
#include "core/font_writer.h"
#include "core/post_processor.h"
#include "ui/console.h"

/**
 * @class Visualizer class
 * @brief Initializes entities and handles time propagation.
 *
 */
class Visualizer {
public:
    /**
     * @fn Visualizer get
     * @brief Returns reference to Visualizer instance
     *
     * Static class member that returns a reference to a game instance.
     * This pattern is uses classic lazy evaluated and correctly destroyed singleton.
     * It is adapted from: http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
     *
     * @return reference to game instance
     */
    static Visualizer& get() {
        static Visualizer instance;
        return instance;
    }

    /**
     * @fn run method
     * @brief Constructs a new game
     *
     * In the run method, a while loop is started wherin the game state is
     * propagated over time.
     *
     * @return void
     */
    void run();

    /**
     * @fn handle_key_down
     * @brief Handles keyboard input
     *
     * Takes key presses as input and adjusts the game state accordingly.
     *
     * @param key the keyboard key
     * @param scancode the scancode
     * @param action the keyboard action (key down, key release)
     * @param mods
     * @return void
     */
    void handle_key_down(const int& key, const int& scancode, const int& action, const int& mods);

    /**
     * @fn handle_key_down
     * @brief Handles keyboard input
     *
     * Takes key presses as input and adjusts the game state accordingly.
     *
     * @param button the mouse button
     * @param action the mouse action
     * @param mods
     * @return void
     */
    void handle_mouse_key_down(const int& button, const int& action, const int& mods);

    /**
     * @fn handle_scroll
     * @brief handles mouse scrolls
     *
     * @return void
     */
    void handle_scroll(double xoffset, double yoffset);

private:
    /**
     * @fn Visualizer method
     * @brief Visualizer constructor method
     *
     * Loads up the display and initializes all entities.
     *
     * @return Visualizer class
     */
    Visualizer();

    /**
     * @fn update method
     * @brief Handles time integration
     *
     * Function handling time propagation
     *
     * @param dt Time integration constant
     * @return Visualizer class
     */
    void update(double dt);

    /**
     * @var accumulator
     * @brief Time accumulation tracker
     *
     * After each frame, the accumulator is incremented by the time corresponding
     * to the target frame rate. The accumulator can only be decremented by
     * performing update (integration) actions. This procedure guarantees that
     * the visualizer as the same pace independent of machine performance.
     */
    double accumulator;

    /**
     * @var frmae_start
     * @brief Current time after start of the execution
     */
    double frame_start;

    /**
     * @var dt
     * @brief Time between two frame intervals based on the target framerate.
     */
    double dt;

    /**
     * @var fps
     * @brief Target framerate
     */
    double fps;

    double angle;

    /**
     * @var state
     * @brief interface state important for handling input
     */
    unsigned int state;

    static const unsigned int STATE_CONSOLE = 1 << 0;

    void pre_draw();

    void draw();

    void post_draw();

    /* Singleton pattern; the function below are deleted */
    Visualizer(Visualizer const&)          = delete;
    void operator=(Visualizer const&)  = delete;
};

#endif // _VISUALIZER_H
