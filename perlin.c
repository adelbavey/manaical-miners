#include <pic32mx.h>
#include <stdint.h>
#include "headers/math.h"

//Noise created following this tutorial: http://freespace.virgin.net/hugo.elias/models/m_perlin.htm

#define OCTAVES 3
#define PERSISTENCE 1
#define AMPLITUDE 1
#define LACUNARITY 100

float interpolate(float a, float b, float x){
  float pi_mod;
  float f_unk;

  pi_mod = x * 3.1415927;
  f_unk = (1 - cos(pi_mod)) * 0.5;
  return (a * (1 - f_unk) + b * x);
}

double noise(int x, int y){
  int n;
  n = x + y * 57;
  n = (n << 13) ^ n;
  return (1.0 - ( (n * ((n * n * 15731) + 789221) +  1376312589) & 0x7fffffff) / 1073741824.0);
}

double smooth_noise(double x, double y){
    double corners = ( noise(x-1, y-1)+ noise(x+1, y-1) + noise(x-1, y+1) + noise(x+1, y+1) ) / 16;
    double sides   = ( noise(x-1, y)  + noise(x+1, y)  + noise(x, y-1)  + noise(x, y+1) ) / 8;
    double center  =  noise(x, y) / 4;
    return corners + sides + center;
}

float noise_handler(float x, float y)
{
  int       int_val[2];
  float     frac_val[2];
  float     value[4];
  float     res[2];

  int_val[0] = (int)x;
  int_val[1] = (int)y;
  frac_val[0] = x - int_val[0];
  frac_val[1] = y - int_val[1];
  value[0] = smooth_noise(int_val[0], int_val[1]);
  value[1] = smooth_noise(int_val[0] + 1, int_val[1]);
  value[2] = smooth_noise(int_val[0], int_val[1] + 1);
  value[3] = smooth_noise(int_val[0] + 1, int_val[1] + 1);
  res[0] = interpolate(value[0], value[1], frac_val[0]);
  res[1] = interpolate(value[2], value[3], frac_val[0]);
  return (interpolate(res[0], res[1], frac_val[1]));
}

double perlin_noise(int x, int y){
  double total = 0;
  int frequency = LACUNARITY;
  int amplitude = AMPLITUDE;
  int i;
  for(i = 0; i < OCTAVES; i++){
    total += noise_handler(x * frequency, y * frequency) * amplitude;
    frequency *= LACUNARITY;
    amplitude *= PERSISTENCE;
  }
  return total;
}