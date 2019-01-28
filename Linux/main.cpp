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

int spawn_point[2] = {1 * tile_size[0], 19 * tile_size[1]};
int player_pos[2] = {spawn_point[0], spawn_point[1]};
float player_pos_raster[2] = {(float)spawn_point[0] / (float)tile_size[0], (float)spawn_point[1] / (float)tile_size[1]};

int player_speed[2] = {0, 0};
int player_acceleration = 1;

int collision_err = 0;

void screen_init()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);

  window = SDL_CreateWindow("Chromatic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_size[0], screen_size[1], fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
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
  map = load_map("data/maps/debug.map");

  if(map.w == -1) return -1;
  return 0;
}

void end()
{
  screen_end();
}

int draw_screen()
{
  SDL_SetRenderDrawColor(renderer, 63, 63, 63, 255);
  SDL_RenderClear(renderer);

  if(render_map(renderer, map) == -1) return -1;
  if(render_player(renderer, player_pos[0], player_pos[1], player_speed) == -1) return -1;

  SDL_RenderPresent(renderer);

  return 0;
}

int update()
{
  if(frame_counter >= 10)
  {
    frame_counter = 0;

    collision_err = collision(player_pos, player_pos_raster, player_speed);
    if(collision_err == -1) printf("Collision Failed\n");

    if(sprint) player_max_speed[0] = 10;
    if(!sprint) player_max_speed[0] = 5;

    if(key_state[SDL_SCANCODE_A] == 1)
    {
      direction = -1;

      if(player_pos[0] > 0)
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
      else if(player_pos[0] <= 0)
      {
        player_speed[0] = 0;
        player_pos[0] = 0;
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
      direction = 1;

      if(player_pos[0] + player_size[0] / 2 < screen_size[0] - player_size[0] / 2 - 1)
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
      else if(player_pos[0] + player_size[0] >= screen_size[0] - player_size[0] / 2 - 1)
      {
        player_speed[0] = 0;
        player_pos[0] = screen_size[0] - player_size[0] - 1;
      }
    }
    else if(key_state[SDL_SCANCODE_D] == 0)
    {
      if(player_speed[0] > 0)
      {
        player_speed[0] -= player_acceleration;
      }
    }

    player_pos[0] += player_speed[0];
    player_pos[1] += player_speed[1];

    player_pos_raster[0] = (float)player_pos[0] / (float)tile_size[0];
    player_pos_raster[1] = (float)player_pos[1] / (float)tile_size[1];

    if(draw_screen() == -1) return -1;
  }

  return 0;
}

int main()
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
        if(!jumping && (standing || double_jump))
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
