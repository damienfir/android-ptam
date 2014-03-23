#include "batiment.h"
#include "OpenGL.h"
#include <cvd/convolution.h>
#include "/usr/include/SOIL.h"
#include <iostream>


using namespace CVD;
using namespace std;

Batiment::Batiment() //Constructeur des bâtiments
{
  dimensions = 0.185; // Dimensions générales des constructions
  mbInitialised = false; //Le cube n'a pas encore été initialisé
  angle = 0; // Initialise l'angle.
  precangle = 1;
}

void Batiment::DrawStuff(Vector<3> v3CameraPos) //Préparation au dessin des bâtiments
{
  if(!mbInitialised)
    Init(); //Si le cube n'a pas encore été initalisé, on le fait.

  mnFrameCounter ++; // On passe à l'image suivante

  glDisable(GL_BLEND); //If enabled,blend the computed fragment color values with the values in the color buffers
  glEnable(GL_CULL_FACE); //If enabled, cull polygons based on their winding in window coordinates
  glEnable(GL_DEPTH_TEST); //Pour utiliser le Z-buffer

  glMatrixMode(GL_MODELVIEW); //Matrice qui gère la position des objets

      glLoadIdentity(); //Matrce identité
      glMultMatrix(ase3WorldFromBuilding[0]);
      glScaled(dimensions, dimensions, dimensions); //On redimensionne l'objet en fonctions des dimensions des constructions
      glCallList(batimentDisplayList); //Exécute une liste d'affichage
      glDisable(GL_DEPTH_TEST); //Désactive le Z-buffer
      glDisable(GL_CULL_FACE);

/*

    //Essai d'une animation : trop lent !

	GLuint spirale = SOIL_load_OGL_texture //Chargement de la texture facade de référence
	(
		"spirale.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    glPushMatrix();
    int pas=0.5;
    positionVoiture +=pas;
    if (positionVoiture > -12) pas=-0.5;
    if (positionVoiture < -24) pas=+0.5;
    glTranslated(positionVoiture, -6, 0);
    glColor3ub(255,255,255);
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, spirale);
    glBegin(GL_QUADS); //Face quadrangle
    glTexCoord2d(0,1); glVertex3d(-0.5,0.5,0);
    glTexCoord2d(0,0); glVertex3d(-0.5,-0.5,0);
    glTexCoord2d(1,0); glVertex3d(0.5,-0.5,0);
    glTexCoord2d(1,1); glVertex3d(0.5,0.5,0);
    glEnd();
    glPopMatrix();

    */

};


void Batiment::Reset() //Mise à jour de l'affichage à chaque nouvelle image
{
  ase3WorldFromBuilding[0] = SE3<>();
  ase3WorldFromBuilding[0].get_translation()[0] = -dimensions;
  ase3WorldFromBuilding[0].get_translation()[1] = -dimensions;
  ase3WorldFromBuilding[0].get_translation()[2] = dimensions;
  
  mnFrameCounter = 0;

};

