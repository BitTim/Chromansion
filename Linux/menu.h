#ifndef MENU_H
#define MENU_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "SDL_Extend.h"

bool menu_quit = false;

int mouse_hover(int button_width, int offset_top, int menu_type)
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	if(menu_type == 1)
	{
		     if(mx >= 25 + button_width * 0 && mx <= 25 + button_width * 1 && my >= offset_top + 25 + 36 && my <= offset_top + 25 + 36 * 2) return 1;
		else if(mx >= 25 + button_width * 1 && mx <= 25 + button_width * 2 && my >= offset_top + 25 + 36 && my <= offset_top + 25 + 36 * 2) return 2;
		else if(mx >= 25 + button_width * 2 && mx <= 25 + button_width * 3 && my >= offset_top + 25 + 36 && my <= offset_top + 25 + 36 * 2) return 3;
		else if(mx >= 25 + button_width * 3 && mx <= 25 + button_width * 4 && my >= offset_top + 25 + 36 && my <= offset_top + 25 + 36 * 2) return 4;
		else if(mx >= 25 + button_width * 4 && mx <= 25 + button_width * 5 && my >= offset_top + 25 + 36 && my <= offset_top + 25 + 36 * 2) return 5;
		else return 0;
	}
	else if(menu_type == 2)
	{
			 //if(mx >= 25 + button_width * 0 && mx <= 25 + button_width * 1 && my 
	}
}

int settings_menu(SDL_Renderer* renderer, TTF_Font* menu_font)
{

}

int main_menu(SDL_Renderer* renderer, TTF_Font* menu_font)
{
	SDL_Event event;

	TTF_Font* header_font = TTF_OpenFont("data/fonts/font.ttf", 36);

	SDL_Color menu_color = {255, 255, 255, 255};
	SDL_Color start_color = {30, 255, 128, 255};
	SDL_Color load_color = {220, 255, 0, 255};
	SDL_Color settings_color = {0, 175, 255, 255};
	SDL_Color about_color = {128, 0, 255, 255};
	SDL_Color exit_color = {255, 30, 50, 255};

	int offset_top = screen_size[1] / 5 * 3;
	int button_width = (screen_size[0] - 50) / 5;

	int curr_hover = 0, prev_hover = -1;

	while(!menu_quit)
	{
		curr_hover = mouse_hover(button_width, offset_top, 1);

		if(curr_hover != prev_hover)
		{
			SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
			SDL_RenderClear(renderer);

			int text_size[2];

			TTF_Print(renderer, "Chromansion", &text_size[0], &text_size[1], 20, offset_top, screen_size[0], header_font, menu_color);
			
			TTF_Print(renderer, "New Game", &text_size[0], &text_size[1], 25, offset_top + 25 + 36, screen_size[0], menu_font, curr_hover == 1 ? start_color : menu_color);
			TTF_Print(renderer, "Load Game", &text_size[0], &text_size[1], 25 + button_width * 1, offset_top + 25 + 36, screen_size[0], menu_font, curr_hover == 2 ? load_color : menu_color);
			TTF_Print(renderer, "Settings", &text_size[0], &text_size[1], 25 + button_width * 2, offset_top + 25 + 36, screen_size[0], menu_font, curr_hover == 3 ? settings_color : menu_color);
			TTF_Print(renderer, "About", &text_size[0], &text_size[1], 25 + button_width * 3, offset_top + 25 + 36, screen_size[0], menu_font, curr_hover == 4 ? about_color : menu_color);
			TTF_Print(renderer, "Exit", &text_size[0], &text_size[1], 25 + button_width * 4, offset_top + 25 + 36, screen_size[0], menu_font, curr_hover == 5 ? exit_color : menu_color);
			
			SDL_RenderPresent(renderer);
		}

		SDL_PollEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				menu_quit = true;
				break;

			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button)
				{
					case SDL_BUTTON_LEFT:
						switch(mouse_hover(button_width, offset_top, 1))
						{
							case 1: //New
								menu_quit = 1;
								break;
							
							case 2: //Load
								// Load Dialogue
								menu_quit = 1;
								break;
							
							case 3: //Settings
								// Settings Dialogue
								break;
							
							case 4: //About
								// Credits
								break;
							
							case 5: //Exit
								menu_quit = true;
								return 1;
								break;
						}
						break;
				}
				break;
		}

		prev_hover = curr_hover;
	}

	return 0;
}

#endif // MENU_H
