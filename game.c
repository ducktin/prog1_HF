#include "game.h"

void new_game(SDL_Surface *screen){
	Palya palya;

	// rákérdezni mappascan-re

	// Kivalaszt melyikkel jatszik

	// Betölti a kivalasztottat (load())
	palya = load("1.txt");

	// Elindítja a jatekot (game())
	game(screen, palya);
}
Palya load(char* fajlnev){
	// fajl beolvas
	int i,j;
	FILE *file;
	Palya palya;
	char mezo;

    file = fopen(fajlnev, "rt");
    if(!file){
		fprintf(stderr, "Nem sikerult megnyitni a fajlt!\n");
		exit(2);
    }
    fscanf(file,"%d %d\n", &palya.magassag, &palya.szelesseg);

	// mezok lefoglal
	palya.mezok = (Mezo**) malloc(palya.magassag * sizeof(Mezo*));
	if(!palya.mezok){
		fprintf(stderr, "Hiba a memoriafoglalas kozben!\n");
        exit(2);
	}
	for(i = 0; i < palya.magassag; ++i){
		palya.mezok[i] = (Mezo*) malloc(palya.szelesseg * sizeof(Mezo));
		if(!palya.mezok[i]){
			fprintf(stderr, "Hiba a memoriafoglalas kozben!\n");
			exit(2);
		}
	}

	// fajlbol feltolt
	i = 0; j = 0;
	while ((mezo = fgetc(file)) != EOF) {
		switch(mezo){
		case '_':
			palya.mezok[i][j].padlo = SIMA;
			palya.mezok[i][j].object= URES;
			break;
		case '#':
			palya.mezok[i][j].padlo = SIMA;
			palya.mezok[i][j].object= FAL;
			break;
		case '@':
			palya.mezok[i][j].padlo = SIMA;
			palya.mezok[i][j].object= JATEKOS;
			palya.jatekos_poz.x = j;
			palya.jatekos_poz.y = i;
			break;
		case '+':
			palya.mezok[i][j].padlo = CEL;
			palya.mezok[i][j].object= JATEKOS;
			palya.jatekos_poz.x = j;
			palya.jatekos_poz.y = i;
			break;
		case '$':
			palya.mezok[i][j].padlo = SIMA;
			palya.mezok[i][j].object= DOBOZ;
			break;
		case '*':
			palya.mezok[i][j].padlo = CEL;
			palya.mezok[i][j].object= DOBOZ;
			break;
		case '.':
			palya.mezok[i][j].padlo = CEL;
			palya.mezok[i][j].object= URES;
			break;
		case '\n':
			i++;
			j = -1;
			break;
		default:
			fprintf(stderr, "A megnyitott fajlnak rossza a formatuma!\n");
			exit(2);
			break;
		}
		j++;
	}
	return palya;
}
void game(SDL_Surface *screen, Palya palya){
	int i, done = 0;
	SDL_Event event;
	// grafikák betöltése
	SDL_Surface *rajzok = IMG_Load("sokoban.png");
	if (!rajzok) {
        fprintf(stderr, "Nem sikerult betolteni a kepfajlt!\n");
        exit(2);
    }
	// zajlik a jatek-> hozzaadja a veremhez a leheseket (tolt_e, Irany)

	//visszaverem


	while(!done){
		palya_rajzol(palya, rajzok, screen);
		SDL_WaitEvent(&event);
		switch(event.type){
		case SDL_KEYUP:
			switch(event.key.keysym.sym){
			case SDLK_UP:
				done = mozgas(&palya, -1, 0); // palya, y, x
				break;
			case SDLK_RIGHT:
				done = mozgas(&palya, 0, 1);
				break;
			case SDLK_DOWN:
				done = mozgas(&palya, 1, 0);
				break;
			case SDLK_LEFT:
				done = mozgas(&palya, 0, -1);
				break;
			case SDLK_BACKSPACE:
				//vissza(&palya, &verem);
				break;
			default:
				break;
			}
			break;
		case SDL_QUIT:
			//done = biztos(screen);
			break;
		}
	}

	// felszabadit

    SDL_FreeSurface(rajzok);
	for(i = 0; i < palya.magassag; ++i){
		free(palya.mezok[i]);
	}
	free(palya.mezok);
}
int palyapos(int koord) {
    return MERET*koord + KERET;
}
void mezo_rajzol(SDL_Surface *forraskep, SDL_Surface *celkep, Mezo mezo, int x, int y){
	SDL_Rect src = { 0, 0, MERET, MERET };
    SDL_Rect dest = { x*MERET + KERET, y*MERET + KERET, 0, 0 };
    switch(mezo.padlo){
	case SIMA:
		src.y = 50;
		break;
	case CEL:
		src.y = 100;
		break;
    }
    SDL_BlitSurface(forraskep, &src, celkep, &dest);
    src.x = 50;
    switch(mezo.object){
	case URES:
		return;
	case FAL:
		src.y = 100 ;
		break;
	case DOBOZ:
		src.y = 50;
		break;
	case JATEKOS:
		src.y = 0;
		break;
    }
    SDL_BlitSurface(forraskep, &src, celkep, &dest);
}
void palya_rajzol(Palya palya, SDL_Surface *forraskep, SDL_Surface *celkep){
	int x,y;

	boxColor(celkep, 0, 0, celkep->w-1, celkep->h-1, 0x000000ff);
	rectangleColor(celkep, palyapos(0)-1, palyapos(0)-1, palyapos(8), palyapos(8), 0x00000080);

    for (y=0; y<palya.magassag; y++)
        for (x=0; x<palya.szelesseg; x++)
            mezo_rajzol(forraskep, celkep, palya.mezok[y][x], x, y);
	SDL_Flip(celkep);
}

