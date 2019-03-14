#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <SDL2/SDL.h>

#include "powerup_handler.h"
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

int render_player(SDL_Renderer* renderer, int x, int y, float index_offset[2], int render_offset[2], int speed[2], int player_color, int player_dir_state)
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

  if(player_dir_state == 0) if(render_texture_animated(renderer, standing, x - index_offset[0] * tile_size[0], y - index_offset[1] * tile_size[1], player_size[0], player_size[1]) == -1) return -1;
  if(player_dir_state == 1) if(render_texture_animated(renderer, right, x - index_offset[0] * tile_size[0], y - index_offset[1] * tile_size[1], player_size[0], player_size[1]) == -1) return -1;
  if(player_dir_state == 2) if(render_texture_animated(renderer, left, x - index_offset[0] * tile_size[0], y - index_offset[1] * tile_size[1], player_size[0], player_size[1]) == -1) return -1;

  return 0;
}

int render_powerups(SDL_Renderer* renderer, std::vector<pup> pup_list, float index_offset[2])
{
	for(pup pow : pup_list)
	{
		float pup_x = (float)pow.x - index_offset[0];
		float pup_y = (float)pow.y - index_offset[1];
		int texture_id = 30;

		if(pup_x >= 0 && pup_y >= 0 && pow.visible)
		{
			if(pow.type == 1) texture_id = 16; 
			if(pow.type == 2) texture_id = 17;
			if(pow.type == 3) texture_id = 18;
			if(pow.type == 4) texture_id = 29;			

			render_texture_animated(renderer, texture_id, (int)(pup_x * (float)tile_size[0]), (int)(pup_y * (float)tile_size[1]), tile_size[0], tile_size[1]);
		}
	}

	return 0;
}

int redraw_tile(SDL_Renderer* renderer, Map map, int x, int y, float index_offset[2], int render_offset[2], bool override_color)
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
      if(player_color == 1 || override_color) render_texture_simple(renderer, 13, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else clear = true;
	  break;
    case 3:
      if(player_color == 2 || override_color) render_texture_simple(renderer, 14, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else clear = true;
      break;
    case 4:
      if(player_color == 3 || override_color) render_texture_simple(renderer, 15, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else clear = true;
      break;
	case 5:
	  if(player_color == 1)      render_texture_simple(renderer, 13, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else if(player_color == 2) render_texture_simple(renderer, 14, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else if(override_color)    render_texture_simple(renderer, 21, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
  	  else clear = true;
	  break;
	case 6:
	  if(player_color == 1)      render_texture_simple(renderer, 13, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else if(player_color == 3) render_texture_simple(renderer, 15, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else if(override_color)    render_texture_simple(renderer, 22, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
  	  else clear = true;
	  break;
	case 7:
	  if(player_color == 2)      render_texture_simple(renderer, 14, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else if(player_color == 3) render_texture_simple(renderer, 15, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else if(override_color)    render_texture_simple(renderer, 23, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
  	  else clear = true;
	  break;
	case 8:
	  if(player_color == 1)      render_texture_simple(renderer, 13, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else if(player_color == 2) render_texture_simple(renderer, 14, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else if(player_color == 3) render_texture_simple(renderer, 15, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
	  else if(override_color)    render_texture_simple(renderer, 24, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
  	  else clear = true;
	  break;
	case 9:
      render_texture_simple(renderer, 25, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
  	  break;
	case 10:
	  if(player_color == 1)		 render_texture_simple(renderer, 26, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
  	  break;
	case 11:
	  if(player_color == 2)		 render_texture_simple(renderer, 27, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
  	  break;
	case 12:
	  if(player_color == 3)		 render_texture_simple(renderer, 28, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
  	  break;
  }

  if(clear)
  {
    clear = false;
    SDL_SetRenderDrawColor(renderer, 63, 63, 63, 255);
    SDL_RenderDrawBox(renderer, x * tile_size[0] - render_offset[0], y * tile_size[1] - render_offset[1], tile_size[0], tile_size[1]);
  }

  return 0;
}

int update_map_player(SDL_Renderer* renderer, float player_pos_raster[2], float index_offset[2], int render_offset[2])
{
  //Update Player Tiles
  if(redraw_tile(renderer, map, (int)player_pos_raster[0], (int)player_pos_raster[1], index_offset, render_offset, false) == -1) return -1;
  if(redraw_tile(renderer, map, (int)player_pos_raster[0], (int)player_pos_raster[1] + 1, index_offset, render_offset, false) == -1) return -1;
  return 0;
}

int render_map(SDL_Renderer* renderer, Map map, float index_offset[2], int render_offset[2], bool override_color)
{
  int t_before = SDL_GetTicks();

  for(int j = 0; j < (int)visible_tiles[1]; j++)
  {
    for(int i = 0; i < (int)visible_tiles[0]; i++)
    {
      if(redraw_tile(renderer, map, i, j, index_offset, render_offset, override_color) == -1) return -1;
    }
  }

  int t_after = SDL_GetTicks();
  return 0;
}

#endif // RENDER_MANAGER_H
