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

#include "console.h"

// used to terminate Console input
const char Console::endl = '\n';

Console::Console() {
    // keep track of time
    this->start = boost::chrono::system_clock::now();

    this->square_mesh.load_square_mesh();
    this->square_mesh.static_load();

    this->shader = new Shader("assets/shaders/console");
    this->shader->add_attribute(ShaderAttribute::POSITION, "position");
    this->shader->add_uniform(ShaderUniform::VEC4, "back_color", 1);
    this->square_mesh.bind();
    this->shader->bind_uniforms_and_attributes();
    this->square_mesh.unbind();
    this->shader->set_uniform(0, &glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)[0]);

    this->add_line_log(std::string(__FILE__) + ": Starting log...");
}

void Console::draw() {
    this->square_mesh.bind();
    shader->link_shader();
    this->square_mesh.draw();
    this->square_mesh.unbind();

    this->ly = Screen::get().get_height() - 20;
    this->ry = Screen::get().get_height() - 20;

    FontWriter::get().reset_lines();
    this->add_line_left("Project ISANA");
    this->add_line_left("Version 0.1.0");
    this->add_line_left("Compiled at " + std::string(__DATE__));
    this->add_line_left("Camera pos " + glm::to_string(Camera::get().get_position()));
    this->add_line_left("Camera distance " + boost::lexical_cast<std::string>(Camera::get().get_distance()));

    for(unsigned int i=0; i<this->log.size(); i++) {
        this->add_line_right("[" + (boost::format("%10.5f") % log_times[i]).str() + "] " + log[i]);
    }

    FontWriter::get().draw();
}

void Console::add_line_log(const std::string& line) {
    this->log.push_back(line);
    boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - this->start;
    this->log_times.push_back(sec.count());
}

void Console::add_to_log_stream(const std::string& line) {
    for(unsigned int i=0; i<line.size(); i++) {
        if(line[i] == '\n') {
            this->add_line_log(this->in_stream);
            in_stream.clear();
            continue;
        }

        this->in_stream += line[i];
    }
}

void Console::add_line_left(const std::string& line) {
    static const unsigned int lx = 10;

    FontWriter::get().add_line(lx, this->ly, line);

    this->ly -= 15;
}

void Console::add_line_right(const std::string& line) {
    const unsigned int rx = Screen::get().get_width() / 2;

    FontWriter::get().add_line(rx, this->ry, line);

    this->ry -= 15;
}
