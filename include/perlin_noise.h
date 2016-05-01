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

#ifndef _PERLIN_NOISE_H
#define _PERLIN_NOISE_H

#include <boost/random.hpp>
#include <cmath>
#include <time.h>
#include <stdlib.h>

class PerlinNoiseGenerator{
public:
    PerlinNoiseGenerator(double _a, double _b, unsigned int _itr, unsigned int _seed);

    double get_perlin_noise(int x);

private:
    unsigned int itr;
    unsigned int seed;
    double a;
    double b;

    double noise2(double x);

};

#endif //_PERLIN_NOISE_H
