#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <SDL2/SDL.h>

#include "texture_handler.h"
#include "SDL_Extend.h"

int render_texture_simple(SDL_Renderer *renderer, int id, int x, int y, int w, int h)
{
  if(id >= texture_atlas.size()) return -1;

  SDL_Rect dst_rect;

  dst_rect.x = x;
  dst_rect.y = y;
  dst_rect.w = w;
  dst_rect.h = h;

  SDL_RenderCopy(renderer, texture_atlas[id].texture, NULL, &dst_rect);
  return 0;
}

int render_texture_animated(SDL_Renderer *renderer, int id, int x, int y, int w, int h)
{
  if(id >= texture_atlas.size()) return -1;

  SDL_Rect src_rect;
  SDL_Rect dst_rect;
  Uint32 format;
  int access;

  SDL_QueryTexture(texture_atlas[id].texture, &format, &access, &src_rect.w, &src_rect.h);

  src_rect.w /= texture_atlas[id].frames;
  src_rect.x = src_rect.w * (int)((SDL_GetTicks() / texture_atlas[id].speed) % texture_atlas[id].frames);
  src_rect.y = 0;

  dst_rect.x = x;
  dst_rect.y = y;
  dst_rect.w = w;
  dst_rect.h = h;

  SDL_RenderCopy(renderer, texture_atlas[id].texture, &src_rect, &dst_rect);
  return 0;
}

int render_player(SDL_Renderer* renderer, int x, int y, float index_offset[2], int render_offset[2], int speed[2], int player_color)
{
  int standing, right, left;

  switch(player_color)
  {
	case 0:
		standing = 0;
		right = 1;
		left = 2;
		break;

	case 1:
		standing = 3;
		right = 4;
		left = 5;
		break;

	case 2:
		standing = 6;
		right = 7;
		left = 8;
		break;

	case 3:
		standing = 9;
		right = 10;
		left = 11;
		break;		

  	default:
		return -1;
		break;
  }

  if(speed[0] == 0) if(render_texture_animated(renderer, standing, x - index_offset[0] * tile_size[0], y - index_offset[1] * tile_size[1], player_size[0], player_size[1]) == -1) return -1;
  if(speed[0] > 0) if(render_texture_animated(renderer, right, x - index_offset[0] * tile_size[0], y - index_offset[1] * tile_size[1], player_size[0], player_size[1]) == -1) return -1;
  if(speed[0] < 0) if(render_texture_animated(renderer, left, x - index_offset[0] * tile_size[0], y - index_offset[1] * tile_size[1], player_size[0], player_size[1]) == -1) return -1;

  return 0;
}