int mozgas(Palya *palya, int vertical, int horizontal){
	if(palya->mezok[palya->jatekos_poz.y + vertical][palya->jatekos_poz.x + horizontal].object == URES){
		lep(palya, vertical, horizontal);
	}
	else if(palya->mezok[palya->jatekos_poz.y + vertical][palya->jatekos_poz.x + horizontal].object == DOBOZ &&
			palya->mezok[palya->jatekos_poz.y + vertical*2][palya->jatekos_poz.x + horizontal*2].object == URES){
		tol(palya, vertical, horizontal);
	}
	return ellenoriz(*palya);
}
void lep(Palya *palya, int vertical, int horizontal){
	palya->mezok[palya->jatekos_poz.y][palya->jatekos_poz.x].object = URES;
	palya->jatekos_poz.y += vertical;
	palya->jatekos_poz.x += horizontal;
	palya->mezok[palya->jatekos_poz.y][palya->jatekos_poz.x].object = JATEKOS;
}
void tol(Palya *palya, int vertical, int horizontal){
	lep(palya, vertical, horizontal);
	palya->mezok[palya->jatekos_poz.y + vertical][palya->jatekos_poz.x + horizontal].object = DOBOZ;
}

int ellenoriz(Palya palya){
	int i, j;
	for(i = 0; i < palya.magassag; ++i){
		for(j = 0; j < palya.szelesseg; ++j){
			if(palya.mezok[i][j].padlo == CEL && palya.mezok[i][j].object != DOBOZ){
				return 0;
			}
		}
	}
	return 0;
}

//int biztos(SDL_Surface *screen){
//	int kivalasztott = -1;
//}

//// Visszaadja a verem új első elemét
//Vissza_elem *push(Vissza_elem *top, int tolt_e, Irany irany){
//	Vissza_elem *uj = (Vissza_elem*) malloc(sizeof(Vissza_elem));
//	uj->tolt_e = tolt_e;
//	uj->irany = irany;
//	uj->kov = top;
//	return uj;
//}
//// Visszaadaja az új első elemet
//Vissza_elem *pop(Vissza_elem *top){
//	Vissza_elem uj;
//	*torolt = top;
//	uj = top->kov;
//	free(top)
//	return uj;
//}


//int vissza(Palya *palya, Vissza_verem *verem){
//	if(verem->db == 0)
//		return 0;
//
//	if(verem->mozgasok[db-1].irany == FEL && verem->mozgasok[db-1].tolt_e){
//		le_tol(palya);
//	}
//	else if(verem->mozgasok[db-1].irany == FEL && !verem->mozgasok[db-1].tolt_e){
//		le_mozog(palya);
//	}
//	else if(verem->mozgasok[db-1].irany == JOBB && verem->mozgasok[db-1].tolt_e){
//		bal_tol(palya);
//	}
//	else if(verem->mozgasok[db-1].irany == JOBB && !verem->mozgasok[db-1].tolt_e){
//		bal_mozog(palya);
//	}
//	else if(verem->mozgasok[db-1].irany == LE && verem->mozgasok[db-1].tolt_e){
//		fel_tol(palya);
//	}
//	else if(verem->mozgasok[db-1].irany == LE && !verem->mozgasok[db-1].tolt_e){
//		fel_mozog(palya);
//	}
//	else if(verem->mozgasok[db-1].irany == BAL && verem->mozgasok[db-1].tolt_e){
//		jobbra_tol(palya);
//	}
//	else if(verem->mozgasok[db-1].irany == BAL && !verem->mozgasok[db-1].tolt_e){
//		jobbra_mozog(palya);
//	}
//}
