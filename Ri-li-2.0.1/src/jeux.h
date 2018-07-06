//      (_||_/
//      (    )       Classe Jeux
//     ( o  0 )
//-OOO�--(_)---�OOO---------------------------------------
//                   Copyright (C) 2006 By Dominique Roux-Serret
// .OOOo      oOOO.  roux-serret@ifrance.com
//-(   )------(   )---------------------------------------
//  ( (        ) /   Le 01/02/2006
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

#ifndef _JEUX_DOM_
#define _JEUX_DOM_

#include "preference.h"
#include "loco.h"

/*** D�finition de la class ***/
/******************************/
class Jeux {
 public:
  Jeux(void);
  ~Jeux(void);

  /*** Fonctions ***/
  /*****************/
  eMenu SDLMain(void);   // Boucle principale
  bool Load(int NiveauN); // Charge un tableau
  bool DrawLevel(int NiveauN);   // Dessine le niveau
  void PrendTouche(int Touche); // Prend les touches enfonc�es

  void TourneFleche(void); // Fait tourner la fleche d'un simple clic.
  void BufTouche(int Touche); // Met une nouvelle touche dans le buffet
  int TestFleche(int Haut,int Bas,int Gauche,int Droite); // Test la direction de sortie de la fleche
  void AfficheEcran(void);    // Fait l'affichage d'un ecran de jeu

  /*** Variables ***/
  bool Pause; // Si en pose

  long DureeJeu; // Horloge de dur�e du jeu 

  int Key;       // M�morise la touche enfonc�e
  int Touche[4]; // Direction demand�e
  int T[LT*HT]; // Pointe sur le tableau

  Loco Lo;      // G�re la locomotive

  int NumSS; // Num�ro du Screen shot
  int NumVideo; // Num�ro de l'ecran affich�
  bool Help; // Si doit affiche les fleches d'aide
};
#endif
