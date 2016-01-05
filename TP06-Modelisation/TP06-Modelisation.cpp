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
static Vector3D centreG;

static std::vector<std::pair<Vector3D, float>> lesSpheres;

bool opUnion(std::vector<std::pair<Vector3D, float>> lesSpheres, Vector3D centreCube)
{
	for (int i = 0; i < lesSpheres.size(); i++)
	{
		if (centreCube.distanceToPoint(lesSpheres.at(i).first) < lesSpheres.at(i).second)
			return true;
	}
	return false;
}

bool opInter(std::vector<std::pair<Vector3D, float>> lesSpheres, Vector3D centreCube)
{
	for (int i = 0; i < lesSpheres.size(); i++)
	{
		if (centreCube.distanceToPoint(lesSpheres.at(i).first) > lesSpheres.at(i).second)
			return false;
	}
	return true;
}

bool opDiff(std::vector<std::pair<Vector3D, float>> lesSpheres, Vector3D centreCube)
{
	bool b = centreCube.distanceToPoint(lesSpheres.at(0).first) < lesSpheres.at(0).second;
	if (!b)
		return b;
	else
	{
		for (int i = 1; i < lesSpheres.size(); i++)
		{
			if (centreCube.distanceToPoint(lesSpheres.at(i).first) < lesSpheres.at(i).second)
				b = false;
		}
		return b;
	}
}

void cube(Vector3D centreSphere, Vector3D centre, float rayonInitial, float rayon)
{
	//devant
	Vector3D devant(centre.x(), centre.y(), centre.z() - 2 * rayon);
	//if(devant.distanceToPoint(centreSphere) > rayonInitial)
	//if(!opUnion(lesSpheres, devant))
	//if (!opInter(lesSpheres, devant))
	if (!opDiff(lesSpheres, devant))
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() - rayon);
		glVertex3f(centre.x() - rayon, centre.y() + rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() + rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() - rayon);
		glEnd();
	}

	//derriere
	Vector3D derriere(centre.x(), centre.y(), centre.z() + 2 * rayon);
	//if (derriere.distanceToPoint(centreSphere) > rayonInitial)
	//if (!opUnion(lesSpheres, derriere))
	//if (!opInter(lesSpheres, derriere))
	if (!opDiff(lesSpheres, derriere))
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() + rayon);
		glVertex3f(centre.x() - rayon, centre.y() + rayon, centre.z() + rayon);
		glVertex3f(centre.x() + rayon, centre.y() + rayon, centre.z() + rayon);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() + rayon);
		glEnd();
	}
	
	//gauche
	Vector3D gauche(centre.x() - 2 * rayon, centre.y(), centre.z());
	//if (gauche.distanceToPoint(centreSphere) > rayonInitial)
	//if (!opUnion(lesSpheres, gauche))
	//if (!opInter(lesSpheres, gauche))
	if (!opDiff(lesSpheres, gauche))
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() - rayon);
		glVertex3f(centre.x() - rayon, centre.y() + rayon, centre.z() - rayon);
		glVertex3f(centre.x() - rayon, centre.y() + rayon, centre.z() + rayon);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() + rayon);
		glEnd();
	}
	
	//droite
	Vector3D droite(centre.x() + 2 * rayon, centre.y(), centre.z());
	//if (droite.distanceToPoint(centreSphere) > rayonInitial)
	//if (!opUnion(lesSpheres, droite))
	//if (!opInter(lesSpheres, droite))
	if (!opDiff(lesSpheres, droite))
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() + rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() + rayon, centre.z() + rayon);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() + rayon);
		glEnd();
	}
	
	//dessous
	Vector3D dessous(centre.x(), centre.y() - 2 * rayon, centre.z());
	//if (dessous.distanceToPoint(centreSphere) > rayonInitial)
	//if (!opUnion(lesSpheres, dessous))
	//if (!opInter(lesSpheres, dessous))
	if (!opDiff(lesSpheres, dessous))
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() - rayon);
		glVertex3f(centre.x() + rayon, centre.y() - rayon, centre.z() + rayon);
		glVertex3f(centre.x() - rayon, centre.y() - rayon, centre.z() + rayon);
		glEnd();
	}
	
	//dessus
	Vector3D dessus(centre.x(), centre.y() + 2 * rayon, centre.z());
	//if (dessus.distanceToPoint(centreSphere) > rayonInitial)
	//if (!opUnion(lesSpheres, dessus))
	//if (!opInter(lesSpheres, dessus))
	if (!opDiff(lesSpheres, dessus))
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 1.0, 1.0);
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
		//if (centre.distanceToPoint(centreSphere) <= rayonInitial)
		//if (opUnion(lesSpheres, centre))
		//if (opInter(lesSpheres, centre))
		if (opDiff(lesSpheres, centre))
		{
			cube(centreSphere, centre, rayonInitial, rayon);
		}	
	}	
}

int main(int argc, char **argv)
{
	// initialisation  des paramètres de GLUT en fonction
	// des arguments sur la ligne de commande
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

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

// initialisation du fond de la fenêtre graphique : noir opaque

GLvoid initGL()
{
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
	glRotated(1,0, 1,0);
	render_scene();

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
		printf("La touche %d n´est pas active.\n", key);
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

		//subdivision(centreTemp, centreTemp, rayonTemp, rayonTemp, 5);
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

	centreG = centreGlobal;
	subdivision(centreGlobal, centreGlobal, max/2, max/2, 5);

	cube(centreGlobal, centreGlobal, max/2, max/2);
}

void render_scene()
{	
	lesSpheres.clear();
	std::pair<Vector3D, float> sphere1(Vector3D(0.0, 0.0, 0.0), 6);
	std::pair<Vector3D, float> sphere2(Vector3D(-5.0, -5.0, 0.0), 6);

	lesSpheres.push_back(sphere1);
	lesSpheres.push_back(sphere2);

	multiDiv(lesSpheres);

	//Vector3D centreSphere(0.0, 0.0, 0.0);
	//subdivision(centreSphere, centreSphere,6, 6, 5);
}