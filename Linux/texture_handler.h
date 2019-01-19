#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface *texture_loader;
std::vector<SDL_Texture*> texture_atlas;

int load_textures(SDL_Renderer* renderer)
{
  texture_loader = IMG_load("data/textures/player_white.png");
  if(texture_loader == NULL) return -1;
  texture_atlas.push_back(SDL_CreateTextureFromSurface(renderer, texture_loader));

  SDL_FreeSurface(texture_loader);
  return 0;
}

#endif // TEXTURE_HANDLER_H
