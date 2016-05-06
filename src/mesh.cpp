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

#include "mesh.h"

Mesh::Mesh() {
}

Mesh::Mesh(const std::string& filename) {
    this->load_mesh_from_file(filename);
}

void Mesh::set_indices(const std::vector<unsigned int>& _indices) {
    this->indices = _indices;
}

void Mesh::set_positions(const std::vector<glm::vec3>& _positions) {
    this->positions = _positions;
}

void Mesh::set_colors(const std::vector<glm::vec3>& _colors) {
    this->colors = _colors;
}

void Mesh::set_normals(const std::vector<glm::vec3>& _normals) {
    this->normals = _normals;
}

unsigned int Mesh::get_nr_indices() const {
    return this->indices.size();
}

unsigned int Mesh::get_nr_positions() const {
    return this->positions.size();
}

unsigned int Mesh::get_nr_normals() const {
    return this->normals.size();
}

unsigned int Mesh::get_nr_colors() const {
    return this->colors.size();
}

const glm::vec3* Mesh::get_positions_start() const {
    return &this->positions[0];
}

const glm::vec3* Mesh::get_normals_start() const {
    return &this->normals[0];
}

const glm::vec3* Mesh::get_colors_start() const {
    return &this->colors[0];
}

const glm::vec2* Mesh::get_texture_coordinates_start() const {
    return &this->texture_coordinates[0];
}

const unsigned int* Mesh::get_indices_start() const {
    return &this->indices[0];
}

void Mesh::load_mesh_from_file(const std::string& filename) {
    // open file
    std::ifstream f;
    f.open(filename.c_str());

    boost::regex v_line("v\\s+([0-9.-]+)\\s+([0-9.-]+)\\s+([0-9.-]+).*");
    boost::regex vt_line("vt\\s+([0-9.-]+)\\s+([0-9.-]+).*");
    boost::regex vn_line("vn\\s+([0-9.-]+)\\s+([0-9.-]+)\\s+([0-9.-]+).*");
    boost::regex f1_line("f\\s+([0-9]+)\\/([0-9]+)\\/([0-9]+)\\s+([0-9]+)\\/([0-9]+)\\/([0-9]+)\\s+([0-9]+)\\/([0-9]+)\\/([0-9]+).*");
    boost::regex f2_line("f\\s+([0-9]+)\\/\\/([0-9]+)\\s+([0-9]+)\\/\\/([0-9]+)\\s+([0-9]+)\\/\\/([0-9]+).*");

    std::vector<glm::vec3> _positions;
    std::vector<glm::vec2> _texture_coordinates;
    std::vector<glm::vec3> _normal_coordinates;
    std::vector<unsigned int> _position_indices;
    std::vector<unsigned int> _texture_indices;
    std::vector<unsigned int> _normal_indices;

    std::string line;
    while(getline(f, line)) {
        boost::smatch what1;

        if (boost::regex_match(line, what1, v_line)) {
            glm::vec3 pos(boost::lexical_cast<float>(what1[1]),
                          boost::lexical_cast<float>(what1[2]),
                          boost::lexical_cast<float>(what1[3]));
            _positions.push_back(pos);
        }

        if (boost::regex_match(line, what1, vt_line)) {
            glm::vec2 tex(boost::lexical_cast<float>(what1[1]),
                          boost::lexical_cast<float>(what1[2]));
            _texture_coordinates.push_back(tex);
        }

        if (boost::regex_match(line, what1, vn_line)) {
            glm::vec3 normal(boost::lexical_cast<float>(what1[1]),
                             boost::lexical_cast<float>(what1[2]),
                             boost::lexical_cast<float>(what1[3]));
            _normal_coordinates.push_back(normal);
        }

        if (boost::regex_match(line, what1, f1_line)) {
            _position_indices.push_back(boost::lexical_cast<unsigned int>(what1[1]) - 1);
            _position_indices.push_back(boost::lexical_cast<unsigned int>(what1[4]) - 1);
            _position_indices.push_back(boost::lexical_cast<unsigned int>(what1[7]) - 1);

            _texture_indices.push_back(boost::lexical_cast<unsigned int>(what1[2]) - 1);
            _texture_indices.push_back(boost::lexical_cast<unsigned int>(what1[5]) - 1);
            _texture_indices.push_back(boost::lexical_cast<unsigned int>(what1[8]) - 1);

            _normal_indices.push_back(boost::lexical_cast<unsigned int>(what1[3]) - 1);
            _normal_indices.push_back(boost::lexical_cast<unsigned int>(what1[6]) - 1);
            _normal_indices.push_back(boost::lexical_cast<unsigned int>(what1[9]) - 1);
        }

        if (boost::regex_match(line, what1, f2_line)) {
            _position_indices.push_back(boost::lexical_cast<unsigned int>(what1[1]) - 1);
            _position_indices.push_back(boost::lexical_cast<unsigned int>(what1[3]) - 1);
            _position_indices.push_back(boost::lexical_cast<unsigned int>(what1[5]) - 1);

            _normal_indices.push_back(boost::lexical_cast<unsigned int>(what1[2]) - 1);
            _normal_indices.push_back(boost::lexical_cast<unsigned int>(what1[4]) - 1);
            _normal_indices.push_back(boost::lexical_cast<unsigned int>(what1[6]) - 1);
        }
    }

    if(_position_indices.size() > 0 && _texture_indices.size() > 0) {
        for(unsigned int i=0; i<_position_indices.size(); i++) {
            this->indices.push_back(i);

            this->positions.push_back(_positions[_position_indices[i]]);
            this->texture_coordinates.push_back(_texture_coordinates[_texture_indices[i]]);
            this->normals.push_back(_normal_coordinates[_normal_indices[i]]);
        }
    }

    if(_position_indices.size() > 0 && _texture_indices.size() == 0) {
        for(unsigned int i=0; i<_position_indices.size(); i++) {
            this->indices.push_back(i);

            this->positions.push_back(_positions[_position_indices[i]]);
            this->normals.push_back(_normal_coordinates[_normal_indices[i]]);
        }
    }
}

