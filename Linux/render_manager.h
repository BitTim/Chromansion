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

int render_player(SDL_Renderer* renderer, int x, int y, int offset[2], int speed[2])
{
  if(speed[0] == 0) if(render_texture_animated(renderer, 0, x - offset[0], y - offset[1], player_size[0], player_size[1]) == -1) return -1;
  if(speed[0] > 0) if(render_texture_animated(renderer, 1, x - offset[0], y - offset[1], player_size[0], player_size[1]) == -1) return -1;
  if(speed[0] < 0) if(render_texture_animated(renderer, 2, x - offset[0], y - offset[1], player_size[0], player_size[1]) == -1) return -1;
  return 0;
}

int redraw_tile(SDL_Renderer* renderer, Map map, int x, int y, int index_offset[2], int render_offset[2])
{
  if((y + index_offset[1]) * map.w + x + index_offset[0] >= map.data.size())
  {
    printf("Could not access Map data at X: %d   Y: %d   Map Size: %d   Index Off X: &d   Index Off Y: %d\n", x, y, map.data.size(), index_offset[0], index_offset[1]);
    return -1;
  }

  if(map.data[(y + index_offset[1]) * map.w + x + index_offset[0]] > num_tile_textures - 1) return -1;

  switch(map.data[(y + index_offset[1]) * map.w + x + index_offset[0]])
  {
    case 0:
      SDL_SetRenderDrawColor(renderer, 63, 63, 63, 255);
      SDL_RenderDrawBox(renderer, x * tile_size[0], y * tile_size[1], tile_size[0] - 1, tile_size[1]);
      break;
    case 1:
      render_texture_simple(renderer, 3, x * tile_size[0] + render_offset[0], y * tile_size[1] + render_offset[1], tile_size[0], tile_size[1]);
      break;
  }
}

