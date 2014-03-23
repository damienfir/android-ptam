#ifndef BATIMENT_H_INCLUDED
#define BATIMENT_H_INCLUDED

#include <TooN/TooN.h>

using namespace TooN;
#include "OpenGL.h"

class Batiment //Classe pour dessiner les batiments en réalité augmentée
{
 public:
  Batiment(); //Constructeur
  void DrawStuff(Vector<3> v3CameraPos); // Construction de l'affichage général (lumières...)
  void Reset(); //Mettre à jour l'objet
  void Init(); //Initialiser l'objet
  void ZoomUp(); //Pour agrandir les batiments
  void ZoomDown(); //Pour rétrécir les bâtiments
  void Rotate();

 protected:
  bool mbInitialised; //Savoir si Init() a déjà été appelée
  void DrawBatiment(); //Dessin du bâtiment

  GLuint batimentDisplayList;
  double dimensions ; //rapport dimensions virtuelles/dimensions réelles
  int precangle;
  int angle;
  SE3<> ase3WorldFromBuilding[4];
  int mnFrameCounter; //Compteur de frame
};


#endif // BATIMENT_H_INCLUDED
