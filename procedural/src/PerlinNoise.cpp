/*
** Felix Schrynemaekers Project
** ProceduralX
** File description:
** PerlinNoise
*/

#include "PerlinNoise.hpp"
#include <cmath>
#include <vector>

static int perm[512] = {151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
    151,160,137,91,90,15,
};

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float grad(int hash, float x, float y) {
    int h = hash & 7;  
    float u = h < 4 ? x : y; 
    float v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

float PerlinNoise::perlin(float x, float y) {
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    x -= floor(x);
    y -= floor(y);

    float u = fade(x);
    float v = fade(y);

    int A = perm[X] + Y;
    int B = perm[X + 1] + Y;

    return lerp(v, 
                lerp(u, grad(perm[A], x, y), grad(perm[B], x - 1, y)),
                lerp(u, grad(perm[A + 1], x, y - 1), grad(perm[B + 1], x - 1, y - 1))
           );
}