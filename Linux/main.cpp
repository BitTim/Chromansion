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

const Uint8 *key_state = SDL_GetKeyboardState(NULL);
unsigned int last_time = 0, current_time;
int unused;

bool quit = false;

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

  load_textures(renderer);
}

void end()
{
  screen_end();
}

int draw_screen()
{
  SDL_SetRenderDrawColor(renderer, 63, 63, 63, 255);
  SDL_RenderClear(renderer);

  if(render_texture_simple(renderer, 0, screen_size[0] / 2 - player_size[0] / 2, screen_size[1] / 2 - player_size[1] / 2, player_size[0], player_size[1]) == -1) return -1;

  SDL_RenderPresent(renderer);
}

int update()
{
  if(draw_screen() == -1) return -1;
}

int main()
{
  init();

  while(!quit)
  {
    current_time = SDL_GetTicks();
 		if(current_time > last_time + 5)
 		{
 			last_time = current_time;
      SDL_PollEvent(&event);

      switch(event.type)
      {
        case SDL_QUIT:
          quit = true;
          break;
      }

      if(update() == -1) return -1;
    }
  }

  end();
}
