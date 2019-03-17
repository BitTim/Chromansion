#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>
#include "map_handler.h"

int screen_size[2];
bool fullscreen;

bool quit = false;

int tile_size[2] = {32, 32};
int player_size[2] = {tile_size[0], tile_size[1] * 2};
float visible_tiles[2] = {(float)screen_size[0] / (float)tile_size[0], (float)screen_size[1] / (float)tile_size[1]};

int num_tile_textures = 13;

bool standing = false;
bool jumping = false;
bool double_jump = true;
bool sprint = false;

int direction = 1;

int player_max_speed[2] = {7, 22};
int gravity = 2;
int drag = 1;

int player_color = 0;
int player_health = 100;
int player_max_health = 100;

bool player_invincible = false;
bool game_over = false;

Map map;

#endif // VARIABLES_H
