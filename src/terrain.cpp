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

/**
 * @fn          Terrain
 *
 * @brief       terrain constructor
 *
 * @return      void
 */
Terrain::Terrain() {
    this->width = 50;
    this->height = 50;
    this->sample_interval = 10;

    Shader* shader = new Shader("assets/shaders/terrain");

    shader->add_uniform(ShaderUniform::MAT4, "model");
    shader->add_uniform(ShaderUniform::MAT4, "view");
    shader->add_uniform(ShaderUniform::MAT4, "mvp");

    shader->add_attribute(ShaderAttribute::POSITION, "position");
    shader->add_attribute(ShaderAttribute::NORMAL, "normal");
    shader->add_attribute(ShaderAttribute::COLOR, "colors");

    shader->bind_uniforms_and_attributes();

    Mesh* mesh = new Mesh();
    this->generate_terrain(mesh);

    this->ter = new ObjectColoredMesh(shader, mesh);
    this->ter->static_load();

    this->generate_trees();
}

/**
 * @fn          get
 *
 * @brief       get a reference to the terrain
 *
 * @return      reference to the terrain object (singleton pattern)
 */
void Terrain::draw() {
    this->ter->draw();

    for(unsigned int i=0; i<this->trees.size(); i++) {
        this->trees[i].draw();
    }
}

/**
 * @fn          generate_terrain
 *
 * @brief       generate the vertices for the terrain using the height map
 *
 * @param mesh  pointer to a mesh object
 *
 * @return      void
 */
void Terrain::generate_terrain(Mesh* mesh) {
    // generate terrain
    this->generate_height_map();

    std::vector<unsigned int> indices_eff;
    std::vector<glm::vec3> positions_eff;
    std::vector<glm::vec3> colors_eff;

    for(unsigned int j=0; j<= this->height; j++) {
        for(unsigned int i=0; i<= this->width; i++) {
            // create positions
            float x = (float)i;
            float y = (float)j;
            positions_eff.push_back(glm::vec3(x, y, this->heights[j * (this->width + 1) + i]));

            // create indices
            if(i != this->width && j != this->height) {
                unsigned int i1 = j * (this->width + 1) + i;
                unsigned int i2 = j * (this->width + 1) + i + 1;
                unsigned int i3 = (j + 1) * (this->width + 1) + i + 1;
                unsigned int i4 = (j + 1) * (this->width + 1) + i;

                indices_eff.push_back(i1);
                indices_eff.push_back(i2);
                indices_eff.push_back(i4);

                indices_eff.push_back(i2);
                indices_eff.push_back(i3);
                indices_eff.push_back(i4);
            }
        }
    }

    // calculate normals
    std::vector<glm::vec3> normals_eff;
    normals_eff.resize(positions_eff.size(), glm::vec3(0,0,0));
    colors_eff.resize(positions_eff.size(), glm::vec3(0,0,0));

    for(unsigned int i=0; i<indices_eff.size(); i+=3) {
        glm::vec3 v1 = positions_eff[indices_eff[i]];
        glm::vec3 v2 = positions_eff[indices_eff[i+1]];
        glm::vec3 v3 = positions_eff[indices_eff[i+2]];

        glm::vec3 d1 = v2 - v1;
        glm::vec3 d2 = v3 - v1;

        glm::vec3 n = glm::cross(d1, d2);
        n = glm::normalize(n);

        normals_eff[indices_eff[i]] += n;
        normals_eff[indices_eff[i+1]] += n;
        normals_eff[indices_eff[i+2]] += n;
    }

    for(unsigned int i=0; i<normals_eff.size(); i++) {
        normals_eff[i]= glm::normalize(normals_eff[i]);
    }

    std::vector<unsigned int> indices;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;

    for(unsigned int i=0; i<indices_eff.size(); i++) {
        indices.push_back(i);
        positions.push_back(positions_eff[indices_eff[i]]);
        normals.push_back(normals_eff[indices_eff[i]]);
    }

    colors.resize(indices.size());
    const glm::vec3 base_color = glm::vec3(53.f / 255.f, 51.f / 255.f, 48.f / 255.f);
    for(unsigned int i=0; i<indices.size(); i+=3) {
        PerlinNoiseGenerator pn(10.0f, 10.5f, 5, 2763226322);
        float c = (pn.get_perlin_noise(i) - 1.0) / 30.0f;

        glm::vec3 mod = glm::vec3(1.0f) * c;

        colors[i] = base_color + mod;
        colors[i+1] = base_color + mod;
        colors[i+2] = base_color + mod;
    }

    mesh->set_indices(indices);
    mesh->set_positions(positions);
    mesh->set_normals(normals);
    mesh->set_colors(colors);
}

