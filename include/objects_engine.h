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

#ifndef _OBJECTS_ENGINE
#define _OBJECTS_ENGINE

#include <vector>

#include "mesh.h"
#include "shader.h"
#include "object.h"
#include "texture_manager.h"
#include "terrain.h"

/**
 * @class ObjectsEngine class
 *
 * @brief class handling the objects_engine
 *
 */
class ObjectsEngine {
private:
    std::vector<Mesh*> meshes;
    std::vector<Shader*> shaders;

    std::vector<Object*> objects;

public:

    /**
     * @fn          get
     *
     * @brief       get a reference to the objects_engine
     *
     * @return      reference to the objects_engine object (singleton pattern)
     */
    static ObjectsEngine& get() {
        static ObjectsEngine objects_engine_instance;
        return objects_engine_instance;
    }

    void update(double dt);

    void draw();

    unsigned int add_shader(const std::string& filename);

    unsigned int add_mesh(const std::string& filename);

private:
    /**
     * @brief       objects_engine constructor
     *
     * @return      objects_engine instance
     */
    ObjectsEngine();

    ObjectsEngine(ObjectsEngine const&)          = delete;
    void operator=(ObjectsEngine const&)  = delete;
};

#endif // _OBJECTS_ENGINE
