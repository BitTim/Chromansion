#ifndef GUI_HANDLER
#define GUI_HANDLER

#include <vector>

#include <SDL2/SDL.h>
#include "SDL_Extend.h"

struct col
{
	int r;
	int g;
	int b;
	bool locked;
};

int GUI_ColorSelect(SDL_Renderer* renderer, std::vector<col> colors, int window_pos_x, int window_pos_y, int item_size_x, int item_size_y, int items_per_row, int mouse_pos[2])
{
	int rows_of_items = (int)((float)colors.size() / (float)items_per_row + 1.0f);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(renderer, 25, 25, 25, 128);
	SDL_RenderDrawBox(renderer, window_pos_x, window_pos_y, item_size_x * items_per_row + (items_per_row + 2) * 5, item_size_y * rows_of_items + (rows_of_items + 2) * 5);

	for(int i = 0; i < colors.size(); i++)
	{
		int pos[2];

		pos[0] = (int)(((float)i / (float)items_per_row - (int)i / (int)items_per_row) * (float)items_per_row);
		pos[1] = (int)(i / items_per_row);

		if(!colors[i].locked)
		{
			SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, 255);
			SDL_RenderDrawBox(renderer, (window_pos_x + 5) + pos[0] * item_size_x + pos[0] * 5, (window_pos_y + 5) + pos[1] * item_size_y + pos[1] * 5, item_size_x, item_size_y);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, colors[i].r / 2, colors[i].g / 2, colors[i].b / 2, 128);
			SDL_RenderDrawBox(renderer, (window_pos_x + 5) + pos[0] * item_size_x + pos[0] * 5 + 5, (window_pos_y + 5) + pos[1] * item_size_y + pos[1] * 5 + 5, item_size_x - 10, item_size_y - 10);
		}
	}

	int m_pos[2] =
	{
		(mouse_pos[0] - window_pos_x) / (item_size_x + 5),
		(mouse_pos[1] - window_pos_y) / (item_size_y + 5)
	};

	SDL_SetRenderDrawColor(renderer, 175, 175, 175, 255);
	if(m_pos[0] >= 0 && m_pos[0] < items_per_row && m_pos[1] >= 0 && m_pos[1] < rows_of_items && m_pos[1] * items_per_row + m_pos[0] < colors.size())
	{
		SDL_RenderDrawBox(renderer, (window_pos_x + 5) + m_pos[0] * item_size_x + m_pos[0] * 5 + 8, (window_pos_y + 5) + m_pos[1] * item_size_y + m_pos[1] * 5 + 8, item_size_x - 16, item_size_y - 16);
		return m_pos[1] * items_per_row + m_pos[0];
	}

	return -1;
}

#endif // GUI_HANDLER
