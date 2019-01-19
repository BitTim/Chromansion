//Chromatic v0.1.0
//Copyright (c) BitTim 2019
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "variables.h"
#include "texture_handler.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

bool quit = false;

TTF_Font *font;
SDL_Color text_color = {255, 255, 255};
SDL_Color green_color = {53, 214, 56};
SDL_Color red_color = {214, 53, 56};

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
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}

void init()
{
  screen_init();
}

void end()
{
  screen_end();
}

int main()
{
  init();

  while(!quit)
  {
    SDL_PollEvent(&event);

    switch(event.type)
    {
      case SDL_QUIT:
        quit = true;
        break;
    }
  }

  end();
}
