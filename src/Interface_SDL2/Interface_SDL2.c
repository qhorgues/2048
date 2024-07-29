
#if defined(WIN32)
    #include <SDL.h>
    #include <SDL_ttf.h>
#else
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../Interface.h"

#define WINDOW_WIDTH 360
#define WINDOW_HEIGHT 480




#define NUMBER_BUTTON_IN_GAME 2
#define NUMBER_BUTTON_END_GAME 2
#define NUMBER_BUTTON_HISTORY 3

#define SIZE_TILE 75
#define SPACE_BETWEEN_TILE 10
#define MARGIN_WITH_BOARD 5
#define MARGIN_LOGO 5
#define SIZE_LOGO 90

#define NUMBER_FONT 7

/*******************INTERNAL******************/
#define SIZE_BOARD ((SIZE_TILE * 4) + (SPACE_BETWEEN_TILE * 5))
#define RADIUS_TILE (SIZE_TILE / 10)
#define RADIUS_BOARD RADIUS_TILE
#define RADIUS_BUTTON RADIUS_TILE
#define MARGIN_TOP_WITH_BOARD (WINDOW_HEIGHT - SIZE_BOARD - MARGIN_WITH_BOARD)
#define POS_INFO (WINDOW_WIDTH - 2*SIZE_LOGO - 4*MARGIN_LOGO)
#define MARGIN_SCORE_HIGHTSCORE 10
#define MARGIN_HISTORY_BOARD  (WINDOW_HEIGHT - SIZE_BOARD)/2

#define WHITE ((SDL_Color){255, 255, 255, SDL_ALPHA_OPAQUE})
#define BLACK ((SDL_Color){0, 0, 0, SDL_ALPHA_OPAQUE})
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
#define TILE_131072_COLOR ((SDL_Color){255, 215, 0, SDL_ALPHA_OPAQUE})


#define BOARD_COLOR ((SDL_Color){187, 173, 160, SDL_ALPHA_OPAQUE})
#define COLOR_TEXT_SCORE ((SDL_Color){233, 225, 211, SDL_ALPHA_OPAQUE})

#define END_POP_UP_WIDTH (2*(WINDOW_WIDTH /3))
#define END_POP_UP_HEIGHT (WINDOW_HEIGHT /2)


#define INDEX_BUTTON_IN_GAME            0
#define END_INDEX_BUTTON_IN_GAME        NUMBER_BUTTON_IN_GAME // exclu
#define INDEX_BUTTON_END_GAME           END_INDEX_BUTTON_IN_GAME
#define END_INDEX_BUTTON_END_GAME       (INDEX_BUTTON_END_GAME + NUMBER_BUTTON_END_GAME) // exclu
#define INDEX_BUTTON_HISTORY            END_INDEX_BUTTON_END_GAME
#define END_INDEX_BUTTON_HISTORY        (INDEX_BUTTON_HISTORY + NUMBER_BUTTON_HISTORY)

#define NUMBER_BUTTON (NUMBER_BUTTON_IN_GAME + NUMBER_BUTTON_END_GAME + NUMBER_BUTTON_HISTORY)

#define KEY_UP 0
#define KEY_DOWN 1
#define KEY_LEFT 2
#define KEY_RIGHT 3

#define UNUSED(x) ((void)(x))

struct Button
{
    SDL_Texture* texture_button;
    SDL_Rect rect;
    SDL_Color color_button;
    //void (*active_button) (struct GameEngine*, enum GameStatus*);
};

