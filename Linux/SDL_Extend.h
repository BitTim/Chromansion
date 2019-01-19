#ifndef SDL_EXTEND_H
#define SDL_EXTEND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void SDL_RenderDrawBox(SDL_Renderer *renderer, int x, int y, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		SDL_RenderDrawLine(renderer, x, i + y, x + width, i + y);
	}
}

void SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int r, int start_angle, int end_angle)
{
	int x2;
	int y2;
	float c_angle;

	for(float i = start_angle - 90; i < end_angle - 90; i += 0.001f)
	{
		c_angle = i * 3.141592654 / 180.0;
		x2 = x + r * cosf(c_angle);
		y2 = y + r * sinf(c_angle);

		SDL_RenderDrawLine(renderer, x, y, x2, y2);
	}
}

void TTF_Print(SDL_Renderer *renderer, const char* text, int* width, int *height, int x, int y, int wrap, TTF_Font *font, SDL_Color color)
{
	SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, wrap);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect dstrect;
	int w, h, access;
	Uint32 format;

	dstrect.x = x;
	dstrect.y = y;

	SDL_QueryTexture(texture, &format, &access, &w, &h);

	*width = w;
	*height = h;

	dstrect.w = w;
	dstrect.h = h;

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
}

#endif // SDL_EXTEND_H
