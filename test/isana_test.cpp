/*************************************************************************
 *
 *  This file is part of REAX.
 *
 *  Authors: Ivo Filot <i.a.w.filot@tue.nl>
 *           Emiel Hensen <e.j.m.hensen@tue.nl>
 *
 * See http://www.boost.org/libs/test for the test library home page.
 *
 * (C) Copyright 2016 Inorganic Materials Chemistry
 *
 ************************************************************************/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE reax_test
#include <boost/test/unit_test.hpp>

#include "mesh.h"

// basic check to see if the test module works
BOOST_AUTO_TEST_CASE(universeInOrder) {
    BOOST_TEST_MESSAGE( "Testing test framework..." );
    BOOST_CHECK((2 + 2) == 4);
}

// basic check to see if the test module works
BOOST_AUTO_TEST_CASE(meshtest1) {
    BOOST_TEST_MESSAGE( "Testing Reading .mesh Mesh..." );
   Mesh mesh("./assets/meshes/turbine.mesh");
   BOOST_CHECK(mesh.get_nr_positions() == 435);
   BOOST_CHECK(mesh.get_nr_normals() == 435);
   BOOST_CHECK(mesh.get_nr_texture_coordinates() == 435);
}

// basic check to see if the test module works
BOOST_AUTO_TEST_CASE(meshtest2) {
    BOOST_TEST_MESSAGE( "Testing Reading .x Mesh..." );
    Mesh mesh("./assets/meshes/turbine.x");
    BOOST_CHECK(mesh.get_nr_positions() == 435);
    BOOST_CHECK(mesh.get_nr_normals() == 435);
    BOOST_CHECK(mesh.get_nr_texture_coordinates() == 435);

    const Armature* armature = mesh.get_armature();
    BOOST_CHECK(armature->get_nr_bones() == 3);
}