/**
 * @fn          generate_height_map
 *
 * @brief       generate the height map for the terrain
 *
 * @return      void
 */
void Terrain::generate_height_map() {
    PerlinNoiseGenerator pn(0.7f, 1.2f, 5, 2763226322);
    this->heights.resize((this->width + 1) * (this->height + 1), 0.0);
    for(unsigned int j=0; j<= this->height; j+=this->sample_interval) {
        for(unsigned int i=0; i<= this->width; i+=this->sample_interval) {
            this->heights[j * (this->width + 1) + i] = pn.get_perlin_noise(i + j * (this->width + 1));
        }
    }

    for(unsigned int j=0; j<= this->height; j++) {
        for(unsigned int i=0; i<= this->width; i++) {

            if(i % this->sample_interval == 0 && j % this->sample_interval == 0) {
                continue;
            }

            unsigned int x = floor(i / this->sample_interval) * this->sample_interval;
            unsigned int y = floor(j / this->sample_interval) * this->sample_interval;

            unsigned int ixx[4];
            unsigned int iyy[4];

            if(!(i % this->sample_interval == 0 && j % this->sample_interval == 0)) {
                ixx[0] = x - this->sample_interval;
                ixx[1] = x;
                ixx[2] = x + this->sample_interval;
                ixx[3] = x + 10;

                iyy[0] = y - this->sample_interval;
                iyy[1] = y;
                iyy[2] = y + this->sample_interval;
                iyy[3] = y + 10;
            }

            if(i < this->sample_interval) {
                ixx[0] = 0;
                ixx[1] = 0;
            }

            if(j < this->sample_interval) {
                iyy[0] = 0;
                iyy[1] = 0;
            }

            if(i > this->width - this->sample_interval) {
                ixx[2] = floor(this->width / this->sample_interval) * this->sample_interval;
                ixx[3] = floor(this->width / this->sample_interval) * this->sample_interval;
            }

            if(j > this->height - this->sample_interval) {
                iyy[2] = floor(this->height / this->sample_interval) * this->sample_interval;;
                iyy[3] = floor(this->height / this->sample_interval) * this->sample_interval;;
            }

            double p[4][4];
            p[0][0] = this->heights[idx(ixx[0], iyy[0])];
            p[1][0] = this->heights[idx(ixx[1], iyy[0])];
            p[2][0] = this->heights[idx(ixx[2], iyy[0])];
            p[3][0] = this->heights[idx(ixx[3], iyy[0])];

            p[0][1] = this->heights[idx(ixx[0], iyy[1])];
            p[1][1] = this->heights[idx(ixx[1], iyy[1])];
            p[2][1] = this->heights[idx(ixx[2], iyy[1])];
            p[3][1] = this->heights[idx(ixx[3], iyy[1])];

            p[0][2] = this->heights[idx(ixx[0], iyy[2])];
            p[1][2] = this->heights[idx(ixx[1], iyy[2])];
            p[2][2] = this->heights[idx(ixx[2], iyy[2])];
            p[3][2] = this->heights[idx(ixx[3], iyy[2])];

            p[0][3] = this->heights[idx(ixx[0], iyy[3])];
            p[1][3] = this->heights[idx(ixx[1], iyy[3])];
            p[2][3] = this->heights[idx(ixx[2], iyy[3])];
            p[3][3] = this->heights[idx(ixx[3], iyy[3])];

            double xx = double(i % this->sample_interval) / (double)this->sample_interval;
            double yy = double(j % this->sample_interval) / (double)this->sample_interval;

            this->heights[this->idx(i,j)] = this->bicubic_interpolate(p, xx, yy);
        }
    }
}

