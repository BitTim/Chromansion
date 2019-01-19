#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface *texture_loader;
std::vector<SDL_Texture*> texture_atlas;

int load_texture(SDL_Renderer* renderer, std::string path)
{
  texture_loader = IMG_Load(path.c_str());

  if(texture_loader == NULL)
  {
    texture_loader = IMG_Load("data/textures/missing.png");
    if(texture_loader == NULL)
    {
      printf("Error 201: Could not load Texure, %s", IMG_GetError());
      return -1;
    }
  }

  texture_atlas.push_back(SDL_CreateTextureFromSurface(renderer, texture_loader));
  return 0;
}

int load_textures(SDL_Renderer* renderer)
{
  if(load_texture(renderer, "data/textures/player_white.png")) return -1;

  SDL_FreeSurface(texture_loader);
  return 0;
}

int render_texture_simple(SDL_Renderer *renderer, int id, int x, int y, int w, int h)
{
  if(id >= texture_atlas.size()) return -1;

  SDL_Rect dst_rect;

  dst_rect.x = x;
  dst_rect.y = y;
  dst_rect.w = w;
  dst_rect.h = h;

  SDL_RenderCopy(renderer, texture_atlas[id], NULL, &dst_rect);
  return 0;
}

#endif // TEXTURE_HANDLER_H
