#include "stdafx.h"
#include <cstdlib>
#include "glut.h"
#include <math.h> 
#include <iostream>
#include "Vector3d.h"
#include <math.h>
#include <stdio.h>      
#include <stdlib.h>   
#include <vector>
#include <map>

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

static std::vector<Vector3D> centreCube;

void cube(Vector3D centreSphere, Vector3D centre, float rayonInitial, float rayon)
{
	//devant
	Vector3D devant(centre.x(), centre.y(), centre.z() - 2 * rayon);
	if(devant.distanceToPoint(centreSphere) > rayonInitial)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() - rayon);
		glVertex3f(centre.x() - rayon, centre.y() + rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() + rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() - rayon);
		glEnd();
	}

	//derriere
	Vector3D derriere(centre.x(), centre.y(), centre.z() + 2 * rayon);
	if (derriere.distanceToPoint(centreSphere) > rayonInitial)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() + rayon);
		glVertex3f(centre.x() - rayon, centre.y() + rayon, centre.z() + rayon);
		glVertex3f(centre.x() + rayon, centre.y() + rayon, centre.z() + rayon);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() + rayon);
		glEnd();
	}
	
	//gauche
	Vector3D gauche(centre.x() - 2 * rayon, centre.y(), centre.z());
	if (gauche.distanceToPoint(centreSphere) > rayonInitial)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() - rayon);
		glVertex3f(centre.x() - rayon, centre.y() + rayon, centre.z() - rayon);
		glVertex3f(centre.x() - rayon, centre.y() + rayon, centre.z() + rayon);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() + rayon);
		glEnd();
	}
	
	//droite
	Vector3D droite(centre.x() + 2 * rayon, centre.y(), centre.z());
	if (droite.distanceToPoint(centreSphere) > rayonInitial)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() + rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() + rayon, centre.z() + rayon);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() + rayon);
		glEnd();
	}
	
	//dessous
	Vector3D dessous(centre.x(), centre.y() - 2 * rayon, centre.z());
	if (dessous.distanceToPoint(centreSphere) > rayonInitial)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() + rayon);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() + rayon);
		glEnd();
	}
	
	//dessus
	Vector3D dessus(centre.x(), centre.y() + 2 * rayon, centre.z());
	if (dessus.distanceToPoint(centreSphere) > rayonInitial)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(centre.x() - rayon, centre.y() + rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() + rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() + rayon, centre.z() + rayon);
		glVertex3f(centre.x() - rayon, centre.y() + rayon, centre.z() + rayon);
		glEnd();
	}
}

void subdivision(Vector3D centreSphere, Vector3D centre, float rayonInitial, float rayon, int nbDivision)
{
	std::vector<Vector3D> lesCentres;
	if (nbDivision > 1)
	{
		lesCentres.push_back(Vector3D(centre.x() - rayon / 2, centre.y() - rayon / 2, centre.z() - rayon / 2));
		lesCentres.push_back(Vector3D(centre.x() - rayon / 2, centre.y() + rayon / 2, centre.z() - rayon / 2));
		lesCentres.push_back(Vector3D(centre.x() + rayon / 2, centre.y() - rayon / 2, centre.z() - rayon / 2));
		lesCentres.push_back(Vector3D(centre.x() + rayon / 2, centre.y() + rayon / 2, centre.z() - rayon / 2));

		lesCentres.push_back(Vector3D(centre.x() - rayon / 2, centre.y() - rayon / 2, centre.z() + rayon / 2));
		lesCentres.push_back(Vector3D(centre.x() - rayon / 2, centre.y() + rayon / 2, centre.z() + rayon / 2));
		lesCentres.push_back(Vector3D(centre.x() + rayon / 2, centre.y() - rayon / 2, centre.z() + rayon / 2));
		lesCentres.push_back(Vector3D(centre.x() + rayon / 2, centre.y() + rayon / 2, centre.z() + rayon / 2));

		for (int i = 0; i < lesCentres.size(); i++)
		{
			subdivision(centreSphere, lesCentres.at(i), rayonInitial, rayon / 2, nbDivision-1);
		}
	}
	else
	{
		if (centre.distanceToPoint(centreSphere) <= rayonInitial)
		{
			cube(centreSphere, centre, rayonInitial, rayon);
		}	
	}	
}

