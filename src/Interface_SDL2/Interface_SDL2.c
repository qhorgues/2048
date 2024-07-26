#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Interface.h"

#define SIZE_TILE 75
#define SPACE_BETWEEN_TILE 10
#define MARGIN_WITH_BOARD 5
#define SIZE_BOARD SIZE_TILE * 4 + (SPACE_BETWEEN_TILE*5)
#define RADIUS_TILE SIZE_TILE / 4
#define RADIUS_BOARD SIZE_BOARD / 4

#define WHITE ((SDL_Color){255, 255, 255, SDL_ALPHA_OPAQUE})

typedef struct Interface_SDL2
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *title_font;
	TTF_Font *button_font;
	TTF_Font *number_font[5];
} Interface_SDL2;

Interface *initInterface(char const *dir_exe)
{
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	Interface_SDL2 *interface = malloc(sizeof(Interface_SDL2));
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

	for (int i = 1; i < 5; i++)
	{
		interface->number_font[i] = TTF_OpenFont(font_dir, 58 - 8 * i);
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
	SDL_Rect txt_rect;
	SDL_QueryTexture(text_txt, NULL, NULL, &(txt_rect.w), &(txt_rect.h));
	txt_rect.x = rect->x + rect->w / 2 - txt_rect.w / 2;
	txt_rect.y = rect->y + rect->h / 2 - txt_rect.h / 2 + 3;

	SDL_RenderCopy(renderer, text_txt, NULL, &txt_rect);
	SDL_FreeSurface(surface_txt);
	SDL_RenderPresent(renderer);
}

static void DrawTile(Interface_SDL2 *interface, int tile, int index_x, int index_y)
{
	SDL_Color backgroud;
	switch (tile)
	{
		case 1:
			backgroud = WHITE;
			break;
		default:
			backgroud = WHITE;
			break;
	}
	SDL_Rect rect_tile;
	rect_tile.x = MARGIN_WITH_BOARD + SPACE_BETWEEN_TILE + (SPACE_BETWEEN_TILE + SIZE_TILE) * index_x;
	rect_tile.y = MARGIN_WITH_BOARD + SPACE_BETWEEN_TILE + (SPACE_BETWEEN_TILE + SIZE_TILE) * index_y;
	rect_tile.w = SIZE_TILE;
	rect_tile.h = SIZE_TILE;

	int number_tile = 1 << tile;
	int numberChr = (int)log10(number_tile);
	char int_to_chr[7];
	snprintf(int_to_chr, 6, "%d", number_tile);

	RenderDrawTextRoundedRectBox(interface->renderer, &rect_tile, RADIUS_TILE, backgroud, (SDL_Color){0, 0, 0, SDL_ALPHA_OPAQUE}, interface->number_font[numberChr], int_to_chr);


}

void test(Interface* interface)
{
	Interface_SDL2* inter = interface;
	
	DrawTile(inter, 3, 0, 0);
	DrawTile(inter, 6, 1, 0);
	DrawTile(inter, 9, 2, 0);
	DrawTile(inter, 13, 3, 0);
	DrawTile(inter, 16, 0, 1);


	SDL_RenderPresent(inter->renderer);
	SDL_Delay(5000);
}


void freeInterface(Interface *interface)
{
	Interface_SDL2 *inter = interface;
	for (int i = 1; i < 5; i++)
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
