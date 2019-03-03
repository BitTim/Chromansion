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

#endif // GFX_MANAGER_H