void Mesh::center() {
    double sum_x = 0.0;
    double sum_y = 0.0;
    double sum_z = 0.0;

    for(unsigned int i=0; i<this->positions.size(); i++) {
        sum_x += this->positions[i].x;
        sum_y += this->positions[i].y;
        sum_z += this->positions[i].z;
    }

    double n_items = (double)this->positions.size();
    glm::vec3 center(sum_x / n_items,
                     sum_y / n_items,
                     sum_z / n_items);

    for(unsigned int i=0; i<this->positions.size(); i++) {
        this->positions[i] -= center;
    }
}

void Mesh::static_load() {
    // load the mesh into memory
    unsigned int size = this->indices.size();

    unsigned int vertex_id = 0;

    // generate a vertex array object and store it in the pointer
    glGenVertexArrays(1, &this->m_vertex_array_object);
    glBindVertexArray(this->m_vertex_array_object);

    // generate a number of buffers (blocks of data on the GPU)
    glGenBuffers(NUM_BUFFERS, this->m_vertex_array_buffers);

    /*
     * POSITIONS
     */

    // bind a buffer identified by POSITION_VB and interpret this buffer as an array
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[POSITION_VB]);
    // fill the buffer with data
    glBufferData(GL_ARRAY_BUFFER, this->positions.size() * 3 * sizeof(float), &this->positions[0][0], GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(vertex_id);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(vertex_id, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * NORMALS
     */

    // up the vertex_id
     vertex_id++;
    // bind a buffer identified by POSITION_VB and interpret this buffer as an array
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[NORMAL_VB]);
    // fill the buffer with data
    glBufferData(GL_ARRAY_BUFFER, this->normals.size() * 3 * sizeof(float), &this->normals[0][0], GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(vertex_id);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(vertex_id, 3, GL_FLOAT, GL_FALSE, 0, 0);


    if(this->colors.size() > 0) {
        /*
         * COLORS
         */

         // up the vertex_id
        vertex_id++;
        // bind a buffer identified by POSITION_VB and interpret this buffer as an array
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[NORMAL_VB]);
        // fill the buffer with data
        glBufferData(GL_ARRAY_BUFFER, this->colors.size() * 3 * sizeof(float), &this->colors[0][0], GL_STATIC_DRAW);

        // specifies the generic vertex attribute of index 0 to be enabled
        glEnableVertexAttribArray(vertex_id);
        // define an array of generic vertex attribute data
        glVertexAttribPointer(vertex_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    /*
     * INDICES_VB
     */

    // bind a buffer identified by INDICES_VB and interpret this buffer as an array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertex_array_buffers[INDICES_VB]);
    // fill the buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}

void Mesh::draw() const {
    // load the vertex array
    glBindVertexArray(m_vertex_array_object);

    // draw the mesh using the indices
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}

unsigned int Mesh::get_type() const {
    unsigned int type = 0;

    if(this->positions.size() > 0) {
        type |= this->MESH_POSITIONS;
    }

    if(this->normals.size() > 0) {
        type |= this->MESH_NORMALS;
    }

    if(this->colors.size() > 0) {
        type |= this->MESH_COLORS;
    }

    if(this->texture_coordinates.size() > 0) {
        type |= this->MESH_TEXTURE_COORDINATES;
    }

    return type;
}

void Mesh::bind() const {
    glBindVertexArray(this->m_vertex_array_object);
}

void Mesh::unbind() const {
    glBindVertexArray(0);
}
