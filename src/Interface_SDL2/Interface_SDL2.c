#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include "../Interface.h"

typedef struct Interface
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  TTF_Font* title_font;
  TTF_Font* button_font;
  TTF_Font* large_number_font;
  TTF_Font* medium_number_font;
  TTF_Font* small_number_font;
} Interface;

Interface* initInterface(void)
{
  Interface* interface = malloc(sizeof(Interface));
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

  interface->window = SDL_CreateRenderer(interface->window, -1, SDL_RENDERER_ACCELERATED);
  if (interface->window == NULL)
  {
    SDL_DestroyWindow(interface->window);
    free(interface);
    return NULL;
  }

  return interface;
}

void freeInterface(Interface* interface)
{
  SDL_DestroyRenderer(interface->renderer);
  SDL_DestroyWindow(interface->window);
  free(interface);
}
