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

#include "perlin_noise.h"

PerlinNoiseGenerator::PerlinNoiseGenerator(double _a, double _b, unsigned int _itr, unsigned int _seed) {
    this->a = _a;
    this->b = _b;
    this->itr = _itr;
    this->seed = _seed;
}

double PerlinNoiseGenerator::get_perlin_noise(int x) {
    double sum = 0.0;
    for(unsigned int i=0; i < this->itr; i++) {
        sum += this->noise2(std::pow(this->b, (double)i) * x) /
                            std::pow(this->a, (double)i);
    }

    return sum;
}

double PerlinNoiseGenerator::noise2(double x) {
   boost::random::mt19937 rng;
   rng.seed(uint32_t(this->seed - x));
   boost::uniform_real<> dist(0.0, 1.0);
   boost::variate_generator<boost::mt19937&, boost::uniform_real<> >
      die(rng, dist);
   return die();
}