int update_partial_map(SDL_Renderer* renderer, int player_pos[2], float player_pos_raster[2], int player_speed[2], int index_offset[2], int render_offset[2])
{
  if(player_pos_raster[1] <= map.h - 1.1f && player_pos_raster[1] >= 0.1f)
  {
    if(redraw_tile(renderer, map, player_pos_raster[0], player_pos_raster[1], index_offset, render_offset) == -1) return -1;
    if(player_pos_raster[1] + 1 < map.h - 0.9f) if(redraw_tile(renderer, map, player_pos_raster[0], player_pos_raster[1] + 1, index_offset, render_offset) == -1) return -1;
    if(player_pos_raster[1] + 2 < map.h - 0.9f) if(redraw_tile(renderer, map, player_pos_raster[0], player_pos_raster[1] + 2, index_offset, render_offset) == -1) return -1;
    if(player_pos_raster[1] - 1 > 0.1f) if(redraw_tile(renderer, map, player_pos_raster[0], player_pos_raster[1] - 1, index_offset, render_offset) == -1) return -1;
    if(redraw_tile(renderer, map, (int)((player_pos[0] - player_speed[0])) / tile_size[0], (int)((player_pos[1] - player_speed[1])) / tile_size[1], index_offset, render_offset) == -1) return -1;
    if(player_pos_raster[1] + 1 < map.h - 0.9f) if(redraw_tile(renderer, map, (int)((player_pos[0] - player_speed[0])) / tile_size[0], (int)((player_pos[1] - player_speed[1])) / tile_size[1] + 1, index_offset, render_offset) == -1) return -1;
    if(player_pos_raster[1] + 2 < map.h - 0.9f) if(redraw_tile(renderer, map, (int)((player_pos[0] - player_speed[0])) / tile_size[0], (int)((player_pos[1] - player_speed[1])) / tile_size[1] + 2, index_offset, render_offset) == -1) return -1;
    if(player_pos_raster[1] - 1 > 0.1f) if(redraw_tile(renderer, map, (int)((player_pos[0] - player_speed[0])) / tile_size[0], (int)((player_pos[1] - player_speed[1])) / tile_size[1] - 1, index_offset, render_offset) == -1) return -1;

    if(player_pos_raster[0] < map.w - 1.1f)
    {
      if(redraw_tile(renderer, map, player_pos_raster[0] + 0.9f, player_pos_raster[1], index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] + 1 < map.h - 0.9f) if(redraw_tile(renderer, map, player_pos_raster[0] + 0.9f, player_pos_raster[1] + 1, index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] + 2 < map.h - 0.9f) if(redraw_tile(renderer, map, player_pos_raster[0] + 0.9f, player_pos_raster[1] + 2, index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] - 1 > 0.1f) if(redraw_tile(renderer, map, player_pos_raster[0] + 0.9f, player_pos_raster[1] - 1, index_offset, render_offset) == -1) return -1;
      if(redraw_tile(renderer, map, (int)((player_pos[0] + tile_size[0] - player_speed[0])) / tile_size[0] + 0.9f, (int)((player_pos[1] - player_speed[1])) / tile_size[1], index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] + 1 < map.h - 0.9f) if(redraw_tile(renderer, map, (int)((player_pos[0] + tile_size[0] - player_speed[0])) / tile_size[0] + 0.9f, (int)((player_pos[1] - player_speed[1])) / tile_size[1] + 1, index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] + 2 < map.h - 0.9f) if(redraw_tile(renderer, map, (int)((player_pos[0] + tile_size[0] - player_speed[0])) / tile_size[0] + 0.9f, (int)((player_pos[1] - player_speed[1])) / tile_size[1] + 2, index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] - 1 > 0.1f) if(redraw_tile(renderer, map, (int)((player_pos[0] + tile_size[0] - player_speed[0])) / tile_size[0] + 0.9f, (int)((player_pos[1] - player_speed[1])) / tile_size[1] - 1, index_offset, render_offset) == -1) return -1;
    }

    if(player_pos_raster[0] > 0.1f)
    {
      if(redraw_tile(renderer, map, player_pos_raster[0] - 0.9f, player_pos_raster[1], index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] + 1 < map.h - 0.9f) if(redraw_tile(renderer, map, player_pos_raster[0] - 0.9f, player_pos_raster[1] + 1, index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] + 2 < map.h - 0.9f) if(redraw_tile(renderer, map, player_pos_raster[0] - 0.9f, player_pos_raster[1] + 2, index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] - 1 > 0.1f) if(redraw_tile(renderer, map, player_pos_raster[0] - 0.9f, player_pos_raster[1] - 1, index_offset, render_offset) == -1) return -1;
      if(redraw_tile(renderer, map, (int)((player_pos[0] + tile_size[0] - player_speed[0])) / tile_size[0] - 0.9f, (int)((player_pos[1] - player_speed[1])) / tile_size[1], index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] + 1 < map.h - 0.9f) if(redraw_tile(renderer, map, (int)((player_pos[0] + tile_size[0] - player_speed[0])) / tile_size[0] - 0.9f, (int)((player_pos[1] - player_speed[1])) / tile_size[1] + 1, index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] + 2 < map.h - 0.9f) if(redraw_tile(renderer, map, (int)((player_pos[0] + tile_size[0] - player_speed[0])) / tile_size[0] - 0.9f, (int)((player_pos[1] - player_speed[1])) / tile_size[1] + 2, index_offset, render_offset) == -1) return -1;
      if(player_pos_raster[1] - 1 > 0.1f) if(redraw_tile(renderer, map, (int)((player_pos[0] + tile_size[0] - player_speed[0])) / tile_size[0] - 0.9f, (int)((player_pos[1] - player_speed[1])) / tile_size[1] - 1, index_offset, render_offset) == -1) return -1;
    }
  }

  return 0;
}

int render_map(SDL_Renderer* renderer, Map map, int index_offset[2], int render_offset[2])
{
  for(int j = 0; j < visible_tiles[1]; j++)
  {
    for(int i = 0; i < visible_tiles[0]; i++)
    {
      if(redraw_tile(renderer, map, i, j, index_offset, render_offset) == -1) return -1;
    }
  }

  return 0;
}

#endif // RENDER_MANAGER_H
