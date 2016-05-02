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

#include "object.h"

/*
 *
 * OBJECT CLASS
 *
 */

Object::Object(Shader* _shader, const Mesh* _mesh) {
    /* load the default shader */
    this->shader = _shader;
    this->mesh = _mesh;
    this->model = glm::mat4(1.0f);
    this->scale = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);
}

void Object::parse_uniform(unsigned int id, const float* val) {
    this->shader->link_shader();
    this->shader->set_uniform(id, val);
}

/*
 *
 * ObjectTexturedMesh CLASS
 *
 */

ObjectTexturedMesh::ObjectTexturedMesh(Shader* _shader, const Mesh* _mesh) {
    /* load the default shader */
    this->shader = _shader;
    this->mesh = _mesh;
    this->model = glm::mat4(1.0f);
    this->scale = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);
}

void ObjectTexturedMesh::draw() {
    // first load shader, then draw vertex arrays
    this->shader->link_shader();

    glm::mat4 view = Camera::get().get_view();
    glm::mat4 projection = Camera::get().get_projection();
    this->model = glm::translate(glm::mat4(1.0f), this->position) * this->rotation * this->scale;
    glm::mat4 mvp = projection * view * this->model;

    this->shader->set_uniform(0, &this->model[0][0]);
    this->shader->set_uniform(1, &view[0][0]);
    this->shader->set_uniform(2, &mvp[0][0]);
    this->shader->set_uniform(3, NULL);

    // load the vertex array
    glBindVertexArray(m_vertex_array_object);

    // draw the mesh using the indices
    glDrawElements(GL_TRIANGLES, this->mesh->get_nr_indices(), GL_UNSIGNED_INT, 0);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}

