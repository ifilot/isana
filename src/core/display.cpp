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

#include "display.h"

/**
 * @fn Display
 *
 * @brief Display constructor
 *
 * Initializes the GLFW library, constructs a window and put it into context.
 * Callbacks are set-up and the GLEW library is initialized.
 *
 */
Display::Display() {
    // set the error callback
    glfwSetErrorCallback(error_callback);

    // initialize the library
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // set window hints
    //glfwWindowHint(GLFW_FLOATING, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // give hint for multisampling
    glfwWindowHint(GLFW_SAMPLES, 16);

    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);

    // create a windowed mode window and its OpenGL context
    this->m_width = 800;
    this->m_height = 600;
    this->m_window = glfwCreateWindow(this->m_width, this->m_height, "Yumi", NULL, NULL);
    glViewport(0, 0, this->m_width, this->m_height);

    // check if the window is properly constructed
    if (!this->m_window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // set the context for OpenGL
    glfwMakeContextCurrent(this->m_window);
    glfwSwapInterval(1);

    // set the key callback
    glfwSetKeyCallback(this->m_window, key_callback);

    // set mouse key callback
    glfwSetMouseButtonCallback(this->m_window, mouse_button_callback);

    // set window callback when a window is resized
    glfwSetWindowSizeCallback(this->m_window, window_size_callback);

    // set scroll callback
    glfwSetScrollCallback(this->m_window, scroll_callback);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Could not initialize GLEW" << std::endl;
    }

    // enable transparency
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // enable culling
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // enable multisampling
    glEnable(GL_MULTISAMPLE);

    // disable cursor (we are going to use our own)
    //glfwSetInputMode(this->m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // configure camera dimensions
    Camera::get().set_aspect_ratio((float)this->m_width / (float)this->m_height);
    Camera::get().update();
}

/**
 * @fn open_frame
 *
 * @brief close frame function
 *
 * Perform these instructions at the start of each frame
 */
void Display::open_frame() {
    glClearColor(249.f/255.f, 230.f/255.f, 174.f/255.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Camera::get().update();
}

/**
 * @fn close_frame
 *
 * @brief open frame function
 *
 * Perform these instructions at the end of each frame
 */
void Display::close_frame() {
        glfwSwapBuffers(this->m_window);
        glfwPollEvents();
}

/*
 * @fn is_closed
 *
 * @brief Checks if the window is closed and if so, terminates the program
 */
bool Display::is_closed() {
    return glfwWindowShouldClose(this->m_window);
}

/**
 * Display destructor
 * Destructs the display class and terminates the window and the glfw library
 */
Display::~Display() {
    glfwDestroyWindow(this->m_window);
    glfwTerminate();
}

/*
 * @fn get_aspect_ratio
 *
 * @brief get aspect ratio function
 *
 * @return the aspect ratio
 */
float Display::get_aspect_ratio() const {
    return (float)this->m_width / (float)this->m_height;
}

 /*
 * @fn set_width
 *
 * @brief set width of the display
 *
 * @param width width of the display
 */
void Display::set_width(const unsigned int &width) {
    this->m_width = width;
}

/*
 * @fn set_height
 *
 * @brief set height of the display
 *
 * @param height height of the display
 */
void Display::set_height(const unsigned int &height) {
    this->m_height = height;
}

 /*
 * @fn center_mouse_pointer
 *
 * @brief center the mouse pointer
 */
void Display::center_mouse_pointer() {
    glfwSetCursorPos(this->m_window, (float)m_width / 2.0f, (float)m_height / 2.0f);
}

/*
 * @fn set_window_title
 *
 * @brief center the mouse pointer
 *
 * @param window_name   the window name
 */
void Display::set_window_title(const std::string& window_name) {
    glfwSetWindowTitle(this->m_window, window_name.c_str());
}

/*
 * @fn get_cursor_position
 *
 * @brief get the position of the cursor
 *
 * @return the position of the cursor
 */
const glm::vec2 Display::get_cursor_position() const {
    double xpos, ypos;
    glfwGetCursorPos(this->m_window, &xpos, &ypos);
    return glm::vec2((float)xpos / (float)this->m_width,
                     (float)ypos / (float)this->m_height);
}

/*
 * *********
 * CALLBACKS
 * *********
 */

/**
 * @fn error_callback
 *
 * @brief error callback function
 *
 * @param error         error code
 * @param description   error description
 *
 */
void Display::error_callback(int error, const char* description) {
    std::cerr << description << std::endl;
}

/**
 * @fn key_callback
 *
 * @brief Registers and handles key presses
 *
 */
void Display::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    Visualizer::get().handle_key_down(key, scancode, action, mods);
}

/**
 * @fn mouse_button_callback
 *
 * @brief registers and handles mouse button presses
 */
void Display::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Visualizer::get().handle_mouse_key_down(button, action, mods);
}

/**
 * @fn scroll_callback
 *
 * @brief registers and handles mouse button presses
 */
void Display::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Visualizer::get().handle_scroll(xoffset, yoffset);
}

/**
 * @fn window_size_callback
 *
 * @brief perform window resizing
 */
void Display::window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Display::get().set_width(width);
    Display::get().set_height(height);
    Camera::get().set_aspect_ratio((float)width / (float)height);
    Camera::get().update();

    // set mouse to center
    Display::get().center_mouse_pointer();
}
