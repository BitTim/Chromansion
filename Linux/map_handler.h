#ifndef MAP_HANDLER_H
#define MAP_HANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Map
{
  unsigned char w;
  unsigned char h;
  std::vector<unsigned char> data;
};

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

  char tmp;
  for(int i = 0; i < map.w * map.h; i++)
  {
    map_file.read((char*)&tmp, 1);
    map.data.push_back(tmp);
  }

  map_file.close();
  return map;
}

#endif // MAP_HANDLER_H