struct Interface_SDL2
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture* text_logo;
    SDL_Texture* text_score;
    TTF_Font *number_font[NUMBER_FONT];
    SDL_Texture *texture_number[16];
    bool key_pressed[4];
    struct Button* buttons[NUMBER_BUTTON];
    int index_history;
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

    for (int i = 1; i < NUMBER_FONT; i++)
    {
        interface->number_font[i] = TTF_OpenFont(font_dir, 58 - 8 * i);
    }
    interface->number_font[0] = interface->number_font[1];

    free(font_dir);

    memset(interface->texture_number, 0, 16 * sizeof(SDL_Texture *));
    memset(interface->key_pressed, 0, 4 * sizeof(bool));
    memset(interface->buttons, 0, sizeof(struct Button*) * NUMBER_BUTTON);
    interface->text_logo = NULL;
    interface->text_score = NULL;

    SDL_Rect pos_button_replay = {
        .x = POS_INFO,
        .y = 2*MARGIN_LOGO + 2*(SIZE_LOGO / 3),
        .w = SIZE_LOGO,
        .h = SIZE_LOGO/3 - MARGIN_LOGO};
    interface->buttons[INDEX_BUTTON_IN_GAME] = initButton(interface->renderer, &pos_button_replay, "Rejouer", interface->number_font[5], (SDL_Color) {255, 255, 255, 255}, TILE_2048_COLOR);

    SDL_Rect pos_button_history = {
        .x = POS_INFO  + SIZE_LOGO + MARGIN_SCORE_HIGHTSCORE,
        .y = 2*MARGIN_LOGO + 2*(SIZE_LOGO / 3),
        .w = SIZE_LOGO,
        .h = SIZE_LOGO/3 - MARGIN_LOGO};
    interface->buttons[INDEX_BUTTON_IN_GAME + 1] = initButton(interface->renderer, &pos_button_history, "Historique", interface->number_font[5], (SDL_Color) {255, 255, 255, 255}, TILE_2048_COLOR);

    SDL_Rect pos_button_restart = {
        .x = WINDOW_WIDTH / 6 + 6,
        .y = WINDOW_HEIGHT / 2 + 20,
        .w = END_POP_UP_WIDTH / 2 - 9,
        .h = WINDOW_HEIGHT / 2 - END_POP_UP_HEIGHT / 2 -26};
    interface->buttons[INDEX_BUTTON_END_GAME] = initButton(interface->renderer, &pos_button_restart, "Rejouer", interface->number_font[3], WHITE, TILE_2048_COLOR);

    SDL_Rect pos_button_quit = {
        .x = WINDOW_WIDTH / 6 + END_POP_UP_WIDTH / 2 + 3,
        .y = WINDOW_HEIGHT / 2 + 20,
        .w = END_POP_UP_WIDTH / 2 - 9,
        .h = WINDOW_HEIGHT / 2 - END_POP_UP_HEIGHT / 2 -26};
    interface->buttons[INDEX_BUTTON_END_GAME +1] = initButton(interface->renderer, &pos_button_quit, "Quitter", interface->number_font[3], WHITE, TILE_2048_COLOR);

    SDL_Rect pos_button_left_arrow = {
        .x = MARGIN_WITH_BOARD +10, 
        .y = WINDOW_HEIGHT / 2 + SIZE_BOARD / 2 + 9,
        .w = 50,
        .h = 50};
    interface->buttons[INDEX_BUTTON_HISTORY] = initButton(interface->renderer, &pos_button_left_arrow, "<", interface->number_font[2], WHITE, TILE_2048_COLOR);

    SDL_Rect pos_button_right_arrow = {
        .x = WINDOW_WIDTH - MARGIN_WITH_BOARD -60, 
        .y = WINDOW_HEIGHT / 2 + SIZE_BOARD / 2 + 9,
        .w = 50,
        .h = 50};
    interface->buttons[INDEX_BUTTON_HISTORY + 1] = initButton(interface->renderer, &pos_button_right_arrow, ">", interface->number_font[2], WHITE, TILE_2048_COLOR);

    int const width_return = 200;
    SDL_Rect pos_button_return = {
        .x = WINDOW_WIDTH / 2 - width_return/2, 
        .y = WINDOW_HEIGHT / 2 + SIZE_BOARD / 2 + 9,
        .w = width_return,
        .h = 50};
    interface->buttons[INDEX_BUTTON_HISTORY + 2] = initButton(interface->renderer, &pos_button_return, "Retour", interface->number_font[2], WHITE, TILE_2048_COLOR);

    interface->index_history = 0;

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

