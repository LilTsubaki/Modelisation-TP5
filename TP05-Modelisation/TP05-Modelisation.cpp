#include "stdafx.h"
#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
#include "glut.h" 
#include "Maillage.h"

#define WIDTH  480
#define HEIGHT 480
 
#define RED   0
#define GREEN 0
#define BLUE  0
#define ALPHA 1
#define PI 3.14159
 
#define KEY_ESC 27
 
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y); 
GLvoid window_idle(); 
 
// Paramètres de la source de lumière 0 (directionnelle)
GLfloat Light0Position[]= { 4.0f, 4.0f, 2.0f, 0.0f };
GLfloat Light0Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat Light0Diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat Light0Specular[]= { 0.3f, 0.3f, 0.3f, 1.0f };
 
// Paramètres de la source de lumière 1 (ponctuelle)
GLfloat Light1Position[]= { 4.0f, 8.0f, 2.0f, 1.0f };
GLfloat Light1Ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat Light1Diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat Light1Specular[]= { 0.3f, 0.3f, 0.3f, 1.0f };

static double zRotate = 0.5;
static double facteur = 0.5;
static Maillage maille;

int main(int argc, char **argv) 
{  
  // initialisation  des paramètres de GLUT en fonction
  // des arguments sur la ligne de commande

	

  glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 
	 maille = Maillage::lectureOff("C:/Users/etu/Desktop/bunny.off", 10);
	 maille.calculNormalTriangle();
    //bunny.Ecriture("bunny.obj");


  // définition et création de la fenêtre graphique
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Primitives graphiques");
 
  // initialisation de OpenGL et de la scène
  initGL();  
  init_scene();
 
  // choix des procédures de callback pour 
  // le tracé graphique
  glutDisplayFunc(&window_display);
  // le redimensionnement de la fenêtre
  glutReshapeFunc(&window_reshape);
  // la gestion des événements clavier
  glutKeyboardFunc(&window_key);
  glutIdleFunc(&window_idle);
  // la boucle prinicipale de gestion des événements utilisateur
  glutMainLoop();  
 
  return 1;
}
 
GLvoid afficheMaille()
{
	Vector3D a;
	Vector3D b;
	Vector3D c;
	double facteur = 100.0;
	int cptNormale = 0;

	std::vector<Vector3D> geometrie = maille.getGeom();
	std::vector<Vector3D> normales = maille.getNormales();
    std::vector<int> topologie = maille.getTopo();
	Vector3D centre = maille.getCentreGravite();
	double scale = maille.getScale();

	//std::cout << maille.getTopo().size() << std::endl;
	for(int i =0; i < topologie.size(); i+=3)
	{
		a = (geometrie.at(topologie.at(i))-centre)/scale;
		b = (geometrie.at(topologie.at(i+1))-centre)/scale;
		c = (geometrie.at(topologie.at(i+2))-centre)/scale;
		//std::cout << "a : " << a.x() << " | " <<  a.y()<< " | " <<  a.z() << std::endl;
		//std::cout << "b : " << b.x() << " | " <<  b.y()<< " | " <<  b.z() << std::endl;
		//std::cout << "c : " << c.x() << " | " <<  c.y()<< " | " <<  c.z() << std::endl;

		glNormal3d(normales.at(cptNormale).x(), normales.at(cptNormale).y(), normales.at(cptNormale).z());
		//std::cout << normales.at(cptNormale).x() << " | " <<  normales.at(cptNormale).y()<< " | " <<  normales.at(cptNormale).z() << std::endl;
		glBegin(GL_TRIANGLES);
			glVertex3f (a.x(), a.y(), a.z());
			glVertex3f (b.x(), b.y(), b.z());
			glVertex3f (c.x(), c.y(), c.z());
	   glEnd();

	   cptNormale++;
	}

	//std::cout << "endload" << std::endl;
}


// initialisation du fond de la fenêtre graphique : noir opaque
 
GLvoid initGL() 
{
	glClearColor(0.6f, 0.6f, 0.7f, 1.0f);               // Couleur servant à effacer la fenêtre (celle du brouillard)
    glShadeModel(GL_SMOOTH);                            // Modèle d'ombrage : lissage de Gouraud
    //glEnable(GL_CULL_FACE);                             // Ne traitre pas les faces cachées
    glEnable(GL_DEPTH_TEST);                            // Active le Z-Buffer
    glDepthFunc(GL_LEQUAL);                             // Fonctionnement du Z-Buffer
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Active la correction de perspective (pour ombrage, texture, ...)
 
    glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Ambient);    // Couleur ambiante de la source de lumière 0 (directionnelle)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Diffuse);    // Couleur diffuse de la source de lumière 0
    glLightfv(GL_LIGHT0, GL_SPECULAR,Light0Specular);   // Couleur spéculaire de la source de lumière 0
    glLightfv(GL_LIGHT0, GL_POSITION,Light0Position);   // Direction de la source de lumière 0
    glEnable(GL_LIGHT0);                                // Activation de la source de lumière 0
 
    glLightfv(GL_LIGHT1, GL_AMBIENT, Light1Ambient);    // Couleur ambiante de la source de lumière 1
    glLightfv(GL_LIGHT1, GL_DIFFUSE, Light1Diffuse);    // Couleur diffuse de la source de lumière 1
    glLightfv(GL_LIGHT1, GL_SPECULAR,Light1Specular);   // Couleur spéculaire de la source de lumière 1
    glLightfv(GL_LIGHT1, GL_POSITION,Light1Position);   // Position de la source de lumière 1
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.02);   // Atténuation de la source de lumière 1
    //glEnable(GL_LIGHT1);                              // Activation de la source de lumière 1
 
    glEnable(GL_LIGHTING);                              // Activation de l'éclairage général

        glShadeModel(GL_SMOOTH);
    glClearColor(RED, GREEN, BLUE, ALPHA);
        glEnable(GL_DEPTH_TEST);
}
 
void init_scene()
{
}
 



// fonction de call-back pour l´affichage dans la fenêtre
GLvoid window_display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
  render_scene();
  glRotated(1,0,1,0);
  	 afficheMaille();
  // trace la scène grapnique qui vient juste d'être définie
  glFlush();
  glutSwapBuffers();
  
}
 
// fonction de call-back pour le redimensionnement de la fenêtre
 
GLvoid window_reshape(GLsizei width, GLsizei height)
{  
  glViewport(0, 0, width, height);
 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-15.0, 15.0, -15.0, 15.0, -15.0, 15.0);
 
  // toutes les transformations suivantes s´appliquent au modèle de vue 
  glMatrixMode(GL_MODELVIEW);
}
 
// fonction de call-back pour la gestion des événements clavier
 
GLvoid window_key(unsigned char key, int x, int y) 
{  
  switch (key) {    
  case KEY_ESC:  
    exit(1);                    
    break;
     

  default:
    printf ("La touche %d n´est pas active.\n", key);
    break;
  }     
}
 
GLvoid window_idle() 
{  
	//zRotate+=facteur;
	glutPostRedisplay();
}
 
 
void render_scene()
{
//c'est ici qu'on dessine
    //glLoadIdentity();
 
 
 }