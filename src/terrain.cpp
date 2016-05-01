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

#include "terrain.h"

Terrain::Terrain() {
    this->width = 100;
    this->height = 100;

    Shader* shader = new Shader("assets/shaders/terrain");

    shader->add_uniform(ShaderUniform::MAT4, "model");
    shader->add_uniform(ShaderUniform::MAT4, "view");
    shader->add_uniform(ShaderUniform::MAT4, "mvp");

    shader->add_attribute(ShaderAttribute::POSITION, "position");
    shader->add_attribute(ShaderAttribute::NORMAL, "normal");

    shader->bind_uniforms_and_attributes();

    Mesh* mesh = new Mesh();
    this->generate_terrain(mesh);
    mesh->center();

    this->ter = new ObjectMesh(shader, mesh);
    this->ter->static_load();
}

void Terrain::draw() {
    this->ter->draw();
}

void Terrain::generate_terrain(Mesh* mesh) {
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> positions;

    for(unsigned int j=0; j<= this->height; j++) {
        for(unsigned int i=0; i<= this->width; i++) {
            // create positions
            float x = (float)i;
            float y = (float)j;
            float z = 5.0f * std::sin(x * .2) * std::sin(y * .2);
            positions.push_back(glm::vec3(x, y, z));

            // create indices
            if(i != this->width && j != this->height) {
                unsigned int i1 = j * (this->width + 1) + i;
                unsigned int i2 = j * (this->width + 1) + i + 1;
                unsigned int i3 = (j + 1) * (this->width + 1) + i + 1;
                unsigned int i4 = (j + 1) * (this->width + 1) + i;

                indices.push_back(i1);
                indices.push_back(i2);
                indices.push_back(i4);

                indices.push_back(i2);
                indices.push_back(i3);
                indices.push_back(i4);
            }
        }
    }

    // calculate normals
    std::vector<glm::vec3> normals;
    normals.resize(positions.size(), glm::vec3(0,0,0));

    for(unsigned int i=0; i<indices.size(); i+=3) {
        glm::vec3 v1 = positions[indices[i]];
        glm::vec3 v2 = positions[indices[i+1]];
        glm::vec3 v3 = positions[indices[i+2]];

        glm::vec3 d1 = v2 - v1;
        glm::vec3 d2 = v3 - v1;

        glm::vec3 n = glm::cross(d1, d2);
        n = glm::normalize(n);

        normals[indices[i]] += n;
        normals[indices[i+1]] += n;
        normals[indices[i+2]] += n;
    }

    for(unsigned int i=0; i<normals.size(); i++) {
        normals[i]= glm::normalize(normals[i]);
    }

    mesh->set_indices(indices);
    mesh->set_positions(positions);
    mesh->set_normals(normals);
}
