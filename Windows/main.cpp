//Chromatic v0.1.0
//Copyright (c) BitTim 2019
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "variables.h"
#include "texture_handler.h"
#include "SDL_Extend.h"
#include "render_manager.h"
#include "map_handler.h"
#include "collision_handler.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

const Uint8 *key_state = SDL_GetKeyboardState(NULL);
unsigned int last_time = 0, current_time;
int unused;

int frame_counter = 0;
bool quit = false;

bool space_pressed = false;

int player_pos[2] = {0, 0};
float player_pos_raster[2] = {0, 0};
float player_pos_raster_old[2] = {0, 0};

float camera_pos[2] = {player_pos_raster[0], player_pos_raster[1]};

int player_speed[2] = {0, 0};
int player_acceleration = 1;

int collision_err = 0;

int index_offset[2] = {};
int render_offset[2] = {};

void screen_init()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);

  window = SDL_CreateWindow("Chromansion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_size[0], screen_size[1], fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
}

void screen_end()
{
  SDL_SetWindowFullscreen(window, 0);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}

int init()
{
  screen_init();
  load_textures(renderer);
  map = load_map("data/maps/test.map");
  if(map.w == -1) return -1;

  player_pos[0] = map.sx * tile_size[0];
  player_pos[1] = (map.h  - map.sy - 2) * tile_size[1];

  player_pos_raster[0] = (float)player_pos[0] / (float)tile_size[0];
  player_pos_raster[1] = (float)player_pos[1] / (float)tile_size[1];

  index_offset[0] = camera_pos[0] - (float)visible_tiles[0] / 2.0f;
  index_offset[1] = camera_pos[1] - (float)visible_tiles[0] / 2.0f;

  if (index_offset[0] < 0) index_offset[0] = 0;
  if (index_offset[1] < 0) index_offset[1] = 0;
  if (index_offset[0] > map.w - visible_tiles[0]) index_offset[0] = map.w - visible_tiles[0];
  if (index_offset[1] > map.h - visible_tiles[1]) index_offset[1] = map.h - visible_tiles[1];

  render_offset[0] = (int)((index_offset[0] - (int)index_offset[0]) * tile_size[0]);
  render_offset[1] = (int)((index_offset[1] - (int)index_offset[1]) * tile_size[1]);

  SDL_SetRenderDrawColor(renderer, 63, 63, 63, 255);
  SDL_RenderClear(renderer);

  if(render_map(renderer, map, index_offset, render_offset) == -1) return -1;

  SDL_RenderPresent(renderer);
  return 0;
}

void end()
{
  screen_end();
}

int draw_screen()
{
  if(update_partial_map(renderer, player_pos, player_pos_raster, player_speed, index_offset, render_offset) == -1) return -1;
  if(render_player(renderer, player_pos[0], player_pos[1], render_offset, player_speed) == -1) return -1;

  SDL_RenderPresent(renderer);

  return 0;
}

int update()
{
  if(frame_counter >= 10)
  {
    frame_counter = 0;

    if(sprint) player_max_speed[0] = 10;
    if(!sprint) player_max_speed[0] = 5;

    if(key_state[SDL_SCANCODE_A] == 1)
    {
      if(player_speed[0] > -player_max_speed[0])
      {
        player_speed[0] -= player_acceleration;
      }
      else if(player_speed[0] < -player_max_speed[0])
      {
        player_speed[0] += player_acceleration;
      }
    }
    else if(key_state[SDL_SCANCODE_A] == 0)
    {
      if(player_speed[0] < 0)
      {
        player_speed[0] += player_acceleration;
      }
    }

    if(key_state[SDL_SCANCODE_D] == 1)
    {
      if(player_speed[0] < player_max_speed[0])
      {
        player_speed[0] += player_acceleration;
      }
      else if(player_speed[0] > player_max_speed[0])
      {
        player_speed[0] -= player_acceleration;
      }
    }
    else if(key_state[SDL_SCANCODE_D] == 0)
    {
      if(player_speed[0] > 0)
      {
        player_speed[0] -= player_acceleration;
      }
    }

    if(player_speed[1] < player_max_speed[1]) player_speed[1] += gravity;
    if(player_speed[1] > 0) jumping = false;

    collision_err = collision(player_pos, player_pos_raster, player_speed);
    if(collision_err == -1) printf("Collision Failed\n");

    player_pos_raster[0] = (float)player_pos[0] / (float)tile_size[0];
    player_pos_raster[1] = (float)player_pos[1] / (float)tile_size[1];

    camera_pos[0] = player_pos_raster[0];
    camera_pos[1] = player_pos_raster[1];

    index_offset[0] = camera_pos[0] - (float)visible_tiles[0] / 2.0f;
		index_offset[1] = camera_pos[1] - (float)visible_tiles[0] / 2.0f;

		if (index_offset[0] < 0) index_offset[0] = 0;
		if (index_offset[1] < 0) index_offset[1] = 0;
		if (index_offset[0] > map.w - visible_tiles[0]) index_offset[0] = map.w - visible_tiles[0];
    if (index_offset[1] > map.h - visible_tiles[1]) index_offset[1] = map.h - visible_tiles[1];

    render_offset[0] = (int)((index_offset[0] - (int)index_offset[0]) * tile_size[0]);
    render_offset[1] = (int)((index_offset[1] - (int)index_offset[1]) * tile_size[1]);

    if(draw_screen() == -1) return -1;
  }

  return 0;
}

int WinMain(int argc, char* argv[])
{
  if(init() == -1) return -1;

  while(!quit)
  {
    current_time = SDL_GetTicks();
 		if(current_time > last_time + 1)
 		{
 			last_time = current_time;
      SDL_PollEvent(&event);

      switch(event.type)
      {
        case SDL_QUIT:
          quit = true;
          break;
      }

      if(key_state[SDL_SCANCODE_ESCAPE] == 1)
      {
        quit = true;
      }

      if(key_state[SDL_SCANCODE_SPACE] == 1 && !space_pressed)
      {
        space_pressed = true;
        if(standing || double_jump)
        {
          if(!standing) double_jump = false;
          player_speed[1] = -player_max_speed[1];
        }
        jumping = true;
      }
      else if(key_state[SDL_SCANCODE_SPACE] == 0) space_pressed = false;

      if(key_state[SDL_SCANCODE_LSHIFT] == 1) sprint = true;
      else sprint = false;

      if(update() == -1) return -1;
      frame_counter++;
    }
  }

  end();
}
