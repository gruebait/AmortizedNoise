/// \file infiniteperlin.cpp
/// \brief Code file for 2D infinite Perlin noise functions.
///
/// Infinite  Perlin noise uses MurmurHash3 to get the gradients at
/// integer points.

// Copyright Ian Parberry, January 2014.
//
// This file is made available under the GNU All-Permissive License.
//
// Copying and distribution of this file, with or without modification,
// are permitted in any medium without royalty provided the copyright
// notice and this notice are preserved.  This file is offered as-is,
// without any warranty.
//
// Created by Ian Parberry, January 2014.
// Last updated May 7, 2014.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Common.h"
#include "MurmurHash3.h"

/// A 2D hash function.
/// Hash two dimensions into a single unsigned int using MurmurHash.
/// \param x X coordinate of value to be hashed.
/// \param y Y coordinate of value to be hashed.
/// \return Hash of (x, y).

unsigned int h2(unsigned int x, unsigned int y){
  unsigned int result;
  unsigned long long key = ((unsigned long long)x<<32) | y;
  MurmurHash3_32(&key, 8, 0, &result); //do the heavy lifting
  return result;
} //h2

/// Compute a single octave of infinite 2D noise at a single point.
/// \param vec Point at which to evaluate noise.
/// \return Noise value between -1.0 and 1.0.

float infinitenoise2(float vec[2]){
  int bx0, bx1, by0, by1;
  float rx0, rx1, ry0, ry1, q[2], sx, sy, a, b, t, u, v;

  setup(0, bx0,bx1, rx0,rx1);
  setup(1, by0,by1, ry0,ry1);

  float b0 = (float)h2(bx0,   by0), b1 = (float)h2(bx0,   by0+1), 
        b2 = (float)h2(bx0+1, by0), b3 = (float)h2(bx0+1, by0+1);

  sx = s_curve(rx0);
  sy = s_curve(ry0);

  q[0] = cos(b0); q[1] = sin(b0); u = at2(rx0, ry0);
  q[0] = cos(b2); q[1] = sin(b2); v = at2(rx1, ry0);
  a = lerp(sx, u, v);

  q[0] = cos(b1); q[1] = sin(b1); u = at2(rx0, ry1);
  q[0] = cos(b3); q[1] = sin(b3); v = at2(rx1, ry1);
  b = lerp(sx, u, v);

  return lerp(sy, a, b);
} //infinitenoise2

/// Compute turbulence, also known as 1/f noise.
/// \param x X coordinate.
/// \param y Y coordinate.
/// \param alpha Persistence.
/// \param beta Lacunarity.
/// \param n Side of square grid.
/// \return Scale factor required to bring it down to range [-1, 1].

float InfinitePerlinNoise2D(float x, float y, float alpha, float beta, int n){
  float sum=0.0f, p[2], scale=1.0f;
  p[0] = x; p[1] = y;

  for(int i=0; i<n; i++){
    sum += infinitenoise2(p)*scale;
    scale *= alpha;
    p[0] *= beta;	p[1] *= beta;
  } //for

  return sum/(2.0f - scale);
} //InfinitePerlinNoise2D
