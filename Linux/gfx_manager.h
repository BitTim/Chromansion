#ifndef GFX_MANAGER_H
#define GFX_MANAGER_H

#include <vector>
#include "variables.h"
#include "texture_handler.h"

int render_gfx(SDL_Renderer* renderer)
{
	SDL_Rect dst_rect;

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.w = screen_size[0];
	dst_rect.h = screen_size[1];

	SDL_RenderCopy(renderer, gfx_atlas[0].texture, NULL, &dst_rect);
	return 0;
}

void gfx_fadeout(SDL_Renderer *renderer)
{
	SDL_Surface* fade;
	fade = SDL_CreateRGBSurface(0, screen_size[0], screen_size[1], 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

	for(int i = 0; i <= 50; i++)
	{
		SDL_FillRect(fade, NULL, SDL_MapRGBA(fade->format, 0, 0, 0, i));
		SDL_RenderCopy(renderer, SDL_CreateTextureFromSurface(renderer, fade), NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	for(int i = 0; i <= 35; i++)
	{
		SDL_FillRect(fade, NULL, SDL_MapRGBA(fade->format, i, i, i, 50));
		SDL_RenderCopy(renderer, SDL_CreateTextureFromSurface(renderer, fade), NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

#endif // GFX_MANAGER_H
