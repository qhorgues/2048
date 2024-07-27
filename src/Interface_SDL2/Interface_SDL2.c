#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Interface.h"

#define WINDOW_WIDTH 360
#define WINDOW_HEIGHT 480
#define SIZE_TILE 75
#define SPACE_BETWEEN_TILE 10
#define MARGIN_WITH_BOARD 5
#define SIZE_BOARD ((SIZE_TILE * 4) + (SPACE_BETWEEN_TILE * 5))
#define RADIUS_TILE (SIZE_TILE / 10)
#define RADIUS_BOARD RADIUS_TILE
#define RADIUS_BUTTON RADIUS_TILE
#define MARGIN_TOP_WITH_BOARD (WINDOW_HEIGHT - SIZE_BOARD - MARGIN_WITH_BOARD)
#define MARGIN_LOGO 5
#define SIZE_LOGO 90

#define WHITE ((SDL_Color){255, 255, 255, SDL_ALPHA_OPAQUE})
#define TILE_2_4_TEXT_COLOR ((SDL_Color){119, 110, 101, SDL_ALPHA_OPAQUE})

#define TILE_EMPTY_COLOR ((SDL_Color){205, 193, 181, SDL_ALPHA_OPAQUE})
#define TILE_2_COLOR ((SDL_Color){238, 227, 218, SDL_ALPHA_OPAQUE})
#define TILE_4_COLOR ((SDL_Color){237, 224, 200, SDL_ALPHA_OPAQUE})
#define TILE_8_COLOR ((SDL_Color){242, 177, 121, SDL_ALPHA_OPAQUE})
#define TILE_16_COLOR ((SDL_Color){236, 141, 84, SDL_ALPHA_OPAQUE})
#define TILE_32_COLOR ((SDL_Color){246, 124, 95, SDL_ALPHA_OPAQUE})
#define TILE_64_COLOR ((SDL_Color){234, 89, 55, SDL_ALPHA_OPAQUE})
#define TILE_128_COLOR ((SDL_Color){242, 216, 107, SDL_ALPHA_OPAQUE})
#define TILE_256_COLOR ((SDL_Color){241, 208, 75, SDL_ALPHA_OPAQUE})
#define TILE_512_COLOR ((SDL_Color){227, 192, 42, SDL_ALPHA_OPAQUE})
#define TILE_1024_COLOR ((SDL_Color){226, 186, 19, SDL_ALPHA_OPAQUE})
#define TILE_2048_COLOR ((SDL_Color){236, 196, 0, SDL_ALPHA_OPAQUE})
#define TILE_4096_COLOR ((SDL_Color){94, 218, 146, SDL_ALPHA_OPAQUE})
#define TILE_8192_COLOR ((SDL_Color){133, 29, 214, SDL_ALPHA_OPAQUE})
#define TILE_16384_COLOR ((SDL_Color){93, 193, 193, SDL_ALPHA_OPAQUE})
#define TILE_32768_COLOR ((SDL_Color){194, 20, 40, SDL_ALPHA_OPAQUE})
#define TILE_65536_COLOR ((SDL_Color){38, 38, 38, SDL_ALPHA_OPAQUE})

#define BOARD_COLOR ((SDL_Color){187, 173, 160, SDL_ALPHA_OPAQUE})

struct Interface_SDL2
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture* text_logo;
    TTF_Font *button_font;
    TTF_Font *number_font[5];
    SDL_Texture *texture_number[16];
    bool key_pressed[4];
};

struct Button
{
    SDL_Texture* texture_button;
    SDL_Rect rect;
    SDL_Color color_button;
};

static struct Button* initButton(SDL_Renderer* renderer, SDL_Rect const* position, char const* text, TTF_Font* font, SDL_Color text_color, SDL_Color color_button)
{
    struct Button* button = malloc(sizeof(struct Button));
    if (button == NULL)
    {
        return NULL;
    }
    SDL_Surface *surface_txt = TTF_RenderText_Shaded(font, text, text_color, color_button);
    if (surface_txt == NULL)
    {
        printf("%s\n", TTF_GetError());
        free(button);
        return NULL;
    }
    button->texture_button = SDL_CreateTextureFromSurface(renderer, surface_txt);
    SDL_FreeSurface(surface_txt);
    if (button->texture_button == NULL)
    {
        printf("%s\n", TTF_GetError());
        free(button);
        return NULL;
    }
    button->rect = *position;
    button->color_button = color_button;
    return button;
}

static void freeButton(struct Button* button)
{
    SDL_DestroyTexture(button->texture_button);
    free(button);
}

static bool checkIfPressedButton(struct Button* button, int x, int y)
{
    return x >= button->rect.x && x <= button->rect.x + button->rect.w
           && y >= button->rect.y && y <= button->rect.y + button->rect.h;
}

static void RenderDrawTextRoundedRectBox_with_texture(SDL_Renderer *renderer, SDL_Rect const *rect, int radius, SDL_Texture *texture_text, SDL_Color background);