void Terrain::generate_trees() {
    Shader* shader = new Shader("assets/shaders/tree");

    shader->add_uniform(ShaderUniform::MAT4, "model");
    shader->add_uniform(ShaderUniform::MAT4, "view");
    shader->add_uniform(ShaderUniform::MAT4, "mvp");

    shader->add_attribute(ShaderAttribute::POSITION, "position");
    shader->add_attribute(ShaderAttribute::NORMAL, "normal");

    shader->bind_uniforms_and_attributes();

    ObjectMesh tree(shader, new Mesh("assets/meshes/tree.mesh"));

    PerlinNoiseGenerator pn(10.0f, 10.5f, 5, 2763226322);
    for(unsigned int i=0; i<50; i++) {
        float x = pn.get_random_number() * (float)this->width;
        float y = pn.get_random_number() * (float)this->height;
        float angle = pn.get_random_number() * 2.0f * M_PI;

        this->trees.push_back(tree);
        this->trees.back().set_position(glm::vec3(x, y, this->get_height(x,y)));
        this->trees.back().set_rotation_matrix(glm::rotate(angle, glm::vec3(0,0,1)));
        this->trees.back().static_load();
    }
}

/**
 * @fn          cubic_interpolate
 *
 * @brief       performs a 1D cubic interpolation
 *
 * @return      interpolated value
 */
double Terrain::cubic_interpolate (double p[4], double x) {
    return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

/**
 * @fn          bicubic_interpolate
 *
 * @brief       performs a 2D cubic interpolation
 *
 * @param p     matrix holding boundary values
 * @param x     fractional x coordinate [0,1]
 * @param y     fractional y coordinate [0,1]
 *
 * @return      interpolated value
 */
double Terrain::bicubic_interpolate (double p[4][4], double x, double y) {
    double arr[4];
    arr[0] = cubic_interpolate(p[0], y);
    arr[1] = cubic_interpolate(p[1], y);
    arr[2] = cubic_interpolate(p[2], y);
    arr[3] = cubic_interpolate(p[3], y);
    return cubic_interpolate(arr, x);
}

/**
 * @fn          idx
 *
 * @brief       return the index of the height map
 *
 * @param i     x map coordinate
 * @param j     y map coordinate
 *
 * @return      void
 */
unsigned int Terrain::idx(unsigned int i, unsigned int j) {
    return i + (this->width + 1) * j;
}

float Terrain::get_height(float x, float y) {
    // cast x and y to nearest coordinates
    unsigned int i = (unsigned int)x;
    unsigned int j = (unsigned int)y;

    float modx = fmod(x, 1.0f);
    float mody = fmod(y, 1.0f);

    glm::vec3 p1, p2, p3;
    if(mody > 1.0f - modx) { // right top
        p1 = glm::vec3((float)i+1, (float)j, this->heights[this->idx(i+1, j)]);
        p2 = glm::vec3((float)i+1, (float)j+1, this->heights[this->idx(i+1, j+1)]);
        p3 = glm::vec3((float)i, (float)j+1, this->heights[this->idx(i, j+1)]);
    } else {
        p1 = glm::vec3((float)i, (float)j, this->heights[this->idx(i, j)]);
        p2 = glm::vec3((float)i+1, (float)j, this->heights[this->idx(i+1, j)]);
        p3 = glm::vec3((float)i, (float)j+1, this->heights[this->idx(i, j+1)]);
    }

    glm::vec3 d1 = p2 - p1;
    glm::vec3 d2 = p3 - p1;
    glm::vec3 n = glm::cross(d1, d2);

    float height = glm::dot((p1 - glm::vec3(x, y, 0.0f)), n) / glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), n);

    return height;
}