static SDL_Texture* loadBlendedText(
    SDL_Renderer *renderer,
    SDL_Color textcolor,
    TTF_Font *font,
    char const *text)
{
    SDL_Surface *surface_txt = TTF_RenderText_Blended(font, text, textcolor);
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
    return texture_text;
}

static SDL_Texture* loadShadedText(
    SDL_Renderer *renderer,
    SDL_Color background,
    SDL_Color textcolor,
    TTF_Font *font,
    char const *text)
{
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
    return texture_text;
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
    SDL_Texture *texture_text = loadShadedText(renderer, background, textcolor, font, text);
    SDL_Rect txt_rect;
    SDL_QueryTexture(texture_text, NULL, NULL, &(txt_rect.w), &(txt_rect.h));
    txt_rect.x = rect->x + rect->w / 2 - txt_rect.w / 2;
    txt_rect.y = rect->y + rect->h / 2 - txt_rect.h / 2 + 3;

    SDL_RenderCopy(renderer, texture_text, NULL, &txt_rect);
    return texture_text;
}

static void DrawTile(struct Interface_SDL2 *interface, int tile, int index_x, int index_y, int offset_with_top)
{
    SDL_Rect rect_tile;
    rect_tile.x = MARGIN_WITH_BOARD + SPACE_BETWEEN_TILE + (SPACE_BETWEEN_TILE + SIZE_TILE) * index_x;
    rect_tile.y = offset_with_top + SPACE_BETWEEN_TILE + (SPACE_BETWEEN_TILE + SIZE_TILE) * index_y;
    rect_tile.w = SIZE_TILE;
    rect_tile.h = SIZE_TILE;

    if (tile == 0)
    {
        SDL_SetRenderDrawColor(interface->renderer, TILE_EMPTY_COLOR.r, TILE_EMPTY_COLOR.g, TILE_EMPTY_COLOR.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRoudedRect(interface->renderer, &rect_tile, RADIUS_TILE);
        return;
    }
    SDL_Color backgroud[17] = {
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
        TILE_65536_COLOR,
        TILE_131072_COLOR};
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

static void DrawBoard(struct Interface_SDL2 *interface, const int (*board)[4], int offset_with_top)
{
    SDL_Rect rect_board = {.x = MARGIN_WITH_BOARD, .y = offset_with_top, .w = SIZE_BOARD, .h = SIZE_BOARD};
    // SDL_Rect rect_background_board = {.x = 0, .y = MARGIN_TOP_WITH_BOARD - MARGIN_WITH_BOARD, .w = WINDOW_WIDTH, .h = SIZE_BOARD + (2*MARGIN_WITH_BOARD)};
    // SDL_SetRenderDrawColor(interface->renderer, WHITE.r, WHITE.g, WHITE.b, SDL_ALPHA_OPAQUE);
    // SDL_RenderFillRect(interface->renderer, &rect_background_board);
    SDL_SetRenderDrawColor(interface->renderer, BOARD_COLOR.r, BOARD_COLOR.g, BOARD_COLOR.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRoudedRect(interface->renderer, &rect_board, RADIUS_BOARD);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            DrawTile(interface, board[i][j], j, i, offset_with_top);
        }
    }
}

static void DrawLogo(struct Interface_SDL2 *interface)
{
    if (interface->text_logo == NULL)
    {
        interface->text_logo = loadShadedText(interface->renderer, TILE_2048_COLOR, WHITE, interface->number_font[2], "2048");
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

static void DrawScore(struct Interface_SDL2 *interface, int score, int xPos, int yPos, const char* score_name)
{
    SDL_Texture *text_score = loadShadedText(interface->renderer, BOARD_COLOR, COLOR_TEXT_SCORE, interface->number_font[5], score_name);
    SDL_Rect txt_rect;
    SDL_Rect rect_tile = {.x = xPos , .y = yPos, .w = SIZE_LOGO, .h = 2*(SIZE_LOGO / 3)};
    SDL_QueryTexture(text_score , NULL, NULL, &(txt_rect.w), &(txt_rect.h));
    txt_rect.x =  rect_tile.x + rect_tile.w / 2 - txt_rect.w / 2;
    txt_rect.y = rect_tile.y + rect_tile.h / 2 - txt_rect.h / 2 - 12;
    SDL_SetRenderDrawColor(interface->renderer, BOARD_COLOR.r, BOARD_COLOR.g, BOARD_COLOR.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRoudedRect(interface->renderer, &rect_tile, RADIUS_TILE);
    SDL_RenderCopy(interface->renderer, text_score, NULL, &txt_rect);
    SDL_DestroyTexture(text_score);

    char txt_score[12];
    snprintf(txt_score, 12, "%d", score);
    int index_font;
    if (score < 1000)
    {
        index_font = 3;
    }
    else if (score >= 1000000)
    {
        index_font = 5;
    }
    else 
    {
        index_font = 4;
    }

    SDL_Texture* texture_score = loadShadedText(interface->renderer, BOARD_COLOR, WHITE, interface->number_font[index_font], txt_score);
    SDL_Rect score_txt_rect;
    SDL_QueryTexture(texture_score, NULL, NULL, &(score_txt_rect.w), &(score_txt_rect.h));
    score_txt_rect.x =  rect_tile.x + rect_tile.w / 2 - score_txt_rect.w / 2;
    score_txt_rect.y = rect_tile.y + rect_tile.h / 2 - score_txt_rect.h / 2 +12;
    SDL_RenderCopy(interface->renderer, texture_score, NULL, &score_txt_rect);
    SDL_DestroyTexture(texture_score);
}

void DrawScoreEnd(struct Interface_SDL2 *interface, int score)
{
    SDL_Surface *surface_txt = TTF_RenderUTF8_Shaded(interface->number_font[3], "Terminée", BLACK, WHITE);
    if (surface_txt == NULL)
    {
        printf("%s\n", TTF_GetError());
        return;
    }
    SDL_Texture* texture_end = SDL_CreateTextureFromSurface(interface->renderer, surface_txt);
    SDL_FreeSurface(surface_txt);
    if (texture_end == NULL)
    {
        printf("%s\n", TTF_GetError());
        return;
    }
    SDL_Rect end_txt_rect;
    SDL_QueryTexture(texture_end , NULL, NULL, &(end_txt_rect.w), &(end_txt_rect.h));
    end_txt_rect.x =  WINDOW_WIDTH / 2  - end_txt_rect.w / 2;
    end_txt_rect.y = WINDOW_HEIGHT / 2 - END_POP_UP_HEIGHT / 2 + 10;
    SDL_RenderCopy(interface->renderer, texture_end, NULL, &end_txt_rect);
    SDL_DestroyTexture(texture_end);

    int height_first_line = end_txt_rect.h;
    texture_end = loadShadedText(interface->renderer, WHITE, BLACK, interface->number_font[4], "Votre score est de :");
    SDL_QueryTexture(texture_end , NULL, NULL, &(end_txt_rect.w), &(end_txt_rect.h));
    end_txt_rect.x =  WINDOW_WIDTH / 2  - end_txt_rect.w / 2;
    end_txt_rect.y = WINDOW_HEIGHT / 2 - END_POP_UP_HEIGHT / 2 + height_first_line + 15;
    SDL_RenderCopy(interface->renderer, texture_end, NULL, &end_txt_rect);
    SDL_DestroyTexture(texture_end);
    
    height_first_line += end_txt_rect.h;

    char txt_score[12];
    snprintf(txt_score, 12, "%d", score);
    SDL_Texture* texture_score = loadShadedText(interface->renderer, WHITE, BLACK, interface->number_font[2], txt_score);
    SDL_Rect score_txt_rect;
    SDL_QueryTexture(texture_score, NULL, NULL, &(score_txt_rect.w), &(score_txt_rect.h));
    score_txt_rect.x =  WINDOW_WIDTH / 2  - score_txt_rect.w / 2;
    score_txt_rect.y = WINDOW_HEIGHT / 2 - END_POP_UP_HEIGHT / 2 + height_first_line + 20;
    SDL_RenderCopy(interface->renderer, texture_score, NULL, &score_txt_rect);
    SDL_DestroyTexture(texture_score);
}

void popUp(struct Interface_SDL2 *interface, int width, int height) {
    SDL_Rect background;
    background.x = 0;
    background.y = 0;
    background.w = WINDOW_WIDTH;
    background.h = WINDOW_HEIGHT;

    SDL_SetRenderDrawColor(interface->renderer, 0, 0, 0, 150);
    SDL_SetRenderDrawBlendMode(interface->renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(interface->renderer, &background);

    background.w = width;
    background.h = height;
    background.x = (WINDOW_WIDTH/2) - background.w/2;
    background.y = (WINDOW_HEIGHT/2) - background.h/2;

    SDL_SetRenderDrawColor(interface->renderer, WHITE.r, WHITE.g, WHITE.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(interface->renderer, &background);
    SDL_SetRenderDrawColor(interface->renderer, BLACK.r, BLACK.g, BLACK.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(interface->renderer, &background);
    SDL_SetRenderDrawBlendMode(interface->renderer, SDL_BLENDMODE_NONE);
}

static void updateInGame(struct Interface_SDL2 *interface, struct GameEngine const *gameEngine)
{
    DrawLogo(interface);
    DrawScore(interface, gameEngine->score, POS_INFO, MARGIN_LOGO, "SCORE");
    if (gameEngine->gameHistory.game[0] != NULL && gameEngine->gameHistory.game[0]->score > gameEngine->score)
    {
        DrawScore(interface, gameEngine->gameHistory.game[0]->score, POS_INFO + SIZE_LOGO + MARGIN_SCORE_HIGHTSCORE, MARGIN_LOGO, "MEILLEUR");
    }
    else
    {
        DrawScore(interface, gameEngine->score, POS_INFO + SIZE_LOGO + MARGIN_SCORE_HIGHTSCORE, MARGIN_LOGO, "MEILLEUR");
    }
    for (int i = INDEX_BUTTON_IN_GAME; i < END_INDEX_BUTTON_IN_GAME; i++)
    {
        renderButton(interface->renderer, interface->buttons[i]);
    }
    DrawBoard(interface, gameEngine->board, MARGIN_TOP_WITH_BOARD);
}

static void updateEndGame(struct Interface_SDL2 *interface, struct GameEngine const *gameEngine)
{
    updateInGame(interface, gameEngine);
    popUp(interface, END_POP_UP_WIDTH, END_POP_UP_HEIGHT);
    DrawScoreEnd(interface, gameEngine->score);
    for (int i = INDEX_BUTTON_END_GAME; i < END_INDEX_BUTTON_END_GAME; i++)
    {
        renderButton(interface->renderer, interface->buttons[i]);
    }
}

static void updateHistory(struct Interface_SDL2 *interface, struct GameEngine const *gameEngine)
{
    updateInGame(interface, gameEngine);
    
    if (gameEngine->gameHistory.game[interface->index_history] != NULL)
    {
        //popUp(interface, WINDOW_WIDTH - 2*MARGIN_WITH_BOARD + 6, SIZE_BOARD+7);
        SDL_Rect background;
        background.x = 0;
        background.y = 0;
        background.w = WINDOW_WIDTH;
        background.h = WINDOW_HEIGHT;//MARGIN_HISTORY_BOARD - 3;

        SDL_SetRenderDrawColor(interface->renderer, 0, 0, 0, 200);
        SDL_SetRenderDrawBlendMode(interface->renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(interface->renderer, &background);
        SDL_SetRenderDrawBlendMode(interface->renderer, SDL_BLENDMODE_NONE);
        struct PastGame const* game = gameEngine->gameHistory.game[interface->index_history];
        SDL_Texture* text = loadBlendedText(interface->renderer, WHITE, interface->number_font[3], "Vos records");
        SDL_Rect rect_text;
        SDL_QueryTexture(text, NULL, NULL, &rect_text.w, &rect_text.h);
        rect_text.x = WINDOW_WIDTH /2 - rect_text.w /2;
        rect_text.y = 5;
        SDL_RenderCopy(interface->renderer, text, NULL, &rect_text);
        SDL_DestroyTexture(text);


        char txt_score[30];
        if (interface->index_history == 0)
        {
            snprintf(txt_score, 30, "Meilleur score : %d", gameEngine->gameHistory.game[interface->index_history]->score);
        }
        else
        {
            snprintf(txt_score, 30, "Score : %d", gameEngine->gameHistory.game[interface->index_history]->score);
        }
        text = loadBlendedText(interface->renderer, WHITE, interface->number_font[4], txt_score);
        SDL_QueryTexture(text, NULL, NULL, &rect_text.w, &rect_text.h);
        rect_text.x = WINDOW_WIDTH /2 - rect_text.w /2;
        rect_text.y = 35;
        SDL_RenderCopy(interface->renderer, text, NULL, &rect_text);
        SDL_DestroyTexture(text);


        DrawBoard(interface, game->board, MARGIN_HISTORY_BOARD);
    }
    else
    {
        popUp(interface, WINDOW_WIDTH - 2*MARGIN_WITH_BOARD + 6, 100);
        SDL_Texture* text = loadShadedText(interface->renderer, WHITE, BLACK, interface->number_font[3], "Vous n'avez pas encore");
        SDL_Rect rect_text;
        SDL_QueryTexture(text, NULL, NULL, &rect_text.w, &rect_text.h);
        rect_text.x = WINDOW_WIDTH /2 - rect_text.w /2;
        rect_text.y = WINDOW_HEIGHT /2 - rect_text.h /2 -20;
        SDL_RenderCopy(interface->renderer, text, NULL, &rect_text);
        SDL_DestroyTexture(text);

        text = loadShadedText(interface->renderer, WHITE, BLACK, interface->number_font[3], "fait de partie");
        SDL_QueryTexture(text, NULL, NULL, &rect_text.w, &rect_text.h);
        rect_text.x = WINDOW_WIDTH /2 - rect_text.w /2;
        rect_text.y = WINDOW_HEIGHT /2 - rect_text.h /2 + 20;
        SDL_RenderCopy(interface->renderer, text, NULL, &rect_text);
        SDL_DestroyTexture(text);
    }

    for (int i = INDEX_BUTTON_HISTORY; i < END_INDEX_BUTTON_HISTORY; i++)
    {
        renderButton(interface->renderer, interface->buttons[i]);
    }
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
    case END_MENU:
        updateEndGame(inter, gameEngine);
        break;
    case HISTORY_MENU:
        updateHistory(interface, gameEngine);
        break;
    default:
        break;
    }
    SDL_RenderPresent(inter->renderer);
}

static void replayButton(struct GameEngine *gameEngine)
{
    resetGameEngine(gameEngine);
}

enum Interactions getInteraction(Interface *interface, enum GameStatus *status, struct GameEngine *gameEngine)
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
                if (*status == IN_GAME && !inter->key_pressed[KEY_UP])
                { 
                    inter->key_pressed[KEY_UP] = true;
                    return INTERACTION_MOVE_UP;
                }
                break;
            case SDL_SCANCODE_DOWN:
                if (*status == IN_GAME && !inter->key_pressed[KEY_DOWN])
                { 
                    inter->key_pressed[KEY_DOWN] = true;
                    return INTERACTION_MOVE_DOWN;
                }
                break;
            case SDL_SCANCODE_LEFT:
                if (*status == IN_GAME && !inter->key_pressed[KEY_LEFT])
                { 
                    inter->key_pressed[KEY_LEFT] = true;
                    return INTERACTION_MOVE_LEFT;
                }
                if (*status == HISTORY_MENU && !inter->key_pressed[KEY_LEFT])
                { 
                    if (inter->index_history > 0 && gameEngine->gameHistory.game[inter->index_history-1] != NULL)
                    {
                        inter->index_history--;
                        update(interface, *status, gameEngine);  
                    }
                }
                break;
            case SDL_SCANCODE_RIGHT:
                if (*status == IN_GAME && !inter->key_pressed[KEY_RIGHT])
                { 
                    inter->key_pressed[KEY_RIGHT] = true;
                    return INTERACTION_MOVE_RIGHT;
                }
                if (*status == HISTORY_MENU && !inter->key_pressed[KEY_RIGHT])
                { 
                    if (inter->index_history < NUMBER_SAVE_GAME - 1 && gameEngine->gameHistory.game[inter->index_history+1] != NULL)
                    {
                        inter->index_history++;
                        update(interface, *status, gameEngine); 
                    }
                }
                break;
            case SDL_SCANCODE_RETURN:
            case SDL_SCANCODE_KP_ENTER:
                if ((*status) == END_MENU)
                {
                    *status = IN_GAME;
                    replayButton(gameEngine);
                    update(interface, *status, gameEngine); 
                }
                break;
            case SDL_SCANCODE_ESCAPE:
                return INTERACTION_QUIT;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_UP:
                inter->key_pressed[KEY_UP] = false;
                break;
            case SDL_SCANCODE_DOWN:
                inter->key_pressed[KEY_DOWN] = false;
                break;
            case SDL_SCANCODE_LEFT:
                inter->key_pressed[KEY_LEFT] = false;
                break;
            case SDL_SCANCODE_RIGHT:
                inter->key_pressed[KEY_RIGHT] = false;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                if (*status == IN_GAME)
                {
                    if (checkIfPressedButton(inter->buttons[INDEX_BUTTON_IN_GAME], event.button.x, event.button.y))
                    {
                        replayButton(gameEngine);
                        update(interface, *status, gameEngine); 
                    }

                    if (checkIfPressedButton(inter->buttons[INDEX_BUTTON_IN_GAME + 1], event.button.x, event.button.y))
                    {
                        *status = HISTORY_MENU;
                        update(interface, *status, gameEngine); 
                    }
                }
                else if (*status == END_MENU)
                {
                    if (checkIfPressedButton(inter->buttons[INDEX_BUTTON_END_GAME], event.button.x, event.button.y))
                    {
                        *status = IN_GAME;
                        replayButton(gameEngine);
                        update(interface, *status, gameEngine); 
                    }

                    if (checkIfPressedButton(inter->buttons[INDEX_BUTTON_END_GAME + 1], event.button.x, event.button.y))
                    {
                        return INTERACTION_QUIT;
                    }
                }
                else if (*status == HISTORY_MENU)
                {
                    if (checkIfPressedButton(inter->buttons[INDEX_BUTTON_HISTORY], event.button.x, event.button.y))
                    {
                        if (inter->index_history > 0 && gameEngine->gameHistory.game[inter->index_history-1] != NULL)
                        {
                            inter->index_history--;
                            update(interface, *status, gameEngine);  
                        }
                    }

                    if (checkIfPressedButton(inter->buttons[INDEX_BUTTON_HISTORY+1], event.button.x, event.button.y))
                    {
                        if (inter->index_history < NUMBER_SAVE_GAME - 1 && gameEngine->gameHistory.game[inter->index_history+1] != NULL)
                        {
                            inter->index_history++;
                            update(interface, *status, gameEngine); 
                        }
                    }
                    if (checkIfPressedButton(inter->buttons[INDEX_BUTTON_HISTORY + 2], event.button.x, event.button.y))
                    {
                        *status = IN_GAME;
                        inter->index_history = 0;
                        update(interface, *status, gameEngine);
                    }
                }
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
    if (inter->text_logo)
    {
        SDL_DestroyTexture(inter->text_logo);
    }
    if (inter->text_score)
    {
        SDL_DestroyTexture(inter->text_score);
    }
    for (int i = 1; i < NUMBER_FONT; i++)
    {
        if (inter->number_font[i])
        {
            TTF_CloseFont(inter->number_font[i]);
        }
    }
    for (int i = 0; i < NUMBER_BUTTON; i++)
    {
        if (inter->buttons[i])
        {
            freeButton(inter->buttons[i]);
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