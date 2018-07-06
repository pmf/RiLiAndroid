//      (_||_/
//      (    )       Classe Loco (G�re la locomotive)
//     ( o  0 )
//-OOO�--(_)---�OOO---------------------------------------
//                   Copyright (C) 2006 By Dominique Roux-Serret
// .OOOo      oOOO.  roux-serret@ifrance.com
//-(   )------(   )---------------------------------------
//  ( (        ) /   Le 16/01/2006
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

#ifndef _LOCO_DOM_
#define _LOCO_DOM_

#include <SDL/SDL.h>
#include "sprite.h"
#include "ecran.h"
#include "preference.h"

/*** Definition de la classe qui m�morise les cases ***/
/******************************************************/
struct s_TLoco {
  int P; // Pointe sur le N� de la case
  float D;  // Distance parcoure (case comprie)
  unsigned char Arrive,Sortie;  // Direction d'arriv�e et de sortie
};

struct s_PosWagon { // M�morise la position avant et arri�re des wagons pour le test de colision
  int dx,dy;
  int fx,fy;
  float SprStart;     // Si fini sa sequence d'affiche de d�part
};
#define N_SPR_START 50

/*** D�finition de la class ***/
/******************************/
class Loco
{
 public:
  Loco(void);
  ~Loco(void);

  /*** Fonctions ***/
  void Init(int Pos,int Direction); // Initialise la loco sur le tableau
  void Affiche(Ecran &Ec); // Fait l'affichage de la loco.
  void TestCase(float Duree,long DureeJeu,int *Tableau); // Test les options sur la case si passe au centre
  void Avance(int Dureems,long DureeJeu,int *Touche,int *Tableau); // Fait avancer la locomotive
  void DoFleche(int *Tableau,int *Touche); // Recherche la position de la futur intersection
  bool TestDir(int FuturDir,int *Tableau); // Test si une direction est possible
  void AddLoco(void); // Ajoute une loco au azard

  /*** Fonctions priv�es ***/
  inline bool Go(int FuturDirection); // Fait avancer le tableau (retourne true si tourne)
  inline void FindPoint(float Dist,int &x,int &y); // Retourne la position d'un point sur le parcour

  /*** Variables ***/
  long Reduit,Alonge,Vitesse; // Memorise l'horloge de fin si doit r�duire ou alonger le train
  int PLoco;  // Position de la t�te de la loco dans le tableau
  int PInter; // Position de la futur intersection pour afficher la fleche
  int PEntree; // Entr�e le la loco sur une case pour la fleche
  int Mort;   // M�morise l'heure + duree pour faire une pause apr�s avoir touch� un wagon
  bool Gagne; // Si a fini le niveau
  float D;    // Distance parcourue par la loco
  struct s_TLoco T[256]; // M�morise le parcour de la loco maxi = 256 cases
  int NWagon; // M�morise le nombre de wagon
  e_Sprite Wagon[256]; // M�morise les wagons
  struct s_PosWagon PosWagon[256]; // M�morise position des wagons � l'�cran pour test de colision
  float MemoDuree; // Memorise la precedente dur�e pour faire avancer les explosions du depart
};

#endif