static void renderButton(SDL_Renderer *renderer, struct Button * button)
{
    RenderDrawTextRoundedRectBox_with_texture(renderer, &button->rect, RADIUS_BUTTON, button->texture_button, button->color_button);
}

Interface *initInterface(char const *dir_exe)
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    struct Interface_SDL2 *interface = malloc(sizeof(struct Interface_SDL2));
    if (interface == NULL)
    {
        return NULL;
    }

    interface->window = SDL_CreateWindow("2048", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

    memset(interface->texture_number, 0, 16 * sizeof(SDL_Texture *));
    memset(interface->key_pressed, 0, 4 * sizeof(bool));
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

static void RenderDrawTextRoundedRectBox_with_texture(SDL_Renderer *renderer, SDL_Rect const *rect, int radius, SDL_Texture *texture_text, SDL_Color background)
{
    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
    SDL_RenderFillRoudedRect(renderer, rect, radius);
    SDL_Rect txt_rect;
    SDL_QueryTexture(texture_text, NULL, NULL, &(txt_rect.w), &(txt_rect.h));
    txt_rect.x = rect->x + rect->w / 2 - txt_rect.w / 2;
    txt_rect.y = rect->y + rect->h / 2 - txt_rect.h / 2 + 3;
    SDL_RenderCopy(renderer, texture_text, NULL, &txt_rect);
}

static SDL_Texture *RenderDrawTextRoundedRectBox(
    SDL_Renderer *renderer,
    SDL_Rect const *rect,
    int radius,
    SDL_Color background,
    SDL_Color textcolor,
    TTF_Font *font,
    char const *text)
{
    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
    SDL_RenderFillRoudedRect(renderer, rect, radius);
    SDL_Surface *surface_txt = TTF_RenderText_Shaded(font, text, textcolor, background);
    if (surface_txt == NULL)
    {
        printf("%s\n", TTF_GetError());
        return NULL;
    }
    SDL_Texture *texture_text = SDL_CreateTextureFromSurface(renderer, surface_txt);
    SDL_FreeSurface(surface_txt);
    if (texture_text == NULL)
    {
        printf("%s\n", TTF_GetError());
        return NULL;
    }
    SDL_Rect txt_rect;
    SDL_QueryTexture(texture_text, NULL, NULL, &(txt_rect.w), &(txt_rect.h));
    txt_rect.x = rect->x + rect->w / 2 - txt_rect.w / 2;
    txt_rect.y = rect->y + rect->h / 2 - txt_rect.h / 2 + 3;

    SDL_RenderCopy(renderer, texture_text, NULL, &txt_rect);
    return texture_text;
}

static void DrawTile(struct Interface_SDL2 *interface, int tile, int index_x, int index_y)
{
    SDL_Rect rect_tile;
    rect_tile.x = MARGIN_WITH_BOARD + SPACE_BETWEEN_TILE + (SPACE_BETWEEN_TILE + SIZE_TILE) * index_x;
    rect_tile.y = MARGIN_TOP_WITH_BOARD + SPACE_BETWEEN_TILE + (SPACE_BETWEEN_TILE + SIZE_TILE) * index_y;
    rect_tile.w = SIZE_TILE;
    rect_tile.h = SIZE_TILE;

    if (tile == 0)
    {
        SDL_SetRenderDrawColor(interface->renderer, TILE_EMPTY_COLOR.r, TILE_EMPTY_COLOR.g, TILE_EMPTY_COLOR.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRoudedRect(interface->renderer, &rect_tile, RADIUS_TILE);
        return;
    }
    SDL_Color backgroud[16] = {
        TILE_2_COLOR,
        TILE_4_COLOR,
        TILE_8_COLOR,
        TILE_16_COLOR,
        TILE_32_COLOR,
        TILE_64_COLOR,
        TILE_128_COLOR,
        TILE_256_COLOR,
        TILE_512_COLOR,
        TILE_1024_COLOR,
        TILE_2048_COLOR,
        TILE_4096_COLOR,
        TILE_8192_COLOR,
        TILE_16384_COLOR,
        TILE_32768_COLOR,
        TILE_65536_COLOR};
    SDL_Color text_color;
    if (tile > 2)
    {
        text_color = WHITE;
    }
    else
    {
        text_color = TILE_2_4_TEXT_COLOR;
    }

    int number_tile = 1 << tile;
    int numberChr = (int)log10(number_tile);
    char int_to_chr[12];
    snprintf(int_to_chr, 12, "%d", number_tile);

    if (interface->texture_number[tile - 1] != NULL)
    {
        RenderDrawTextRoundedRectBox_with_texture(interface->renderer, &rect_tile, RADIUS_TILE, interface->texture_number[tile - 1], backgroud[tile - 1]);
    }
    else
    {
        interface->texture_number[tile - 1] = RenderDrawTextRoundedRectBox(
            interface->renderer,
            &rect_tile,
            RADIUS_TILE,
            backgroud[tile - 1],
            text_color,
            interface->number_font[numberChr], int_to_chr);
    }
}

static void DrawBoard(struct Interface_SDL2 *interface, const int (*board)[4])
{
    SDL_Rect rect_board = {.x = MARGIN_WITH_BOARD, .y = MARGIN_TOP_WITH_BOARD, .w = SIZE_BOARD, .h = SIZE_BOARD};
    // SDL_Rect rect_background_board = {.x = 0, .y = MARGIN_TOP_WITH_BOARD - MARGIN_WITH_BOARD, .w = WINDOW_WIDTH, .h = SIZE_BOARD + (2*MARGIN_WITH_BOARD)};
    // SDL_SetRenderDrawColor(interface->renderer, WHITE.r, WHITE.g, WHITE.b, SDL_ALPHA_OPAQUE);
    // SDL_RenderFillRect(interface->renderer, &rect_background_board);
    SDL_SetRenderDrawColor(interface->renderer, BOARD_COLOR.r, BOARD_COLOR.g, BOARD_COLOR.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRoudedRect(interface->renderer, &rect_board, RADIUS_BOARD);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            DrawTile(interface, board[i][j], i, j);
        }
    }
}

static void DrawLogo(struct Interface_SDL2 *interface)
{
    if (interface->text_logo == NULL)
    {
        SDL_Surface *surface_txt = TTF_RenderText_Shaded(interface->number_font[2], "2048", WHITE, TILE_2048_COLOR);
        if (surface_txt == NULL)
        {
            printf("%s\n", TTF_GetError());
        }
        interface->text_logo = SDL_CreateTextureFromSurface(interface->renderer, surface_txt);
        SDL_FreeSurface(surface_txt);
        if (interface->text_logo == NULL)
        {
            printf("%s\n", TTF_GetError());
        }
    }
    SDL_Rect txt_rect;
    SDL_Rect rect_tile = {.x = MARGIN_LOGO, .y = MARGIN_LOGO, .w = SIZE_LOGO, .h = SIZE_LOGO};
    SDL_QueryTexture(interface->text_logo, NULL, NULL, &(txt_rect.w), &(txt_rect.h));
    txt_rect.x =  rect_tile.x + rect_tile.w / 2 - txt_rect.w / 2;
    txt_rect.y = rect_tile.y + rect_tile.h / 2 - txt_rect.h / 2 + 3;
    SDL_SetRenderDrawColor(interface->renderer, TILE_2048_COLOR.r, TILE_2048_COLOR.g, TILE_2048_COLOR.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRoudedRect(interface->renderer, &rect_tile, RADIUS_TILE);
    SDL_RenderCopy(interface->renderer, interface->text_logo, NULL, &txt_rect);
}

static void updateInGame(struct Interface_SDL2 *interface, struct GameEngine const *gameEngine)
{
    DrawLogo(interface);
    DrawBoard(interface, gameEngine->board);
}

void update(Interface *interface, enum GameStatus status, struct GameEngine const *gameEngine)
{
    struct Interface_SDL2 *inter = interface;
    SDL_SetRenderDrawColor(inter->renderer, WHITE.r, WHITE.g, WHITE.b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(inter->renderer);
    switch (status)
    {
    case IN_GAME:
        updateInGame(inter, gameEngine);
        break;
    default:
        break;
    }
    SDL_RenderPresent(inter->renderer);
}

enum Interactions getInteraction(Interface *interface, enum GameStatus *status, struct GameEngine const *gameEngine)
{
    struct Interface_SDL2 *inter = interface;
    SDL_Event event;
    while (true)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_WINDOWEVENT:
            update(interface, *status, gameEngine);
            break;
        case SDL_QUIT:
            return INTERACTION_QUIT;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_UP:
                if (!inter->key_pressed[0])
                { 
                    inter->key_pressed[0] = true;
                    return INTERACTION_MOVE_UP;
                }
                break;
            case SDL_SCANCODE_DOWN:
                if (!inter->key_pressed[1])
                { 
                    inter->key_pressed[1] = true;
                    return INTERACTION_MOVE_DOWN;
                }
                break;
            case SDL_SCANCODE_LEFT:
                if (!inter->key_pressed[2])
                { 
                    inter->key_pressed[2] = true;
                    return INTERACTION_MOVE_LEFT;
                }
                break;
            case SDL_SCANCODE_RIGHT:
                if (!inter->key_pressed[3])
                { 
                    inter->key_pressed[3] = true;
                    return INTERACTION_MOVE_RIGHT;
                }
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_UP:
                inter->key_pressed[0] = false;
                break;
            case SDL_SCANCODE_DOWN:
                inter->key_pressed[1] = false;
                break;
            case SDL_SCANCODE_LEFT:
                inter->key_pressed[2] = false;
                break;
            case SDL_SCANCODE_RIGHT:
                inter->key_pressed[3] = false;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void freeInterface(Interface *interface)
{
    struct Interface_SDL2 *inter = interface;
    for (int i = 1; i < 5; i++)
    {
        if (inter->number_font[i])
        {
            TTF_CloseFont(inter->number_font[i]);
        }
    }
    for (int i = 0; i < 16; i++)
    {
        if (inter->texture_number[i] != NULL)
        {
            SDL_DestroyTexture(inter->texture_number[i]);
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

void test(void)
{
    SDL_Delay(5000);
}