int redraw_tile(SDL_Renderer* renderer, Map map, int x, int y, float index_offset[2], int render_offset[2])
{
  int curr_data;

  if(((int)index_offset[1] + y) * map.w + (int)index_offset[0] + x >= map.data.size() || (int)index_offset[0] + x >= map.w || (int)index_offset[1] + y >= map.h)
  {
    printf("Could not access Map data at X: %d   Y: %d   Map Size X: %d   Map Size Y: %d   Map Size Tiles: %d   Index Off X: %f   Index Off Y: %f   Visible X: %.2f   Visible Y: %.2f\n", x, y, map.w, map.h, (int)map.data.size(), index_offset[0], index_offset[1], visible_tiles[0], visible_tiles[1]);
    return -1;
  }

  curr_data = map.data[((int)index_offset[1] + y) * map.w + (int)index_offset[0] + x];

  if(curr_data > num_tile_textures - 1) return -1;
  bool clear = false;

  switch(curr_data)
  {
    case 0:
      clear = true;
	  break;
    case 1:
      render_texture_simple(renderer, 12, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
      break;
    case 2:
      if(player_color == 1) render_texture_simple(renderer, 13, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else clear = true;
	  break;
    case 3:
      if(player_color == 2) render_texture_simple(renderer, 14, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else clear = true;
      break;
    case 4:
      if(player_color == 3) render_texture_simple(renderer, 15, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else clear = true;
      break;
  }

  if(clear)
  {
    clear = false;
    SDL_SetRenderDrawColor(renderer, 63, 63, 63, 255);
    SDL_RenderDrawBox(renderer, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
  }
}

int update_map_player(SDL_Renderer* renderer, float player_pos_raster[2], float index_offset[2], int render_offset[2])
{
  //Update Player Tiles
  if(redraw_tile(renderer, map, (int)player_pos_raster[0], (int)player_pos_raster[1], index_offset, render_offset) == -1) return -1;
  if(redraw_tile(renderer, map, (int)player_pos_raster[0], (int)player_pos_raster[1] + 1, index_offset, render_offset) == -1) return -1;
}

int update_map_no_scroll(SDL_Renderer* renderer, float player_pos_r[2], float player_pos_r_prev[2], int player_speed[2], float index_offset[2], float index_offset_prev[2], int render_offset[2], int render_offset_prev[2])
{
  //Clamp Coordinates to visible space
  float player_pos_raster[2] = {player_pos_r[0] - index_offset[0], player_pos_r[1] - index_offset[1]};
  float player_pos_raster_prev[2] = {player_pos_r_prev[0] - index_offset_prev[0], player_pos_r_prev[1] - index_offset_prev[1]};

  //Update Preious Tiles
  if(redraw_tile(renderer, map, (int)player_pos_raster_prev[0], (int)player_pos_raster_prev[1], index_offset_prev, render_offset_prev) == -1) return -1;
  if(redraw_tile(renderer, map, (int)player_pos_raster_prev[0], (int)player_pos_raster_prev[1] + 1, index_offset_prev, render_offset_prev) == -1) return -1;

  if((float)render_offset_prev[0] - (int)render_offset_prev[0] > 0) if(redraw_tile(renderer, map, (int)player_pos_raster_prev[0] + 1, (int)player_pos_raster_prev[1], index_offset_prev, render_offset_prev) == -1) return -1;
  if((float)render_offset_prev[1] - (int)render_offset_prev[1] > 0) if(redraw_tile(renderer, map, (int)player_pos_raster_prev[0], (int)player_pos_raster_prev[1] + 2, index_offset_prev, render_offset_prev) == -1) return -1;
  if((float)render_offset_prev[0] - (int)render_offset_prev[0] > 0 && (float)render_offset_prev[1] - (int)render_offset_prev[1] > 0) if(redraw_tile(renderer, map, (int)player_pos_raster_prev[0] + 1, (int)player_pos_raster_prev[1] + 2, index_offset_prev, render_offset_prev) == -1) return -1;

  //if((float)render_offset_prev[0] / (float)tile_size[0] > (float)player_speed[0] / (float)tile_size[0]) if(redraw_tile(renderer, map, (int)player_pos_raster_prev[0] + 1, (int)player_pos_raster_prev[1], index_offset_prev, render_offset_prev) == -1) return -1;
  //if((float)render_offset_prev[1] / (float)tile_size[1] > (float)player_speed[1] / (float)tile_size[1]) if(redraw_tile(renderer, map, (int)player_pos_raster_prev[0], (int)player_pos_raster_prev[1] + 2, index_offset_prev, render_offset_prev) == -1) return -1;
  //if((float)render_offset_prev[0] / (float)tile_size[0] > (float)player_speed[0] / (float)tile_size[0] && (float)render_offset_prev[1] / (float)tile_size[1] > (float)player_speed[1] / (float)tile_size[1]) if(redraw_tile(renderer, map, (int)player_pos_raster_prev[0] + 1, (int)player_pos_raster_prev[1] + 2, index_offset_prev, render_offset_prev) == -1) return -1;

  //Update Curent Tiles
  if(redraw_tile(renderer, map, (int)player_pos_raster[0], (int)player_pos_raster[1], index_offset, render_offset) == -1) return -1;
  if(redraw_tile(renderer, map, (int)player_pos_raster[0], (int)player_pos_raster[1] + 1, index_offset, render_offset) == -1) return -1;

  if((float)render_offset[0] - (int)render_offset[0] > 0) if(redraw_tile(renderer, map, (int)player_pos_raster[0] + 1, (int)player_pos_raster[1], index_offset, render_offset) == -1) return -1;
  if((float)render_offset[1] - (int)render_offset[1] > 0) if(redraw_tile(renderer, map, (int)player_pos_raster[0], (int)player_pos_raster[1] + 2, index_offset, render_offset) == -1) return -1;
  if((float)render_offset[0] - (int)render_offset[0] > 0 && (float)render_offset[1] - (int)render_offset[1] > 0) if(redraw_tile(renderer, map, (int)player_pos_raster[0] + 1, (int)player_pos_raster[1] + 2, index_offset, render_offset) == -1) return -1;

  //if((float)render_offset[0] / (float)tile_size[0] > (float)player_speed[0] / (float)tile_size[0]) if(redraw_tile(renderer, map, (int)player_pos_raster[0] + 1, (int)player_pos_raster[1], index_offset, render_offset) == -1) return -1;
  //if((float)render_offset[1] / (float)tile_size[1] > (float)player_speed[1] / (float)tile_size[1]) if(redraw_tile(renderer, map, (int)player_pos_raster[0], (int)player_pos_raster[1] + 2, index_offset, render_offset) == -1) return -1;
  //if((float)render_offset[0] / (float)tile_size[0] > (float)player_speed[0] / (float)tile_size[0] && (float)render_offset[1] / (float)tile_size[1] > (float)player_speed[1] / (float)tile_size[1]) if(redraw_tile(renderer, map, (int)player_pos_raster[0] + 1, (int)player_pos_raster[1] + 2, index_offset, render_offset) == -1) return -1;
}

int update_map_scroll(SDL_Renderer* renderer, float player_pos_r[2], float player_pos_r_prev[2], int player_speed[2], float index_offset[2], float index_offset_prev[2], int render_offset[2], int render_offset_prev[2])
{
  //Clamp Coordinates to visible space
  float player_pos_raster[2] = {player_pos_r[0] - index_offset[0], player_pos_r[1] - index_offset[1]};
  float player_pos_raster_prev[2] = {player_pos_r_prev[0] - index_offset_prev[0], player_pos_r_prev[1] - index_offset_prev[1]};

  //Update Tiles
  for(int j = 0; j < (int)visible_tiles[1]; j++)
  {
    for(int i = 0; i < (int)visible_tiles[0]; i++)
    {
      int curr_data = map.data[((int)index_offset[1] + j) * map.w + (int)index_offset[0] + i];
      int prev_data = map.data[((int)index_offset_prev[1] + j) * map.w + (int)index_offset_prev[0] + i];

      printf("%d   %d\n", index_offset[0], index_offset[1]);
      //printf("Curr X: %d Y: %d D: %d   Prev X: %d Y: %d D: %d\n", index_offset[0] + i, index_offset[1] + j, curr_data, index_offset_prev[0] + i, index_offset_prev[1] + j, prev_data);

      if(curr_data != prev_data)
      {
        if(redraw_tile(renderer, map, i, j, index_offset, render_offset) == -1) return -1;
        if((float)render_offset_prev[0] - (int)render_offset_prev[0] > 0) if(redraw_tile(renderer, map, i, j, index_offset, render_offset) == -1) return -1;
        if((float)render_offset_prev[1] - (int)render_offset_prev[1] > 0) if(redraw_tile(renderer, map, i, j, index_offset, render_offset) == -1) return -1;
        if((float)render_offset_prev[0] - (int)render_offset_prev[0] > 0 && (float)render_offset_prev[1] - (int)render_offset_prev[1] > 0) if(redraw_tile(renderer, map, i, j, index_offset, render_offset) == -1) return -1;
      
        if(redraw_tile(renderer, map, i, j, index_offset, render_offset) == -1) return -1;
        if((float)render_offset[0] - (int)render_offset[0] > 0) if(redraw_tile(renderer, map, i, j, index_offset, render_offset) == -1) return -1;
        if((float)render_offset[1] - (int)render_offset[1] > 0) if(redraw_tile(renderer, map, i, j, index_offset, render_offset) == -1) return -1;
        if((float)render_offset[0] - (int)render_offset[0] > 0 && (float)render_offset[1] - (int)render_offset[1] > 0) if(redraw_tile(renderer, map, i, j, index_offset, render_offset) == -1) return -1;
      }
    }
  }

  update_map_player(renderer, player_pos_raster, index_offset, render_offset);
}

int render_map(SDL_Renderer* renderer, Map map, float index_offset[2], int render_offset[2])
{
  int t_before = SDL_GetTicks();

  for(int j = 0; j < (int)visible_tiles[1]; j++)
  {
    for(int i = 0; i < (int)visible_tiles[0]; i++)
    {
      if(redraw_tile(renderer, map, i, j, index_offset, render_offset) == -1) return -1;
    }
  }

  int t_after = SDL_GetTicks();
  //printf("Time to render one frame: %d Visible X: %.f   Visible Y: %.2f\n", t_after - t_before, visible_tiles[0], visible_tiles[1]);

  return 0;
}

#endif // RENDER_MANAGER_H
