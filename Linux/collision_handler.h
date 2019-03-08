#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <vector>
#include <algorithm>
#include "variables.h"
#include "health_manager.h"

int pos_new[2];
float pos_new_raster[2];

int collision(int pos[2], float pos_raster[2], int speed[2])
{
  pos_new[0] = pos[0] + speed[0];
  pos_new[1] = pos[1] + speed[1];

  pos_new_raster[0] = (float)pos_new[0] / (float)tile_size[0];
  pos_new_raster[1] = (float)pos_new[1] / (float)tile_size[0];

  std::vector<std::vector<int>> except_colors = {{0, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12}, {0, 3, 4, 7, 11, 12}, {0, 2, 4, 6, 10, 12}, {0, 2, 3, 7, 10, 11}};
  std::vector<int> all_exceptions {0, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12};
  std::vector<int> exceptions;

  exceptions.clear();

  for(int i = 0; i < except_colors[player_color].size(); i++)
  {
	exceptions.push_back(except_colors[player_color][i]);
  }

  if(speed[0] <= 0)
  {
    if(std::find(exceptions.begin(), exceptions.end(), map.data[(int)pos_raster[1] * map.w + (int)pos_new_raster[0]]) == exceptions.end() || std::find(exceptions.begin(), exceptions.end(), map.data[(int)(pos_raster[1] + 0.9f) * map.w + (int)pos_new_raster[0]]) == exceptions.end() || std::find(exceptions.begin(), exceptions.end(), map.data[(int)(pos_raster[1] + 1) * map.w + (int)pos_new_raster[0]]) == exceptions.end() || std::find(exceptions.begin(), exceptions.end(), map.data[(int)(pos_raster[1] + 1.9f) * map.w + (int)pos_new_raster[0]]) == exceptions.end())
    {
	  check_damage(map.data[(int)pos_raster[1] * map.w + (int)pos_raster[0] - 1], map.data[((int)pos_raster[1] + 1) * map.w + (int)pos_raster[0] - 1]);
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
    if(std::find(exceptions.begin(), exceptions.end(), map.data[(int)pos_raster[1] * map.w + (int)pos_new_raster[0] + 1]) == exceptions.end() || std::find(exceptions.begin(), exceptions.end(), map.data[(int)(pos_raster[1] + 0.9f) * map.w + (int)pos_new_raster[0] + 1]) == exceptions.end() || std::find(exceptions.begin(), exceptions.end(), map.data[(int)(pos_raster[1] + 1) * map.w + (int)pos_new_raster[0] + 1]) == exceptions.end() || std::find(exceptions.begin(), exceptions.end(), map.data[(int)(pos_raster[1] + 1.9f) * map.w + (int)pos_new_raster[0] + 1]) == exceptions.end() || pos_new_raster[0] >= map.w - 1)
    {
	  check_damage(map.data[(int)pos_raster[1] * map.w + (int)pos_raster[0] + 1], map.data[((int)pos_raster[1] + 1) * map.w + (int)pos_raster[0] + 1]);
      pos_new_raster[0] = (int)pos_new_raster[0]; 
      speed[0] = 0;
    }
  }

  standing = false;

  if(speed[1] <= 0)
  {
    if(std::find(exceptions.begin(), exceptions.end(), map.data[((int)pos_new_raster[1] - 1) * map.w + (int)pos_new_raster[0]]) == exceptions.end() || std::find(exceptions.begin(), exceptions.end(), map.data[((int)pos_new_raster[1] - 1) * map.w + (int)(pos_new_raster[0] + 0.9f)]) == exceptions.end() || pos[1] < 0)
    {
	  check_damage(map.data[((int)pos_raster[1] - 1) * map.w + (int)pos_raster[0]], map.data[(int)pos_raster[1] * map.w + (int)pos_raster[0]]);
      pos_new_raster[1] = (int)pos_new_raster[1] + 1;
      speed[1] = 0;
    }
  }
  else
  {
    if(std::find(exceptions.begin(), exceptions.end(), map.data[((int)pos_new_raster[1] + 2) * map.w + (int)pos_new_raster[0]]) == exceptions.end() || std::find(exceptions.begin(), exceptions.end(), map.data[((int)pos_new_raster[1] + 2) * map.w + (int)(pos_new_raster[0] + 0.9f)]) == exceptions.end() || (pos_raster[1] + 2) * tile_size[0] >= map.h * tile_size[0])
    {
	  check_damage(map.data[((int)pos_raster[1] + 1) * map.w + (int)pos_raster[0]], map.data[((int)pos_raster[1] + 2) * map.w + (int)pos_raster[0]]);
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