void ObjectTexturedMesh::static_load() {
    // load the mesh into memory
    unsigned int size = this->mesh->get_nr_indices();

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
    glBufferData(GL_ARRAY_BUFFER, this->mesh->get_nr_positions() * 3 * sizeof(float), this->mesh->get_positions_start(), GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(0);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * NORMALS
     */

    // bind a buffer identified by POSITION_VB and interpret this buffer as an array
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[NORMAL_VB]);
    // fill the buffer with data
    glBufferData(GL_ARRAY_BUFFER, this->mesh->get_nr_normals() * 3 * sizeof(float), this->mesh->get_normals_start(), GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(1);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * TEXTURE COORDINATES
     */

    // bind a buffer identified by POSITION_VB and interpret this buffer as an array
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[TEXTURE_COORDINATE_VB]);
    // fill the buffer with data
    glBufferData(GL_ARRAY_BUFFER, size * 2 * sizeof(float), this->mesh->get_texture_coordinates_start(), GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(2);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * INDICES_VB
     */

    // bind a buffer identified by INDICES_VB and interpret this buffer as an array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertex_array_buffers[INDICES_VB]);
    // fill the buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), this->mesh->get_indices_start(), GL_STATIC_DRAW);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}

/*
 *
 * ObjectLines CLASS
 *
 */

ObjectLines::ObjectLines(Shader* _shader, const Mesh* _mesh) {
    /* load the default shader */
    this->shader = _shader;
    this->mesh = _mesh;
    this->model = glm::mat4(1.0f);
    this->scale = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);
}

void ObjectLines::draw() {
    // first load shader, then draw vertex arrays
    this->shader->link_shader();

    glm::mat4 view = Camera::get().get_view();
    glm::mat4 projection = Camera::get().get_projection();
    this->model = glm::translate(glm::mat4(1.0f), this->position) * this->rotation * this->scale;
    glm::mat4 mvp = projection * view * this->model;

    this->shader->set_uniform(0, &this->model[0][0]);
    this->shader->set_uniform(1, &view[0][0]);
    this->shader->set_uniform(2, &mvp[0][0]);
    this->shader->set_uniform(3, NULL);

    // load the vertex array
    glBindVertexArray(m_vertex_array_object);

    // draw the mesh using the indices
    glDrawElements(GL_LINE_LOOP, this->mesh->get_nr_indices(), GL_UNSIGNED_INT, 0);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}

void ObjectLines::static_load() {
    // load the mesh into memory
    unsigned int size = this->mesh->get_nr_indices();

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
    glBufferData(GL_ARRAY_BUFFER, this->mesh->get_nr_positions() * 3 * sizeof(float), this->mesh->get_positions_start(), GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(0);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * INDICES_VB
     */

    // bind a buffer identified by INDICES_VB and interpret this buffer as an array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertex_array_buffers[INDICES_VB]);
    // fill the buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), this->mesh->get_indices_start(), GL_STATIC_DRAW);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}

/*
 *
 * ObjectMesh CLASS
 *
 */

ObjectMesh::ObjectMesh(Shader* _shader, const Mesh* _mesh) {
    /* load the default shader */
    this->shader = _shader;
    this->mesh = _mesh;
    this->model = glm::mat4(1.0f);
    this->scale = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);
}

void ObjectMesh::draw() {
    // first load shader, then draw vertex arrays
    this->shader->link_shader();

    glm::mat4 view = Camera::get().get_view();
    glm::mat4 projection = Camera::get().get_projection();
    this->model = glm::translate(glm::mat4(1.0f), this->position) * this->rotation * this->scale;
    glm::mat4 mvp = projection * view * this->model;

    this->shader->set_uniform(0, &this->model[0][0]);
    this->shader->set_uniform(1, &view[0][0]);
    this->shader->set_uniform(2, &mvp[0][0]);

    // load the vertex array
    glBindVertexArray(m_vertex_array_object);

    // draw the mesh using the indices
    glDrawElements(GL_TRIANGLES, this->mesh->get_nr_indices(), GL_UNSIGNED_INT, 0);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}

void ObjectMesh::static_load() {
    // load the mesh into memory
    unsigned int size = this->mesh->get_nr_indices();

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
    glBufferData(GL_ARRAY_BUFFER, this->mesh->get_nr_positions() * 3 * sizeof(float), this->mesh->get_positions_start(), GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(0);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * NORMALS
     */

    // bind a buffer identified by POSITION_VB and interpret this buffer as an array
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[NORMAL_VB]);
    // fill the buffer with data
    glBufferData(GL_ARRAY_BUFFER, this->mesh->get_nr_normals() * 3 * sizeof(float), this->mesh->get_normals_start(), GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(1);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * INDICES_VB
     */

    // bind a buffer identified by INDICES_VB and interpret this buffer as an array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertex_array_buffers[INDICES_VB]);
    // fill the buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), this->mesh->get_indices_start(), GL_STATIC_DRAW);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}

/*
 *
 * ObjectColoredMesh CLASS
 *
 */

ObjectColoredMesh::ObjectColoredMesh(Shader* _shader, const Mesh* _mesh) {
    /* load the default shader */
    this->shader = _shader;
    this->mesh = _mesh;
    this->model = glm::mat4(1.0f);
    this->scale = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);
}

void ObjectColoredMesh::draw() {
    // first load shader, then draw vertex arrays
    this->shader->link_shader();

    glm::mat4 view = Camera::get().get_view();
    glm::mat4 projection = Camera::get().get_projection();
    this->model = glm::translate(glm::mat4(1.0f), this->position) * this->rotation * this->scale;
    glm::mat4 mvp = projection * view * this->model;

    this->shader->set_uniform(0, &this->model[0][0]);
    this->shader->set_uniform(1, &view[0][0]);
    this->shader->set_uniform(2, &mvp[0][0]);

    // load the vertex array
    glBindVertexArray(m_vertex_array_object);

    // draw the mesh using the indices
    glDrawElements(GL_TRIANGLES, this->mesh->get_nr_indices(), GL_UNSIGNED_INT, 0);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}

void ObjectColoredMesh::static_load() {
    // load the mesh into memory
    unsigned int size = this->mesh->get_nr_indices();

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
    glBufferData(GL_ARRAY_BUFFER, this->mesh->get_nr_positions() * 3 * sizeof(float), this->mesh->get_positions_start(), GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(0);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * NORMALS
     */

    // bind a buffer identified by POSITION_VB and interpret this buffer as an array
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[NORMAL_VB]);
    // fill the buffer with data
    glBufferData(GL_ARRAY_BUFFER, this->mesh->get_nr_normals() * 3 * sizeof(float), this->mesh->get_normals_start(), GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(1);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * COLORS
     */

    // bind a buffer identified by POSITION_VB and interpret this buffer as an array
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[COLOR_VB]);
    // fill the buffer with data
    glBufferData(GL_ARRAY_BUFFER, this->mesh->get_nr_colors() * 3 * sizeof(float), this->mesh->get_colors_start(), GL_STATIC_DRAW);

    // specifies the generic vertex attribute of index 0 to be enabled
    glEnableVertexAttribArray(2);
    // define an array of generic vertex attribute data
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*
     * INDICES_VB
     */

    // bind a buffer identified by INDICES_VB and interpret this buffer as an array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertex_array_buffers[INDICES_VB]);
    // fill the buffer with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), this->mesh->get_indices_start(), GL_STATIC_DRAW);

    // after this command, any commands that use a vertex array will
    // no longer work
    glBindVertexArray(0);
}
