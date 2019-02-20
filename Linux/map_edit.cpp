//Map Editor for Chromansion

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "variables.h"
#include "texture_handler.h"
#include "SDL_Extend.h"
#include "render_manager.h"
#include "map_handler.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

const Uint8 *key_state = SDL_GetKeyboardState(NULL);
unsigned int last_time = 0, current_time;
int unused;

int frame_counter = 0;
bool quit = false;

int camera_pos[2] = {0, 0};
std::string path = "data/maps/";

float index_offset[2] = {0, 0};
int render_offset[2] = {0, 0};

int mouse_pos[2] = {0, 0};
int mouse_raster[2] = {0, 0};
int prev_mouse_raster[2] = {0, 0};

int brush_id = 0;

bool saving = false;
bool draw = false;

std::fstream map_file;

void screen_init()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);

  window = SDL_CreateWindow("Chromansion Map Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_size[0], screen_size[1], fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(renderer, 5, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);
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

void change_size();
void change_spawn();

int init()
{
  screen_init();
  load_textures(renderer);

  std::string tmp;

  printf("Enter the name of the map: ");
  std::cin >> tmp;

  path.append(tmp);
  path.append(".map");

  std::fstream map_file_test(path.c_str());
  if(map_file_test.good()) map = load_map(path.c_str()); 
  else
  {
  	change_size();
  	change_spawn();
  }

  index_offset[0] = camera_pos[0];
  index_offset[1] = camera_pos[1];

  if(index_offset[0] < 0) index_offset[0] = 0;
  if(index_offset[1] < 0) index_offset[1] = 0;
  if(index_offset[0] > (map.w < visible_tiles[0] ? map.w : map.w - visible_tiles[0])) index_offset[0] = map.w < visible_tiles[0] ? map.w : map.w - visible_tiles[0];
  if(index_offset[1] > (map.h < visible_tiles[1] ? map.h : map.h - visible_tiles[1])) index_offset[1] = map.h < visible_tiles[1] ? map.h : map.h - visible_tiles[1];

  render_offset[0] = (int)((index_offset[0] - (int)index_offset[0]) * tile_size[0]);
  render_offset[1] = (int)((index_offset[1] - (int)index_offset[1]) * tile_size[1]);

  render_map(renderer, map, index_offset, render_offset, true);
  SDL_RenderPresent(renderer);

  return 0;
}

void end()
{
  screen_end();
}

void change_size()
{
  std::string tmp;

  printf("Enter the new width of the map: ");
  std::cin >> tmp;

  map.w = std::stoi(tmp);

  printf("Enter the new height of the map: ");
  std::cin >> tmp;

  map.h = std::stoi(tmp);

  visible_tiles[0] = screen_size[0] / tile_size[0];
  visible_tiles[1] = screen_size[1] / tile_size[1];

  if(map.w < visible_tiles[0]) visible_tiles[0] = map.w;
  if(map.h < visible_tiles[1]) visible_tiles[1] = map.h;

  render_map(renderer, map, index_offset, render_offset, true);

  map.data.resize(map.w * map.h, 0);
}

void change_spawn()
{
  std::string tmp;

  printf("Enter the new X coordinate for the spawn: ");
  std::cin >> tmp;

  map.sx = std::stoi(tmp);

  printf("Enter the new Y coordinate for the spawn: ");
  std::cin >> tmp;

  map.sy = std::stoi(tmp);
}

void change_brush()
{
  std::string tmp;

  printf("Enter the ID of the tile you want to use: ");
  std::cin >> tmp;

  brush_id = std::stoi(tmp);

  if(brush_id < 0 || brush_id > num_tile_textures - 1)
  {
    printf("Error: Invalid ID\n");
    change_brush();
  }
}

void draw_screen()
{
  /*if(camera_pos[0] != index_offset[0] || camera_pos[1] != index_offset[1])*/ render_map(renderer, map, index_offset, render_offset, true);

  index_offset[0] = camera_pos[0];
  index_offset[1] = camera_pos[1];

  if(index_offset[0] < 0) index_offset[0] = 0;
  if(index_offset[1] < 0) index_offset[1] = 0;
  if(index_offset[0] > (map.w < visible_tiles[0] ? map.w : map.w - visible_tiles[0])) index_offset[0] = map.w < visible_tiles[0] ? map.w : map.w - visible_tiles[0];
  if(index_offset[1] > (map.h < visible_tiles[1] ? map.h : map.h - visible_tiles[1])) index_offset[1] = map.h < visible_tiles[1] ? map.h : map.h - visible_tiles[1];

  render_offset[0] = (int)((index_offset[0] - (int)index_offset[0]) * tile_size[0]);
  render_offset[1] = (int)((index_offset[1] - (int)index_offset[1]) * tile_size[1]);

  mouse_raster[0] = mouse_pos[0] / tile_size[0];
  mouse_raster[1] = mouse_pos[1] / tile_size[1];

  if(mouse_raster[0] < map.w && mouse_raster[1] < map.h && draw) map.data[(mouse_raster[1] + index_offset[1]) * map.w + mouse_raster[0] + index_offset[0]] = brush_id;

  redraw_tile(renderer, map, mouse_raster[0], mouse_raster[1], index_offset, render_offset, true);
  redraw_tile(renderer, map, prev_mouse_raster[0], prev_mouse_raster[1], index_offset, render_offset, true);

  SDL_SetRenderDrawColor(renderer, 255, 128, 128, 255);

  SDL_RenderDrawBox(renderer, mouse_raster[0] * tile_size[0], mouse_raster[1] * tile_size[1], tile_size[0], 2);
  SDL_RenderDrawBox(renderer, mouse_raster[0] * tile_size[0], mouse_raster[1] * tile_size[1], 2, tile_size[1]);
  SDL_RenderDrawBox(renderer, mouse_raster[0] * tile_size[0], (mouse_raster[1] + 1) * tile_size[1] - 2, tile_size[0], 2);
  SDL_RenderDrawBox(renderer, (mouse_raster[0] + 1) * tile_size[0] - 2, mouse_raster[1] * tile_size[1], 2, tile_size[1]);

  prev_mouse_raster[0] = mouse_raster[0];
  prev_mouse_raster[1] = mouse_raster[1];

  SDL_RenderPresent(renderer);
}

int main()
{
  if(init() == -1) return -1;
  draw_screen();

  while(!quit)
  {
    current_time = SDL_GetTicks();
 		if(current_time > last_time + 1)
 		{
 			last_time = current_time;

      SDL_GetMouseState(&mouse_pos[0], &mouse_pos[1]);
      SDL_PollEvent(&event);

      switch(event.type)
      {
        case SDL_QUIT:
          quit = true;
          break;
      }

      if(key_state[SDL_SCANCODE_ESCAPE] == 1) quit = true;

	  if(key_state[SDL_SCANCODE_SPACE] == 1) draw = true;
	  else if(key_state[SDL_SCANCODE_SPACE] == 0) draw = false;

      if(key_state[SDL_SCANCODE_W] == 1 && camera_pos[1] >= 0) camera_pos[1]--;
      if(key_state[SDL_SCANCODE_A] == 1 && camera_pos[0] >= 0) camera_pos[0]--;
      if(key_state[SDL_SCANCODE_S] == 1 && key_state[SDL_SCANCODE_LCTRL] == 0 && camera_pos[1] <= map.h - visible_tiles[1]) camera_pos[1]++;
      if(key_state[SDL_SCANCODE_D] == 1 && camera_pos[0] <= map.w - visible_tiles[0]) camera_pos[0]++;

      if(key_state[SDL_SCANCODE_Q] == 1) change_size();
      if(key_state[SDL_SCANCODE_E] == 1) change_spawn();
      if(key_state[SDL_SCANCODE_R] == 1) change_brush();

      if(key_state[SDL_SCANCODE_S] == 1 && key_state[SDL_SCANCODE_LCTRL] == 1 && !saving)
      {
        write_map(path.c_str(), map);
        printf("Saved to %s\n", path.c_str());
        saving = true;
      }
      else if(key_state[SDL_SCANCODE_S] == 0 || key_state[SDL_SCANCODE_LCTRL] == 0) saving = false;

      draw_screen();
      frame_counter++;
    }
  }

  end();
}
