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

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstdlib> // EXIT_FAILURE
#include <iostream>
#include <boost/format.hpp>

#include "camera.h"
#include "visualizer.h"
#include "screen.h"
#include "post_processor.h"

/**
 * @class Display class
 *
 * @brief class handling the display
 *
 */
class Display {
public:
    // classic lazy evaluated and correctly destroyed singleton
    // adapted from: http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
    static Display& get() {
        static Display instance;
        return instance;
    }

    /**
     * Display destructor
     * Destructs the display class and terminates the window and the glfw library
     */
    virtual ~Display();

    /**
     * @fn open_frame
     *
     * @brief close frame function
     *
     * Perform these instructions at the start of each frame
     */
    void open_frame();

    /**
     * @fn close_frame
     *
     * @brief open frame function
     *
     * Perform these instructions at the end of each frame
     */
    void close_frame();

    /*
     * @fn is_closed
     *
     * @brief Checks if the window is closed and if so, terminates the program
     */
    bool is_closed();

    /*
     * @fn get_aspect_ratio
     *
     * @brief get aspect ratio function
     *
     * @return the aspect ratio
     */
    float get_aspect_ratio() const;

    /*
     * @fn set_width
     *
     * @brief set width of the display
     *
     * @param width width of the display
     */
    void set_width(const unsigned int &width);

    /*
     * @fn set_height
     *
     * @brief set height of the display
     *
     * @param height height of the display
     */
    void set_height(const unsigned int &height);

    /*
     * @fn center_mouse_pointer
     *
     * @brief center the mouse pointer
     */
    void center_mouse_pointer();

    /*
     * @fn set_window_title
     *
     * @brief center the mouse pointer
     *
     * @param window_name   the window name
     */
    void set_window_title(const std::string& window_name);

    /*
     * @fn get_cursor_position
     *
     * @brief get the position of the cursor
     *
     * @return the position of the cursor
     */
    const glm::vec2 get_cursor_position() const;

    /**
     * @fn error_callback
     *
     * @brief error callback function
     *
     * @param error         error code
     * @param description   error description
     *
     */
    static void error_callback(int error, const char* description);

    /**
     * @fn key_callback
     *
     * @brief Registers and handles key presses
     *
     */
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /**
     * @fn mouse_button_callback
     *
     * @brief registers and handles mouse button presses
     */
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    /**
     * @fn window_size_callback
     *
     * @brief perform window resizing
     */
    static void window_size_callback(GLFWwindow* window, int width, int height);

    /**
     * @fn scroll_callback
     *
     * @brief handle scroll callbacks
     */
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

private:
    /**
     * @fn Display
     *
     * @brief Display constructor
     *
     * Initializes the GLFW library, constructs a window and put it into context.
     * Callbacks are set-up and the GLEW library is initialized.
     *
     */
    Display();

    GLFWwindow* m_window;       //!< pointer to the window

    // Singleton pattern
    Display(Display const&)          = delete;
    void operator=(Display const&)  = delete;
};

#endif // _DISPLAY_H