int main(int argc, char **argv)
{
	// initialisation  des param�tres de GLUT en fonction
	// des arguments sur la ligne de commande
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// d�finition et cr�ation de la fen�tre graphique
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Primitives graphiques");

	// initialisation de OpenGL et de la sc�ne
	initGL();
	init_scene();

	// choix des proc�dures de callback pour 
	// le trac� graphique
	glutDisplayFunc(&window_display);
	// le redimensionnement de la fen�tre
	glutReshapeFunc(&window_reshape);
	// la gestion des �v�nements clavier
	glutKeyboardFunc(&window_key);
	glutIdleFunc(&window_idle);
	// la boucle prinicipale de gestion des �v�nements utilisateur
	glutMainLoop();

	


	return 1;
}

// initialisation du fond de la fen�tre graphique : noir opaque

GLvoid initGL()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(RED, GREEN, BLUE, ALPHA);
	glEnable(GL_DEPTH_TEST);
}

void init_scene()
{
}

// fonction de call-back pour l�affichage dans la fen�tre

GLvoid window_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRotated(1,0, 1,0);
	render_scene();

	// trace la sc�ne grapnique qui vient juste d'�tre d�finie
	glFlush();
	glutSwapBuffers();
}

// fonction de call-back pour le redimensionnement de la fen�tre

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-15.0, 15.0, -15.0, 15.0, -15.0, 15.0);

	// toutes les transformations suivantes s�appliquent au mod�le de vue 
	glMatrixMode(GL_MODELVIEW);
}

// fonction de call-back pour la gestion des �v�nements clavier

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case KEY_ESC:
		exit(1);
		break;

	default:
		printf("La touche %d n�est pas active.\n", key);
		break;
	}
}

GLvoid window_idle()
{
	glutPostRedisplay();
}

void multiDiv(std::vector<std::pair<Vector3D, float>> lesSpheres)
{
	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::min();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::min();

	for (int i = 0; i < lesSpheres.size(); i++)
	{
		Vector3D centreTemp = lesSpheres.at(i).first;
		float rayonTemp = lesSpheres.at(i).second;

		if (centreTemp.x()-rayonTemp < minX)
			minX = centreTemp.x() - rayonTemp;
		if (centreTemp.x()+rayonTemp > maxX)
			maxX = centreTemp.x() + rayonTemp;

		if (centreTemp.y()-rayonTemp < minY)
			minY = centreTemp.y()- rayonTemp;
		if (centreTemp.y()+rayonTemp > maxY)
			maxY = centreTemp.y()+ rayonTemp;

		if (centreTemp.z()-rayonTemp < minZ)
			minZ = centreTemp.z()- rayonTemp;
		if (centreTemp.z()+rayonTemp > maxZ)
			maxZ = centreTemp.z()+ rayonTemp;

		subdivision(centreTemp, centreTemp, rayonTemp, rayonTemp, 5);
	}

	Vector3D centreGlobal((minX + maxX) / 2, (minY + maxY) / 2, (minY + maxY) / 2);
	float diffX = maxX - minX;
	float diffY = maxY - minY;
	float diffZ = maxZ - minZ;

	float max = diffX;
	if (diffY > max)
		max = diffY;
	if (diffZ > max)
		max = diffZ;

	cube(centreGlobal, centreGlobal, max/2, max/2);
}

void render_scene()
{	
	std::vector<std::pair<Vector3D, float>> lesSpheres;
	std::pair<Vector3D, float> sphere1(Vector3D(0.0, 0.0, 0.0), 6);
	std::pair<Vector3D, float> sphere2(Vector3D(-5.0, -5.0, 0.0), 2);

	lesSpheres.push_back(sphere1);
	lesSpheres.push_back(sphere2);
	
	multiDiv(lesSpheres);

	//Vector3D centreSphere(0.0, 0.0, 0.0);
	//subdivision(centreSphere, centreSphere,6, 6, 5);
}