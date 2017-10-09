#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <SDL_gfxPrimitives.h>



enum {MERET = 50, KERET = 20, MAX_SZ = 15, MAX_M = 12};

typedef enum {
	SIMA,
	CEL
} Padlo;
typedef enum {
	URES,
	FAL,
	DOBOZ,
	JATEKOS
} Object;

typedef enum Irany{
	FEL,
	JOBB,
	LE,
	BAL
} Irany;

typedef struct Vissza_elem{
	int tolt_e;
	Irany irany;
	struct Vissza_elem *kov;
} Vissza_elem;

typedef struct Mezo{
	Padlo padlo;
	Object object;
} Mezo;
typedef struct Poz{
	int x;
	int y;
} Poz;
typedef struct Palya{
	Mezo** mezok;
	int magassag;
	int szelesseg;
	Poz jatekos_poz;
	//int tolasok;
} Palya;

void new_game(SDL_Surface *screen);
Palya load(char* fajlnev);
void game(SDL_Surface *screen, Palya palya);

int palyapos(int koord);
void mezo_rajzol(SDL_Surface *forraskep, SDL_Surface *celkep, Mezo mezo, int x, int y);
void palya_rajzol(Palya palya, SDL_Surface *forraskep, SDL_Surface *celkep);

int mozgas(Palya *palya, int vertical, int horizontal);
void lep(Palya *palya, int vertical, int horizontal);
void tol(Palya *palya, int vertical, int horizontal);

int ellenoriz(Palya palya);
int vissza(Palya *palya, Vissza_elem *verem);



#endif // GAME_H_INCLUDED
