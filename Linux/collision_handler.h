#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <vector>
#include "variables.h"

int pos_new[2];
float pos_new_raster[2];

int collision(int pos[2], float pos_raster[2], int speed[2])
{
  pos_new[0] = pos[0] + speed[0];
  pos_new[1] = pos[1] + speed[1];

  pos_new_raster[0] = (float)pos_new[0] / (float)tile_size[0];
  pos_new_raster[1] = (float)pos_new[1] / (float)tile_size[0];

  if(speed[0] <= 0)
  {
    if(map.data[(int)pos_raster[1] * map.w + (int)pos_new_raster[0]] != 0 || map.data[(int)(pos_raster[1] + 0.9f) * map.w + (int)pos_new_raster[0]] != 0 || map.data[(int)(pos_raster[1] + 1) * map.w + (int)pos_new_raster[0]] != 0 || map.data[(int)(pos_raster[1] + 1.9f) * map.w + (int)pos_new_raster[0]] != 0)
    {
      pos_new_raster[0] = (int)pos_new_raster[0] + 1;
      speed[0] = 0;
    }

    if(pos_new_raster[0] < 0)
    {
      pos_new_raster[0] = (int)pos_new_raster[0];
      speed[0] = 0;
    }
  }
  else
  {
    if(map.data[(int)pos_raster[1] * map.w + (int)pos_new_raster[0] + 1] != 0 || map.data[(int)(pos_raster[1] + 0.9f) * map.w + (int)pos_new_raster[0] + 1] != 0 || map.data[(int)(pos_raster[1] + 1) * map.w + (int)pos_new_raster[0] + 1] != 0 || map.data[(int)(pos_raster[1] + 1.9f) * map.w + (int)pos_new_raster[0] + 1] != 0 || pos_new_raster[0] >= map.w - 1)
    {
      pos_new_raster[0] = (int)pos_new_raster[0];
      speed[0] = 0;
    }
  }

  standing = false;

  if(speed[1] <= 0)
  {
    if(map.data[(int)pos_new_raster[1] * map.w + (int)pos_new_raster[0]] != 0 || map.data[(int)pos_new_raster[1] * map.w + (int)(pos_new_raster[0] + 0.9f)] != 0 || pos[1] < 0)
    {
      pos_new_raster[1] = (int)pos_new_raster[1] + 1;
      speed[1] = 0;
    }
  }
  else
  {
    if(map.data[((int)pos_new_raster[1] + 2) * map.w + (int)pos_new_raster[0]] != 0 || map.data[((int)pos_new_raster[1] + 2) * map.w + (int)(pos_new_raster[0] + 0.9f)] != 0 || (pos_raster[1] + 2) * tile_size[0] >= map.h * tile_size[0])
    {
      pos_new_raster[1] = (int)pos_new_raster[1];
      speed[1] = 0;
      standing = true;
      double_jump = true;
    }
  }

  pos[0] = pos_new_raster[0] * tile_size[0];
  pos[1] = pos_new_raster[1] * tile_size[1];

  return 0;
}

#endif // COLLISION_HANDLER_H