void Batiment::DrawBatiment() //Contstruction des faces des bâtiments
{
 GLuint tex_2d = SOIL_load_OGL_texture //Chargement de la texture du bassin des carènes
	(
		"carene.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

 GLuint herbe = SOIL_load_OGL_texture //Chargement de la texture d'herbe
	(
		"herbe.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

  GLuint newBuilding = SOIL_load_OGL_texture //Chargement de la texture des fenêtres du nouveau bâtiment
	(
		"newBuilding.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

  GLuint cabanon = SOIL_load_OGL_texture //Chargement de la texture du cabanon
	(
		"cabanon.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

  GLuint rez2rouge = SOIL_load_OGL_texture //Chargement de la texture  facade rouge de la résidence
	(
		"rez2.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

  GLuint rez2blanc = SOIL_load_OGL_texture //Chargement de la texture facade blanche de la résidence
	(
		"rez22.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	GLuint batimentRef = SOIL_load_OGL_texture //Chargement de la texture facade de référence
	(
		"reference.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);


glEnable(GL_DEPTH_TEST); //Active le Z-buffer

glTranslated(0,0,-2);//Pour compenser les erreurs systématiques.
glRotated(angle,0,0,1); //Pour changer l'endroit par rapport à la maquette où la calibration se fera.


    //Créations des faces :

    // Face du batiment de référence pour le calibrage
    glColor3ub(255,255,255); //La couleur derrière la texture est blanche pour ne pas interférer
	glEnable(GL_TEXTURE_2D); //Active l'utilisation de textures
    glBindTexture(GL_TEXTURE_2D, batimentRef); //On attache la texture aux éléments qui suivent
    glBegin(GL_QUADS); //Face quadrangle
    glTexCoord2d(0,1); glVertex3d(-2.5,2.5,1.5); //glTexCoord2d désigne les coordonnées de la texture à attacher au point construit ensuite. glVertex3d est un point dans l'expace (x,y,z)
    glTexCoord2d(0,0); glVertex3d(-2.5,-2.5,1.5);
    glTexCoord2d(1,0); glVertex3d(2.5,-2.5,1.5);
    glTexCoord2d(1,1); glVertex3d(2.5,2.5,1.5);
    glEnd();//Fin de construction des quadrangles (Pour changer de texture, on est obligé de s'arrêter puis de recommencer)

    //La rez
    glBindTexture(GL_TEXTURE_2D, rez2rouge);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// Permet de répéter des images sur une surface, pour en faire un motif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //1
    glBegin(GL_QUADS);
    glTexCoord2d(0,2); glVertex3d(-1.2+6.5,8.2+15,1.5);
    glTexCoord2d(0,0); glVertex3d(-1.2+6.5,8.2+15,0);
    glTexCoord2d(2,0); glVertex3d(-0.2+6.5,7.4+15,0);
    glTexCoord2d(2,2); glVertex3d(-0.2+6.5,7.4+15,1.5);

    //2
    glTexCoord2d(0,2); glVertex3d(-0.6+6.5,9.4+15,1.5);
    glTexCoord2d(0,0); glVertex3d(-0.6+6.5,9.4+15,0);
    glTexCoord2d(1,0); glVertex3d(-1.2+6.5,8.2+15,0);
    glTexCoord2d(1,2); glVertex3d(-1.2+6.5,8.2+15,1.5);
    glEnd();

    //3
    glDisable(GL_TEXTURE_2D); // On désactive la texture pour cette face.
    glColor3ub(225,225,225); //La couleur de la face suivante sera blanche (mais pas trop, pour éviter d'avoir du blanc non naturel)
    glBegin(GL_QUADS);
    glVertex3d(-1+6.5,9.6+15,1.3);
    glVertex3d(-1+6.5,9.6+15,0);
    glVertex3d(-0.6+6.5,9.4+15,0);
    glVertex3d(-0.6+6.5,9.4+15,1.3);
    glEnd();

    //4
    glEnable(GL_TEXTURE_2D); //On charge une autre texture pour les faces suivantes
    glColor3ub(255,255,255); //La texture doit s'appliquer sur un blanc pur pour éviter de la modifier avec une couleur.
    glBindTexture(GL_TEXTURE_2D, rez2blanc);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);
    glTexCoord2d(0,2); glVertex3d(0+6.5,10.8+15,1.3);
    glTexCoord2d(0,0); glVertex3d(0+6.5,10.8+15,0);
    glTexCoord2d(2,0); glVertex3d(-1+6.5,9.6+15,0);
    glTexCoord2d(2,2); glVertex3d(-1+6.5,9.6+15,1.3);

    //5
    glTexCoord2d(0,2); glVertex3d(1+6.5,10+15,1.3);
    glTexCoord2d(0,0); glVertex3d(1+6.5,10+15,0);
    glTexCoord2d(3,0); glVertex3d(0+6.5,10.8+15,0);
    glTexCoord2d(3,2); glVertex3d(0+6.5,10.8+15,1.3);

    //6
    glTexCoord2d(0,2); glVertex3d(4.6+6.5,7.335+15,1.6);
    glTexCoord2d(0,0); glVertex3d(4.6+6.5,7.335+15,0);
    glTexCoord2d(5,0); glVertex3d(1+6.5,10+15,0);
    glTexCoord2d(5,2); glVertex3d(1+6.5,10+15,1.6);

    //7
    glTexCoord2d(0,2); glVertex3d(5.9+6.5,6.3+15,1.3);
    glTexCoord2d(0,0); glVertex3d(5.9+6.5,6.3+15,0);
    glTexCoord2d(2,0); glVertex3d(4.6+6.5,7.335+15,0);
    glTexCoord2d(2,2); glVertex3d(4.6+6.5,7.335+15,1.3);
    glEnd();

    //8
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glVertex3d(5.1+6.5,5.5+15,1.3);
    glVertex3d(5.1+6.5,5.5+15,0);
    glVertex3d(5.9+6.5,6.3+15,0);
    glVertex3d(5.9+6.5,6.3+15,1.3);

    //9
    glVertex3d(4.7+6.5,5.9+15,1.3);
    glVertex3d(4.7+6.5,5.9+15,0);
    glVertex3d(5.1+6.5,5.5+15,0);
    glVertex3d(5.1+6.5,5.5+15,1.3);
    glEnd();

    //10
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rez2rouge);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0,2); glVertex3d(4.6+6.5,5.8+15,1.4);
    glTexCoord2d(0.0,0.0); glVertex3d(4.6+6.5,5.8+15,0);
    glTexCoord2d(0.5,0); glVertex3d(4.7+6.5,5.9+15,0);
    glTexCoord2d(0.5,2); glVertex3d(4.7+6.5,5.9+15,1.4);

    //11
    glTexCoord2d(0,2); glVertex3d(3.6+6.5,5.92+15,1.4);
    glTexCoord2d(0,0); glVertex3d(3.6+6.5,5.92+15,0);
    glTexCoord2d(1,0); glVertex3d(4.6+6.5,5.8+15,0);
    glTexCoord2d(1,2); glVertex3d(4.6+6.5,5.8+15,1.4);
    glEnd();

    //12
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2d(0,0.5); glVertex3d(3.3+6.5,5.5+15,1.6);
    glTexCoord2d(0,0); glVertex3d(3.3+6.5,5.5+15,0);
    glTexCoord2d(1,0); glVertex3d(3.6+6.5,5.92+15,0);
    glTexCoord2d(1,0.5); glVertex3d(3.6+6.5,5.92+15,1.6);
    glEnd();

    //13
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rez2blanc);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);
    glTexCoord2d(0,3); glVertex3d(0.1+6.5,8+15,1.6);
    glTexCoord2d(0,0); glVertex3d(0.1+6.5,8+15,0);
    glTexCoord2d(5,0); glVertex3d(3.3+6.5,5.5+15,0);
    glTexCoord2d(5,3); glVertex3d(3.3+6.5,5.5+15,1.6);
    glEnd();

    //14
    glBindTexture(GL_TEXTURE_2D, rez2rouge);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);
    glTexCoord2d(0,2); glVertex3d(-0.2+6.5,7.4+15,1.5);
    glTexCoord2d(0,0); glVertex3d(-0.2+6.5,7.4+15,0);
    glTexCoord2d(0.5,0); glVertex3d(0.1+6.5,8+15,0);
    glTexCoord2d(0.5,2); glVertex3d(0.1+6.5,8+15,1.5);
    glEnd();

    //15
    glDisable(GL_TEXTURE_2D);
    glColor3ub(243,176,160);
    glBegin(GL_QUADS);
    glVertex3d(0.05+6.5,8.07+15,1.6);
    glVertex3d(0.05+6.5,8.07+15,1.5);
    glVertex3d(0.1+6.5,8+15,1.5);
    glVertex3d(0.1+6.5,8+15,1.6);

    //16
    glVertex3d(0.77+6.5,9.54+15,1.6);
    glVertex3d(0.77+6.5,9.54+15,1.5);
    glVertex3d(0.05+6.5,8.07+15,1.5);
    glVertex3d(0.05+6.5,8.07+15,1.6);

    //17
    glVertex3d(1+6.5,9.4+15,1.5);
    glVertex3d(1+6.5,9.4+15,1.3);
    glVertex3d(-0.2+6.5,10.2+15,1.3);
    glVertex3d(-0.2+6.5,10.2+15,1.5);

    //18
    glVertex3d(1+6.5,10+15,1.6);
    glVertex3d(1+6.5,10+15,1.3);
    glVertex3d(0.77+6.5,9.54+15,1.3);
    glVertex3d(0.77+6.5,9.54+15,1.6);
    glEnd();

    //19
    glEnable(GL_TEXTURE_2D);
    glColor3ub(255,255,255);
    glBindTexture(GL_TEXTURE_2D, rez2rouge);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);
    glTexCoord2d(0,0.5); glVertex3d(-0.2+6.5,10.2+15,1.5);
    glTexCoord2d(0,0); glVertex3d(-0.2+6.5,10.2+15,1.3);
    glTexCoord2d(1,0); glVertex3d(-0.6+6.5,9.4+15,1.3);
    glTexCoord2d(1,0.5); glVertex3d(-0.6+6.5,9.4+15,1.5);
    glEnd();

    //20
    glDisable(GL_TEXTURE_2D);
    glColor3ub(243,176,160);
    glBegin(GL_QUADS);
    glVertex3d(3.6+6.5,5.92+15,1.6);
    glVertex3d(3.6+6.5,5.92+15,1.4);
    glVertex3d(4.5+6.5,7.19+15,1.4);
    glVertex3d(4.5+6.5,7.19+15,1.6);

    //21
    glVertex3d(5.3+6.5,6.5+15,1.4);
    glVertex3d(5.3+6.5,6.5+15,1.3);
    glVertex3d(4.5+6.5,7.19+15,1.3);
    glVertex3d(4.5+6.5,7.19+15,1.4);
    glEnd();

    //22
    glColor3ub(225,225,225);
    glBindTexture(GL_TEXTURE_2D, rez2rouge);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);
    glTexCoord2d(0,0.5); glVertex3d(4.7+6.5,5.9+15,1.4);
    glTexCoord2d(0,0); glVertex3d(4.7+6.5,5.9+15,1.3);
    glTexCoord2d(1,0); glVertex3d(5.3+6.5,6.5+15,1.3);
    glTexCoord2d(1,0.5); glVertex3d(5.3+6.5,6.5+15,1.4);
    glEnd();

    //23
    glDisable(GL_TEXTURE_2D);
    glColor3ub(243,176,160); //Rouge de la résidence
    glBegin(GL_QUADS);
    glVertex3d(4.5+6.5,7.19+15,1.6);
    glVertex3d(4.5+6.5,7.19+15,1.3);
    glVertex3d(4.6+6.5,7.335+15,1.3);
    glVertex3d(4.6+6.5,7.335+15,1.6);

    //24
    glColor3ub(50,50,50); //Noir mais pas trop
    glVertex3d(-0.2+6.5,10.2+15,1.5);
    glVertex3d(-1.2+6.5,8.2+15,1.5);
    glVertex3d(-0.2+6.5,7.4+15,1.5);
    glVertex3d(0.85+6.5,9.49+15,1.5);

    //25
    glVertex3d(0+6.5,10.8+15,1.3);
    glVertex3d(-1+6.5,9.6+15,1.3);
    glVertex3d(0.33+6.5,8.65+15,1.3);
    glVertex3d(1+6.5,10+15,1.3);

    //26
    glColor3ub(100,100,100); //Gris du toit de la résidence
    glVertex3d(1+6.5,10+15,1.6);
    glVertex3d(0.05+6.5,8.07+15,1.6);
    glVertex3d(3.3+6.5,5.5+15,1.6);
    glVertex3d(4.6+6.5,7.335+15,1.6);

    //27
    glColor3ub(50,50,50);
    glVertex3d(4.6+6.5,5.8+15,1.4);
    glVertex3d(5.3+6.5,6.5+15,1.4);
    glVertex3d(4.5+6.5,7.19+15,1.4);
    glVertex3d(3.6+6.5,5.92+15,1.4);

    //28
    glVertex3d(4.6+6.5,7.335+15,1.3);
    glVertex3d(4.05+6.5,6.559+15,1.3);
    glVertex3d(5.1+6.5,5.5+15,1.3);
    glVertex3d(5.9+6.5,6.3+15,1.3);
    glEnd();


    //Nouveau batiment
    glEnable(GL_TEXTURE_2D);
    glColor3ub(255,255,255);
    glBindTexture(GL_TEXTURE_2D, newBuilding);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //1
    glBegin(GL_QUADS);
    glTexCoord2d(0,0.33); glVertex3d(12.5,2.5,1.5);
    glTexCoord2d(0,0); glVertex3d(12.5,2.5,0);
    glTexCoord2d(1,0); glVertex3d(12.5,-2.5,0);
    glTexCoord2d(1,0.33); glVertex3d(12.5,-2.5,1.5);

    //2
    glTexCoord2d(0,0.33); glVertex3d(12.5,-2.5,1.5);
    glTexCoord2d(0,0); glVertex3d(12.5,-2.5,0);
    glTexCoord2d(1,0); glVertex3d(17.5,-2.5,0);
    glTexCoord2d(1,0.33); glVertex3d(17.5,-2.5,1.5);

    //3
    glTexCoord2d(0,0.33); glVertex3d(17.5,-2.5,1.5);
    glTexCoord2d(0,0); glVertex3d(17.5,-2.5,0);
    glTexCoord2d(1,0); glVertex3d(17.5,2.5,0);
    glTexCoord2d(1,0.33); glVertex3d(17.5,2.5,1.5);

    //4
    glTexCoord2d(0,0.33); glVertex3d(17.5,2.5,1.5);
    glTexCoord2d(0,0); glVertex3d(17.5,2.5,0);
    glTexCoord2d(1,0); glVertex3d(12.5,2.5,0);
    glTexCoord2d(1,0.33); glVertex3d(12.5,2.5,1.5);

    //5
    glTexCoord2d(0,0.32);glVertex3d(15.8,0.8,1.5);
    glTexCoord2d(0,0); glVertex3d(15.8,0.8,0);
    glTexCoord2d(0.33,0); glVertex3d(15.8,-0.8,0);
    glTexCoord2d(0.33,0.32); glVertex3d(15.8,-0.8,1.5);

    //6
    glTexCoord2d(0,0.32); glVertex3d(15.8,-0.8,1.5);
    glTexCoord2d(0,0); glVertex3d(15.8,-0.8,0);
    glTexCoord2d(0.33,0); glVertex3d(13.3,-0.8,0);
    glTexCoord2d(0.33,0.32); glVertex3d(13.3,-0.8,1.5);

    //7
    glTexCoord2d(0,0.32); glVertex3d(13.3,-0.8,1.5);
    glTexCoord2d(0,0); glVertex3d(13.3,-0.8,0);
    glTexCoord2d(0.33,0); glVertex3d(13.3,0.8,0);
    glTexCoord2d(0.33,0.32); glVertex3d(13.3,0.8,1.5);

    //8
    glTexCoord2d(0,0.32); glVertex3d(13.3,0.8,1.5);
    glTexCoord2d(0,0); glVertex3d(13.3,0.8,0);
    glTexCoord2d(0.33,0); glVertex3d(15.8,0.8,0);
    glTexCoord2d(0.33,0.32); glVertex3d(15.8,0.8,1.5);
    glEnd();

    //9
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, herbe);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_QUADS);
    glTexCoord2d(0,2.94); glVertex3d(12.5,2.5,1.5);
    glTexCoord2d(0,0); glVertex3d(12.5,-2.5,1.5);
    glTexCoord2d(1,0); glVertex3d(14.2,-2.5,1.5);
    glTexCoord2d(1,2.94); glVertex3d(14.2,2.5,1.5);

    //10
    glTexCoord2d(0,2.94);glVertex3d(15.8,2.5,1.5);
    glTexCoord2d(0,0); glVertex3d(15.8,-2.5,1.5);
    glTexCoord2d(1,0); glVertex3d(17.5,-2.5,1.5);
    glTexCoord2d(1,2.94); glVertex3d(17.5,2.5,1.5);

    //11
    glTexCoord2d(0,1); glVertex3d(14.2,2.5,1.5);
    glTexCoord2d(0,0); glVertex3d(14.2,0.8,1.5);
    glTexCoord2d(2,0); glVertex3d(15.8,0.8,1.5);
    glTexCoord2d(2,1); glVertex3d(15.8,2.5,1.5);

    //12
    glTexCoord2d(0,1); glVertex3d(14.2,-0.8,1.5);
    glTexCoord2d(0,0); glVertex3d(14.2,-2.5,1.5);
    glTexCoord2d(2,0); glVertex3d(15.8,-2.5,1.5);
    glTexCoord2d(2,1); glVertex3d(15.8,-0.8,1.5);
    glEnd();

    //Carenes

    //1
    glBindTexture(GL_TEXTURE_2D, tex_2d);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_QUADS);
    glTexCoord2d(0,2); glVertex3d(-14,-5,0.5);
    glTexCoord2d(0,0); glVertex3d(-14,-5,0);
    glTexCoord2d(3,0); glVertex3d(-12.5,-5,0);
    glTexCoord2d(3,2); glVertex3d(-12.5,-5,0.5);
    glEnd();

    //2
    glDisable(GL_TEXTURE_2D);
    glColor3ub(225,225,225);
    glBegin(GL_QUADS);
    glVertex3d(-12.5,-5,0.5);
    glVertex3d(-12.5,-5,0);
    glVertex3d(-12.5,1,0);
    glVertex3d(-12.5,1,0.5);
    glEnd();

    //3
    glEnable(GL_TEXTURE_2D);
    glColor3ub(255,255,255);
    glBindTexture(GL_TEXTURE_2D, tex_2d);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_QUADS);
    glTexCoord2d(0,2); glVertex3d(-14,13.5,0.5);
    glTexCoord2d(0,0); glVertex3d(-14,13.5,0);
    glTexCoord2d(21,0); glVertex3d(-14,-5,0);
    glTexCoord2d(21,2); glVertex3d(-14,-5,0.5);
    glEnd();

    //4
    glDisable(GL_TEXTURE_2D);
    glColor3ub(137,137,147);
    glBegin(GL_QUADS);
    glVertex3d(-14,-5,0.5);
    glVertex3d(-12.5,-5,0.5);
    glVertex3d(-12.5,1,0.5);
    glVertex3d(-14,1,0.5);
    glEnd();

    //5
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    GLfloat bleu[] = {0.0f,0.0f,0.8f,0.5f}; //Pour avoir une surface bleu transparente
    glColor4fv(bleu);
    glBegin(GL_QUADS);
    glVertex3d(-14.5,13.5,0.5);
    glVertex3d(-14.5,-5,0.5);
    glVertex3d(-14,-5.0,0.5);
    glVertex3d(-14,13.5,0.5);
    glEnd();
    glDisable(GL_BLEND);

    //6
    glEnable(GL_TEXTURE_2D);
    glColor3ub(255,255,255);
    glBindTexture(GL_TEXTURE_2D, tex_2d);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);
    glTexCoord2d(0,6); glVertex3d(-12.5,13.5,1.5);
    glTexCoord2d(0,0); glVertex3d(-12.5,13.5,0);
    glTexCoord2d(8,0); glVertex3d(-12,9.5,0);
    glTexCoord2d(8,6); glVertex3d(-12,9.5,1.5);

    //7
    glTexCoord2d(0,2); glVertex3d(-12.5,13.5,0.5);
    glTexCoord2d(0,0); glVertex3d(-12.5,13.5,0);
    glTexCoord2d(3,0); glVertex3d(-14,13.5,0);
    glTexCoord2d(3,2); glVertex3d(-14,13.5,0.5);
    glEnd();

    //8
    glDisable(GL_TEXTURE_2D);
    glColor3ub(137,137,147);
    glBegin(GL_QUADS);
    glVertex3d(-14,13.5,0.5);
    glVertex3d(-14,9.5,0.5);
    glVertex3d(-12,9.5,0.5);
    glVertex3d(-12,13.5,0.5);

    //9
    glTexCoord2d(0,0.5);
    glVertex3d(-12.5,13.5,1.5);
    glVertex3d(-12,9.5,1.5);
    glVertex3d(-4,9.5,1.5);
    glVertex3d(-4,13.5,1.5);

    //10
    glColor3ub(225,225,225);
    glVertex3d(-12,9.5,1.5);
    glVertex3d(-12,9.5,0);
    glVertex3d(-4,9.5,0);
    glVertex3d(-4,9.5,1.5);
    glEnd();

    //11
    glEnable(GL_TEXTURE_2D);
    glColor3ub(255,255,255);
    glBindTexture(GL_TEXTURE_2D, tex_2d);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);
    glTexCoord2d(0,6); glVertex3d(-4,9.5,1.5);
    glTexCoord2d(0,0); glVertex3d(-4,9.5,0);
    glTexCoord2d(8,0); glVertex3d(-4,13.5,0);
    glTexCoord2d(8,6); glVertex3d(-4,13.5,1.5);

    //12
    glTexCoord2d(0,6); glVertex3d(-4,13.5,1.5);
    glTexCoord2d(0,0); glVertex3d(-4,13.5,0);
    glTexCoord2d(17,0); glVertex3d(-12.5,13.5,0);
    glTexCoord2d(17,6); glVertex3d(-12.5,13.5,1.5);

    //13
    glTexCoord2d(0,4); glVertex3d(-12,10.5,1);
    glTexCoord2d(0,0); glVertex3d(-12,10.5,0);
    glTexCoord2d(2,0); glVertex3d(-13.3,10.5,0);
    glTexCoord2d(2,4); glVertex3d(-13.3,10.5,1);

    //14
    glTexCoord2d(0,4); glVertex3d(-13.3,10.5,1);
    glTexCoord2d(0,0); glVertex3d(-13.3,10.5,0);
    glTexCoord2d(2,0); glVertex3d(-13.3,9.5,0);
    glTexCoord2d(2,4); glVertex3d(-13.3,9.5,1);
    glEnd();

    //15
    glDisable(GL_TEXTURE_2D);
    glColor3ub(137,137,147);
    glBegin(GL_QUADS);
    glVertex3d(-13.3,10.5,1);
    glVertex3d(-13.3,9.5,1);
    glVertex3d(-12,9.5,1);
    glVertex3d(-12,10.5,1);
    glEnd();

    //Cabanon
    glEnable(GL_TEXTURE_2D);
    glColor3ub(255,255,255);
    glBindTexture(GL_TEXTURE_2D, cabanon);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_QUADS);
    glTexCoord2d(0,1); glVertex3d(-16.1,-4.8,0.25);
    glTexCoord2d(0,0); glVertex3d(-16.1,-4.8,0);
    glTexCoord2d(1,0); glVertex3d(-15.5,-4.8,0);
    glTexCoord2d(1,1); glVertex3d(-15.5,-4.8,0.25);

    glTexCoord2d(0,1); glVertex3d(-15.5,-4.8,0.25);
    glTexCoord2d(0,0); glVertex3d(-15.5,-4.8,0);
    glTexCoord2d(1,0); glVertex3d(-15.5,-4.5,0);
    glTexCoord2d(1,1); glVertex3d(-15.5,-4.5,0.25);

    glTexCoord2d(0,1); glVertex3d(-15.5,-4.5,0.25);
    glTexCoord2d(0,0); glVertex3d(-15.5,-4.5,0);
    glTexCoord2d(1,0); glVertex3d(-16.1,-4.5,0);
    glTexCoord2d(1,1); glVertex3d(-16.1,-4.5,0.25);

    glTexCoord2d(0,1); glVertex3d(-16.1,-4.5,0.25);
    glTexCoord2d(0,0); glVertex3d(-16.1,-4.5,0);
    glTexCoord2d(1,0); glVertex3d(-16.1,-4.8,0);
    glTexCoord2d(1,1); glVertex3d(-16.1,-4.8,0.25);
    glEnd();

    //toit
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glVertex3d(-16.1,-4.5,0.25);
    glVertex3d(-16.1,-5,0.25);
    glVertex3d(-15.5,-5,0.25);
    glVertex3d(-15.5,-4.5,0.25);

    //Local à poubelles de la résidence
    glColor3ub(225,225,225);
    glVertex3d(7,19.8,0.25);
    glVertex3d(7,19.8,0.1);
    glVertex3d(7.5,19.8,0);
    glVertex3d(7.5,19.8,0.25);

    glVertex3d(7.5,19.8,0.25);
    glVertex3d(7.5,19.8,0);
    glVertex3d(7.5,20.6,0);
    glVertex3d(7.5,20.6,0.25);

    glVertex3d(7.5,20.6,0.25);
    glVertex3d(7.5,20.6,0);
    glVertex3d(7,20.6,0);
    glVertex3d(7,20.6,0.25);

    glVertex3d(7,20.6,0.25);
    glVertex3d(7,20.6,0.1);
    glVertex3d(7,19.8,0.1);
    glVertex3d(7,19.8,0.1);

    glColor3ub(50,50,50);
    glVertex3d(7,20.6,0.25);
    glVertex3d(7,19.8,0.25);
    glVertex3d(7.5,19.8,0.25);
    glVertex3d(7.5, 20.6,0.25);
    glEnd();


}

void Batiment::Init() //Initialisation de l'objet
{
  if(precangle == angle){
	return;
  }
  precangle = angle;
  batimentDisplayList = glGenLists(1); //Liste d'affichage des structures
  glNewList(batimentDisplayList,GL_COMPILE);
  DrawBatiment(); //Dessin des structures
  glEndList();
};

//Fonctions de zoom des structures.

void Batiment::ZoomUp()
{
    dimensions +=0.005;
    cout << "dimensions : "<< dimensions << endl; //On affiche les dimensions courantes.
};

void Batiment::ZoomDown()
{
    dimensions -=0.005;
    cout << "dimensions : " << dimensions << endl;
};

void Batiment::Rotate()
{
    angle +=90;
    cout << "angle : " << angle << endl;
    Init();
};

