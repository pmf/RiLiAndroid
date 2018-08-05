//      (_||_/
//      (    )       Programme Principale
//     ( o  0 )
//-OOO°--(_)---°OOO---------------------------------------
//                   Copyright (C) 2006 By Dominique Roux-Serret
// .OOOo      oOOO.  roux-serret@ifrance.com
//-(   )------(   )---------------------------------------
//  ( (        ) /   Le 03/01/2006
//   (_)      (_/

//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 or version 3 of the License.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <SDL.h>
//#include <SDL_mixer.h>

#include "preference.h"
#include "jeux.h"
#include "audio.h"
#include "sprite.h"
#include "ecran.h"
#include "mouse.h"
#include "menu.h"
#include "tableau.h"
#include "editeur.h"
#include "utils.h"

/*** Variables globales ***/
/************************/
SDL_Window *screen;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Surface *sdlVideo; // Pointe sur l'écran video
Uint32 FontColor;      // Couleur du fond d'écran

char Titre[] = "Ri-li V2.0.1";

Sprite *Sprites = NULL; // Pointe sur les sprites
int NSprites = 0; // Nombre de sprites en mémoire
Ecran Ec[2];          // Pointe sur les 2 buffets vidéo
sPreference Pref;     // Tableau des préférences.
Jeux Jeu;             // Gère le jeu
Mouse Sourie;         // Gère les mouvements de sourie
Menu MenuPrincipale;  // Gère les menus
Tableau Niveau;       // Gère les niveaux
Editeur Edit;         // Gère le menu de l'éditeur
					  //Audio Sons;           // Gère les sons

int Horloge = 0; // Horloges du jeu
int HorlogeAvant = 0;

void DoRender()
{
	SDL_UpdateTexture(texture, NULL, sdlVideo->pixels, sdlVideo->pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

#ifdef LINUX
char DefPath[256]; // Chemin par defaut dans arg
#endif

				   /*** Initialise les preferences ***/
				   /**********************************/
void InitPref(void)
{
#ifdef LINUX
	DefPath[0] = 0;
#endif

	Pref.NiveauMax = 0;
	Pref.FullScreen = false;
	Pref.Langue = -1;
	Pref.Volume = (float)SDL_MIX_MAXVOLUME;
	Pref.VolumeM = (float)SDL_MIX_MAXVOLUME; //*6.0/10.0;

	for (int i = 0; i<8; i++) { // Vide les scores
		Pref.Sco[i].Score = 0;
		Pref.Sco[i].Name[0] = 0;
	}
	/*   Pref.Sco[0].Score=11425; */
	/*   sprintf(Pref.Sco[0].Name,"%s","Dominique"); */
	/*   Pref.Sco[1].Score=678; */
	/*   sprintf(Pref.Sco[1].Name,"%s","Veronique"); */

	LoadPref();

	Pref.Difficulte = Normal;
	Pref.Vitesse = VITESSE_MOY;
	Pref.VitesseMoy = VITESSE_MOY;
	Pref.NVie = N_VIES_DEP;
	Pref.EcartWagon = ECARTWAGON_MOY;
}

/*** Preogramme principale ***/
/*****************************/
int main(int narg, char *argv[])
{
	int i;
	Sprite Spr;
	eMenu RetM, RetMenu = mMenu;

	// Initialuse les préferences
	InitPref();
#ifdef LINUX
	if (narg>1) strcpy(DefPath, argv[1]);
#endif

	// Initilise SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0) {
		std::cerr << "Impossible d'initialiser SDL:" << SDL_GetError() << std::endl;
		exit(-1);
	}
	// Ferme le programme correctement quant quit
	atexit(SDL_Quit);


	// Demande la resolution Video
#ifndef LINUX
	int vOption = SDL_WINDOW_OPENGL;
#else
#ifndef __AMIGAOS4__
	int vOption = SDL_SWSURFACE | SDL_DOUBLEBUF;
#else
	int vOption = SDL_SWSURFACE;
#endif
#endif
	//if (Pref.FullScreen) vOption |= SDL_WINDOW_FULLSCREEN;

	screen = SDL_CreateWindow(
		Titre,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		600,
		0);

	if (screen == NULL) {
		std::cerr << "Impossible de passer dans le mode vidéo 800x600 !" << std::endl;
		exit(-1);
	}

	renderer = SDL_CreateRenderer(screen, -1, 0);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	sdlVideo = SDL_CreateRGBSurfaceWithFormat(0, 800, 600, 32, SDL_PIXELFORMAT_BGRA32);
	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		800,
		600);

	DoRender();


	if (LoadSprites() == false) exit(-1);
	if (Niveau.Load() == false) exit(-1);

	// Initialise l'horloge et le hazard
	HorlogeAvant = Horloge = SDL_GetTicks();
	srand(SDL_GetTicks());
	// Si pas de langues demande la langue
	//if (Pref.Langue == -1) MenuPrincipale.SDLMain_Langue();

	RetMenu = mJeux;

	// Gère les menus
	do {
		switch (RetMenu) {
		case mMenu:
			RetM = MenuPrincipale.SDLMain();
			break;
		case mLangue:
			RetM = MenuPrincipale.SDLMain_Langue();
			break;
		case mOption:
			RetM = MenuPrincipale.SDLMain_Options();
			break;
		case mScoreEdit:
			RetM = MenuPrincipale.SDLMain_Score(true);
			break;
		case mScore:
			RetM = MenuPrincipale.SDLMain_Score();
			break;
		case mMenuSpeed:
			RetM = MenuPrincipale.SDLMain_Speed();
			break;
		case mMenuNiveau:
			RetM = MenuPrincipale.SDLMain_Niveau();
			break;
		case mJeux:
			//Sons.LoadMusic(1);
			RetM = Jeu.SDLMain();
			//Sons.LoadMusic(0);
			break;
		case mEdit:
			RetM = Edit.SDLMain(0);
			break;
		default:
			RetM = mQuit;
		}
		RetMenu = RetM;
	} while (RetMenu != mQuit);


	for (i = 0; i<NSprites; i++) { // Efface les sprites
		Sprites[i].Delete();
	}

	delete[] Sprites;

	SauvePref(); // Sauve les preferences

	exit(0);
}
