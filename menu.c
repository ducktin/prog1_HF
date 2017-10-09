#include "menu.h"
#include "game.h"

void menu(SDL_Surface *screen){
    SDL_Event event;
    Menu kivalasztott = NEW_GAME;
    int quit = 0;
    char* menus[] = {
		"(N)ew game",
		"(H)ighscores",
		"(A)bout",
		"(Q)uit"
    };
    int menu_count = 4;

    while(!quit){
		draw_menu(screen, menus, menu_count, kivalasztott);
        SDL_WaitEvent(&event);
        switch(event.type){
        case SDL_KEYUP:
        	switch(event.key.keysym.sym){
				case SDLK_UP:
					if(kivalasztott == NEW_GAME)
						kivalasztott = QUIT;
					else
						kivalasztott -= 1;
					break;
				case SDLK_DOWN:
					if(kivalasztott == QUIT)
						kivalasztott = NEW_GAME;
					else
						kivalasztott += 1;
					break;
				case SDLK_SPACE:
					switch(kivalasztott){
					case NEW_GAME:
						new_game(screen);
						break;
					case HIGHSCORES: break;
					case ABOUT: break;
					case QUIT:
						quit = 1;
						break;
					}
					break;
				case SDLK_n:
					new_game(screen);
					break;
				case SDLK_h:
					//highscores(screen);
					break;
				case SDLK_a:
					//about(screen);
					break;
				case SDLK_q:
					quit = 1;
					break;
				default:
					break;
        	}
        	break;
        case SDL_QUIT:
            quit = 1;
            break;
        }
    }
}
void draw_menu(SDL_Surface *screen, char* menus[], int menu_count, Menu kivalasztott){
	SDL_Color menu_color = {255,255,255};
	SDL_Rect hova = {0,0,0,0};
	TTF_Font *font_60, *font_40;
	SDL_Surface *felirat;
	int i, cim_m;

	boxColor(screen, 0,0,screen->w-1,screen->h-1,0x000000ff);

	TTF_Init();
	font_60 = TTF_OpenFont("arial.ttf", 60);
	font_40 = TTF_OpenFont("arial.ttf", 40);
	if (!font_60 || !font_40) {
        fprintf(stderr, "Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(2);
    }
    felirat = TTF_RenderUTF8_Blended(font_60, "Sokoban", menu_color);
    cim_m = felirat->h + 80;
    hova.x = (screen->w - felirat->w)/2;
    hova.y = 20;
    SDL_BlitSurface(felirat, NULL, screen, &hova);
    SDL_FreeSurface(felirat);

	// Menu elemek kiírása
    for(i = 0; i < menu_count; ++i){
		felirat = TTF_RenderUTF8_Blended(font_40, menus[i], menu_color);
		hova.x = (screen->w - felirat->w)/2;
		hova.y = i*20 + i*felirat->h + cim_m;
		SDL_BlitSurface(felirat, NULL, screen, &hova);
		if(kivalasztott == i){
			filledTrigonRGBA(screen, screen->w - 100, hova.y+(felirat->h/2), screen->w - 50, hova.y, screen->w - 50, hova.y+felirat->h, menu_color.r, menu_color.g, menu_color.b, 255);
		}
		SDL_FreeSurface(felirat);
    }
	TTF_CloseFont(font_60);
	TTF_CloseFont(font_40);

    SDL_Flip(screen);
}
