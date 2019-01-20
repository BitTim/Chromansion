#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture* load_texture(SDL_Renderer* renderer, std::string path);

class Sprite
{
public:
  SDL_Texture *texture;

  bool animated = false;
  int frames = 1;
  int speed = 100;

  Sprite(SDL_Renderer *renderer, std::string path)
  {
    texture = load_texture(renderer, path);
  }

  Sprite(SDL_Renderer *renderer, std::string path, int num_frames, int init_speed)
  {
    animated = true;
    frames = num_frames;
    speed = init_speed;

    texture = load_texture(renderer, path);
  }
};

SDL_Surface *texture_loader;
Sprite *sprite_loader;
std::vector<Sprite> texture_atlas;

SDL_Texture* load_texture(SDL_Renderer* renderer, std::string path)
{
  texture_loader = IMG_Load(path.c_str());

  if(texture_loader == NULL)
  {
    texture_loader = IMG_Load("data/textures/missing.png");
    if(texture_loader == NULL)
    {
      printf("Error 201: Could not load Texure, %s", IMG_GetError());
      return NULL;
    }
  }

  return SDL_CreateTextureFromSurface(renderer, texture_loader);
}

int load_textures(SDL_Renderer* renderer)
{
  sprite_loader = new Sprite(renderer, "data/textures/player_white_idle.png", 12, 40);
  texture_atlas.push_back(*sprite_loader);
  delete sprite_loader;

  SDL_FreeSurface(texture_loader);
  return 0;
}

#endif // TEXTURE_HANDLER_H
