#include <SDL.h>
#include "menu.h"
#include "game.h"

int main(int argc, char *argv[]) {
    SDL_Surface *screen;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    screen = SDL_SetVideoMode(MERET*MAX_SZ+KERET*2, MERET*MAX_M+KERET*2, 0, SDL_ANYFORMAT);
    if (!screen) {
        fprintf(stderr, "Nem sikerult megnyitni az ablakot!\n");
        exit(1);
    }
    SDL_WM_SetCaption("Sokoban", "Sokoban");

    menu(screen);

    SDL_Quit();

    return 0;
}
