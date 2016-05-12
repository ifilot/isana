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

#include "visualizer.h"

/**
 * @fn run method
 * @brief Constructs a new game
 *
 * In the run method, a while loop is started wherein the game state is
 * propagated over time.
 *
 * @return void
 */
void Visualizer::run() {
    /**
     * @var num_frames
     * @brief Counts the number of frames in a single second
     */
    unsigned int num_frames = 0;

    /**
     * @var last_time
     * @brief The time since the execution of the program after the
     *        last frame call.
     */
    double last_time = glfwGetTime();

    /* while the display runs, do at every frame */
    while(!Display::get().is_closed()) {

        /**
         * @var current_time
         * @brief The time since the execution of the program before the
         *        new integration step(s)
         */
        const double current_time = glfwGetTime();

        /* add passed time to the accumulator */
        this->accumulator += current_time - frame_start;

        /* keep integrating the game state until the accumulator
           is sufficiently depleted that a frame can be rendered */
        while(accumulator >= dt) {
            this->update(dt);
            this->accumulator -= dt;
        }

        /* calculate the FPS at every second and update the window
           title accordingly */
        if(current_time - last_time >= 1.0) {
            /**
             * @var fps
             * @brief string holding the frames per second
             */
            std::string fps = boost::lexical_cast<std::string>(num_frames);
            Display::get().set_window_title("Yumi::Running at: " + fps + std::string(" fps"));

            /* reset counters */
            last_time = glfwGetTime();
            num_frames = 0;
        }

        /* set the start time of the frame rendering */
        this->frame_start = current_time;

        /* render the game state */
        num_frames++;                  /* increment frame counter */
        Display::get().open_frame();   /* start new frame */

        // start drawing from here

        // draw topology
        this->draw();

        // stop drawing here
        Display::get().close_frame();  /* close the frame */
    }
}

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
 *
 * @return void
 */
void Visualizer::handle_key_down(const int& key, const int& scancode, const int& action, const int& mods) {
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {

    }

    if(key == 'A') {
        Camera::get().pan_left();
    }

    if(key == 'D') {
        Camera::get().pan_right();
    }

    if(key == 'S') {
        Camera::get().pan_up();
    }

    if(key == 'W') {
        Camera::get().pan_down();
    }

    if(key == 'Q') {
        Camera::get().angle_ccw();
    }

    if(key == 'E') {
        Camera::get().angle_cw();
    }
}

/**
 * @fn handle_key_down
 * @brief Handles keyboard input
 *
 * Takes key presses as input and adjusts the game state accordingly.
 *
 * @param button the mouse button
 * @param action the mouse action
 * @param mods
 *
 * @return void
 */
void Visualizer::handle_mouse_key_down(const int& button, const int& action, const int& mods) {

}

void Visualizer::handle_scroll(double xoffset, double yoffset) {
    if(yoffset > 0) {
        Camera::get().zoom_out();
    }

    if(yoffset < 0) {
        Camera::get().zoom_in();
    }
}

/**
 * @fn Game method
 * @brief Game constructor method
 *
 * Loads up the display and initializes all entities.
 *
 * @return Game class
 */
Visualizer::Visualizer():
    accumulator(0.0),       /* default accumulator should be zero */
    fps(60.0)               /* set the target framerate */
    {

    this->angle = 0.0;

    /* calculate the time interval */
    this->dt = 1.0 / fps;

    /* set the time at the frame start */
    this->frame_start = glfwGetTime();

    /* make sure the display is loaded before loading the shader */
    Display::get();

    // add objects
    ObjectsEngine::get();
}

/**
 * @fn update method
 * @brief Handles time integration
 *
 * Function handling time propagation
 *
 * @param dt Time integration constant
 * @return Game class
 */
void Visualizer::update(double dt) {
    ObjectsEngine::get().update(dt);
}

void Visualizer::draw() {
    Terrain::get().draw();
    ObjectsEngine::get().draw();
    FontWriter::get().draw();
}
