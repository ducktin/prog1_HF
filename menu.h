#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
typedef enum{
    NEW_GAME,
    HIGHSCORES,
    ABOUT,
    QUIT
} Menu;

void menu(SDL_Surface *screen);
void draw_menu(SDL_Surface *screen, char* menus[], int menu_count, Menu kivalasztott);

#endif // MENU_H_INCLUDED
