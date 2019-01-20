#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <SDL2/SDL.h>

#include "texture_handler.h"

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

int render_player(SDL_Renderer* renderer, int x, int y)
{
  if(render_texture_animated(renderer, 0, x, y, player_size[0], player_size[1]) == -1) return -1;
  return 0;
}

#endif // RENDER_MANAGER_H
