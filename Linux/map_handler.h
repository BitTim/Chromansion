#ifndef MAP_HANDLER_H
#define MAP_HANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "powerup_handler.h"
#include "door_manager.h"
#include "gui_handler.h"

struct Map
{
  unsigned char w;
  unsigned char h;
  unsigned char sx;
  unsigned char sy;
  std::vector<unsigned char> data;
  std::vector<int> colors;
  std::vector<door> doors;
  std::vector<pup> powups;
};

int write_map(const char* path, Map map)
{
  std::ofstream map_file;
  map_file.open(path, std::fstream::binary);

  char sign_a = 'M';
  char sign_b = 'P';

  map_file << sign_a << sign_b << map.w << map.h << map.sx << map.sy;

  for(int i = 0; i < map.w * map.h; i++)
  {
    map_file << map.data[i];
  }

  for(pup pow : map.powups)
  {
	map_file << pow.x << pow.y << pow.type;
  }

  map_file.close();
  return 0;
}

Map load_map(const char* path)
{
  Map err_map;

  err_map.w = -1;

  std::ifstream map_file;
  map_file.open(path, std::fstream::binary);

  char sign[2];
  map_file.read(sign, 2);

  if(sign[0] != 0x4d || sign[1] != 0x50) return err_map;

  Map map;
  map_file.read((char*)&map.w, 1);
  map_file.read((char*)&map.h, 1);

  map_file.read((char*)&map.sx, 1);
  map_file.read((char*)&map.sy, 1);

  char tmp;
  for(int i = 0; i < map.w * map.h; i++)
  {
    map_file.read((char*)&tmp, 1);
    map.data.push_back(tmp);
  }

  int num_cols;

  map_file.read((char*)&tmp, 1);
  num_cols = (int)tmp;

  for(int i = 0; i < num_cols; i++)
  {
	map_file.read((char*)&tmp, 1);
	map.colors.push_back((int)tmp);
  }

  int num_doors;

  map_file.read((char*)&tmp, 1);
  num_doors = (int)tmp;

  char door_x, door_y, door_id;
  door tmp_door;

  for(int i = 0; i < num_doors; i++)
  {
	map_file.read((char*)&door_x, 1);
	map_file.read((char*)&door_y, 1);
	map_file.read((char*)&door_id, 1);

	tmp_door = {(int)door_x, (int)door_y, (int)door_id};
	map.doors.push_back(tmp_door);
  }

  bool eof = false;
  char tmp_x, tmp_y, tmp_type;
  char tmp_vis;
  pup tmp_pup;

  while(!eof)
  {
	map_file.read((char*)&tmp_x, 1);
	map_file.read((char*)&tmp_y, 1);
	map_file.read((char*)&tmp_type, 1);
	map_file.read((char*)&tmp_vis, 1);
	
	if(map_file.eof()) eof = true;
	else
	{
		tmp_pup = {(int)tmp_x, (int)tmp_y, (int)tmp_type, (bool)tmp_vis};
		map.powups.push_back(tmp_pup);
	}
  }

  map_file.close();
  return map;
}

#endif // MAP_HANDLER_H
