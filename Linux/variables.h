#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>
#include "map_handler.h"

int screen_size[2] = {1366, 768};
bool fullscreen = false;

int tile_size[2] = {32, 32};
int player_size[2] = {tile_size[0], tile_size[1] * 2};

int num_tile_textures = 2;

bool standing = false;
bool jumping = false;
bool double_jump = true;
bool sprint = false;

int direction = 1;

int player_max_speed[2] = {7, 22};
int gravity = 2;

Map map;

#endif // VARIABLES_H
