#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Interface.h"

struct Interface
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *title_font;
	TTF_Font *button_font;
	TTF_Font *number_font[4];
};

Interface *initInterface(char const *dir_exe)
{
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	struct Interface *interface = malloc(sizeof(struct Interface));
	if (interface == NULL)
	{
		return NULL;
	}

	interface->window = SDL_CreateWindow("2048", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 360, 480, SDL_WINDOW_SHOWN);
	if (interface->window == NULL)
	{
		free(interface);
		return NULL;
	}

	interface->renderer = SDL_CreateRenderer(interface->window, -1, SDL_RENDERER_ACCELERATED);
	if (interface->renderer == NULL)
	{
		SDL_DestroyWindow(interface->window);
		free(interface);
		return NULL;
	}

	char *font_dir = malloc((sizeof(char) * strlen(dir_exe)) + sizeof("assets/calibrib.ttf"));
	if (font_dir == NULL)
	{
		SDL_DestroyRenderer(interface->renderer);
		SDL_DestroyWindow(interface->window);
		free(interface);
		return NULL;
	}

	strcpy(font_dir, dir_exe);
	strcat(font_dir, "assets/calibrib.ttf");

	for (int i = 1; i < 4; i++)
	{
		interface->number_font[i] = TTF_OpenFont(font_dir, 15 - 3 * i);
	}
	interface->number_font[0] = interface->number_font[1];

	free(font_dir);
	return interface;
}

static int SDL_RenderFillRoudedRect(SDL_Renderer *renderer, SDL_Rect const *rect, int radius)
{
	if (radius > rect->w / 2 || radius > rect->h / 2)
	{
		radius = (rect->w > rect->h) ? rect->h / 2 : rect->w / 2;
	}

	int d = 3 - (2 * radius);
	int x = 0;
	int y = radius;
	while (y >= x)
	{
		if (SDL_RenderDrawLine(renderer, rect->x - x + radius, rect->y - y + radius, rect->x + x - 1 + rect->w - radius, rect->y - y + radius) < 0) // haut
		{
			return -1;
		}

		if (SDL_RenderDrawLine(renderer, rect->x - x + radius, rect->y + y + rect->h - radius, rect->x + x - 1 + rect->w - radius, rect->y + y + rect->h - radius) < 0) // bas
		{
			return -1;
		}

		if (SDL_RenderDrawLine(renderer, rect->x - y + radius, rect->y - x + radius, rect->x + y - 1 + rect->w - radius, rect->y - x + radius) < 0) // milieu haut
		{
			return -1;
		}

		if (SDL_RenderDrawLine(renderer, rect->x - y + radius, rect->y + x + rect->h - radius, rect->x + y - 1 + rect->w - radius, rect->y + x + rect->h - radius) < 0) // milieu bas
		{
			return -1;
		}
		if (d < 0)
			d = d + (4 * x) + 6;
		else
		{
			d = d + 4 * (x - y) + 10;
			y--;
		}

		x++;
	}
	SDL_Rect sub_rect = {rect->x, rect->y + radius, rect->w, rect->h - 2 * radius};
	SDL_RenderFillRect(renderer, &sub_rect);
	return 0;
}

static void RenderDrawTextRoundedRectBox(SDL_Renderer *renderer, SDL_Rect const *rect, int radius, SDL_Color background, SDL_Color textcolor, TTF_Font *font, char const *text)
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderFillRoudedRect(renderer, rect, radius);
	SDL_Surface *surface_txt = TTF_RenderText_Shaded(font, text, textcolor, background);
	if (surface_txt == NULL)
	{
		printf("%s\n", TTF_GetError());
		return;
	}
	SDL_Texture *text_txt = SDL_CreateTextureFromSurface(renderer, surface_txt);
	SDL_RenderCopy(renderer, text_txt, NULL, rect);
	SDL_FreeSurface(surface_txt);
	SDL_RenderPresent(renderer);
}

void test(Interface* interface)
{
	struct Interface* inter = interface;
	SDL_Rect rect = {.x = 100, .y = 100, .w = 100, .h = 100};
	SDL_SetRenderDrawColor(inter->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRoudedRect(inter->renderer, &rect, 25);
	SDL_RenderPresent(inter->renderer);
	SDL_Delay(2000);
}


void freeInterface(Interface *interface)
{
	struct Interface *inter = interface;
	for (int i = 1; i < 4; i++)
	{
		if (inter->number_font[i])
		{
			TTF_CloseFont(inter->number_font[i]);
		}
	}
	if (inter->renderer)
	{
		SDL_DestroyRenderer(inter->renderer);
	}
	if (inter->window)
	{
		SDL_DestroyWindow(inter->window);
	}

	free(inter);
	TTF_Quit();
	SDL_Quit();
}
