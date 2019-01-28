#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <vector>
#include "variables.h"

int collision(int pos[2], float player_pos_raster[2], int speed[2])
{
  int map_dat_head_left = map.data[(int)player_pos_raster[1] * map.w + (int)player_pos_raster[0]];
  int map_dat_head_right = 0;

  int map_dat_feet_left = map.data[(int)(player_pos_raster[1] + 1) * map.w + (int)player_pos_raster[0]];
  int map_dat_feet_right = 0;

  int map_dat_floor_left = map.data[(int)(player_pos_raster[1] + 2) * map.w + (int)player_pos_raster[0]];
  int map_dat_floor_right = 0;

  if((direction == -1 ? 1 - player_pos_raster[0] - (int)player_pos_raster[0] : player_pos_raster[0] - (int)player_pos_raster[0]))
  {
    map_dat_head_right = map.data[(int)player_pos_raster[1] * map.w + (int)player_pos_raster[0] + 1];
    map_dat_feet_right = map.data[(int)(player_pos_raster[1] + 1) * map.w + (int)player_pos_raster[0] + 1];
     map_dat_floor_right =map.data[(int)(player_pos_raster[1] + 2) * map.w + (int)player_pos_raster[0] + 1];
  }
  else
  {
    map_dat_head_right = map_dat_head_left;
    map_dat_feet_right = map_dat_feet_left;
    map_dat_floor_right = map_dat_floor_left;
  }

  if(map_dat_head_right != 0 || map_dat_feet_right != 0)
  {
    pos[0] -= speed[0];
    speed[0] = 0;
    //pos[0] = (int)player_pos_raster[0] * tile_size[0];
  }

  if(map_dat_head_left != 0 || map_dat_feet_left != 0)
  {
    pos[0] -= speed[0];
    speed[0] = 0;
    //pos[0] = ((int)player_pos_raster[0] + 1) * tile_size[0];
  }

  if(jumping && (map_dat_head_left != 0 || map_dat_head_right != 0))
  {
    jumping = false;
    pos[1] -= speed[1];
    speed[1] = 0;
  }

  if(!jumping && (map_dat_floor_left != 0 || map_dat_floor_right != 0))
  {
    standing = true;
    double_jump = true;
    pos[1] = (int)player_pos_raster[1] * tile_size[1];
    speed[1] = 0;
  }
  else
  {
    standing = false;

    if(speed[1] != player_max_speed[1])
    {
      speed[1] += gravity;
    }

    if(speed[1] > 0)
    {
      jumping = false;
    }
  }

  if(jumping) standing = false;

  return 0;
}

#endif // COLLISION_HANDLER_H
