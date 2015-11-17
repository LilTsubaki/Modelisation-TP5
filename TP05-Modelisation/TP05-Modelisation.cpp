/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 * hello.c
 * This is a simple, introductory OpenGL program.
 */
#include "stdafx.h"
#include <cstdlib>
#include "glut.h"
#include <math.h> 
#include <iostream>
#include "Vector3d.h"
#include "Bernstein.h"
#include <math.h>
#define M_PI 3.14159265358979323846

#define ESC 27



static int pointSelect = -1;
static std::vector<Vector3D> pointsCaracs;
static std::vector<Vector3D> pointsCaracs2;
static Vector3D temp;
static bool b = false;

Vector3D casteljau(std::vector<Vector3D> points, double u)
{
	Vector3D pointRetour;
	if(points.size() ==2)
	{
		pointRetour = points.at(0)+((points.at(1) - points.at(0)) * u);
		return pointRetour;
	}
	else
	{
		std::vector<Vector3D> newPoints;
		for(int i = 0; i < points.size()-1; i++)
		{
			newPoints.push_back(points.at(i)+((points.at(i+1) - points.at(i)) * u));
		}
		pointRetour = casteljau(newPoints, u); 
	}
	return pointRetour;
}


GLvoid hermite(Vector3D p0, Vector3D p1, Vector3D v0, Vector3D v1)
{
	double u = 0;
	double f1;
	double f2;
	double f3;
	double f4;
	double x;
	double y;
	double z;

	glBegin(GL_LINE_STRIP);
		glColor3f (1.0, 0.0, 1.0);
	for(int i = 0; i <= 10; i++)
	{
		f1 = 2*u*u*u-3*u*u+1;
		f2 = -2*u*u*u+3*u*u;
		f3 = u*u*u-2*u*u+u;
		f4 = u*u*u-u*u;

		x = f1*p0.x()+f2*p1.x()+f3*v0.x()+f4*v1.x();
		y = f1*p0.y()+f2*p1.y()+f3*v0.y()+f4*v1.y();
		z = f1*p0.z()+f2*p1.z()+f3*v0.z()+f4*v1.z();

		
		glVertex3f (x, y, z);

		u+=0.1;
	}
	glEnd();
}

std::vector<Vector3D> bernstein(std::vector<Vector3D> pointCarac, int n)
{
	std::vector<Bernstein> lesBernstein;
	std::vector<Vector3D> lesPoints;
	double u = 0;
	double x;
	double y;
	double z;

	for(int i = 0; i <= n; i++)
	{
		std::vector<double> deg;
		Bernstein bTemp(i, n-1);
		lesBernstein.push_back(bTemp); 
	}

	glBegin(GL_LINE_STRIP);
		glColor3f (1.0, 0.0, 1.0);
	for(int i = 0; i <= 10; i++)
	{
		Vector3D point;
		for(int j = 0; j < n; j++)
		{
			point += pointCarac.at(j)*lesBernstein.at(j).getInfluence(u);
		}
		//std::cout << "--------------------"<< std::endl;
		glVertex3f (point.x(), point.y(), point.z());
		lesPoints.push_back(point);
		u+=0.1;
	}
	glEnd();
	return lesPoints;
}

void surfaceBernstein(std::vector<Vector3D> pointCarac, std::vector<Vector3D> pointCarac2, int n)
{
	std::vector<Vector3D> b1 = bernstein(pointsCaracs, n);
	std::vector<Vector3D> b2 = bernstein(pointsCaracs2, n);

	for (int i = 0; i < b1.size(); i++)
	{
		glBegin(GL_LINE_STRIP);
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(b1[i].x(), b1[i].y(), b1[i].z());
		glVertex3f(b2[i].x(), b2[i].y(), b2[i].z());
		glEnd();
	}
}

void cercleAutourBernstein(std::vector<Vector3D> pointCarac, int n, double nbPointsCercle, double rayonCercle)
{
	double epsilon = 0.0001;
	double u = 0;
	std::vector<Bernstein> lesBernstein;
	for (int i = 0; i <= n; i++)
	{
		std::vector<double> deg;
		Bernstein bTemp(i, n - 1);
		lesBernstein.push_back(bTemp);
	}


	std::vector<Vector3D> b1 = bernstein(pointsCaracs, n);
	std::vector<Vector3D> orthos1;
	std::vector<Vector3D> orthos2;
	//on parcours tout les points calculer par l'algo de bernstein
	for (int i = 0; i < b1.size(); i++)
	{
		//on cherche la tangeant au point i
		Vector3D pointPrecedent;
		Vector3D pointSuivant;
		//on calcul deux points proches du notre 
		for (int j = 0; j < n; j++)
		{
			pointPrecedent += pointCarac.at(j)*lesBernstein.at(j).getInfluence(u- epsilon);
			pointSuivant += pointCarac.at(j)*lesBernstein.at(j).getInfluence(u+ epsilon);
		}
		//on calcul la tangeante a partir de nos deux points
		Vector3D tangeante = (pointSuivant - pointPrecedent) / (2 * epsilon);
		tangeante.normalize();

		//on prend un premier vecteur "random" afin de trouver un premier vecteur orthogonal à la tangeante
		Vector3D random(0.2, 0.1, 0.3);
		Vector3D ortho1 = tangeante^random;
		Vector3D ortho2 = ortho1^tangeante;

		ortho1.normalize();
		ortho2.normalize();

		orthos1.push_back(ortho1);
		orthos2.push_back(ortho2);

		glBegin(GL_LINE_STRIP);
		glColor3f(1.0, 1.0, 0.0);
		for (double deg = 0; deg <= 2 * M_PI; deg += (2 * M_PI) / nbPointsCercle)
		{
			Vector3D pointCercle = b1[i] + (rayonCercle*cos(deg)*ortho1 + rayonCercle*sin(deg)*ortho2);
			glVertex3f(pointCercle.x(), pointCercle.y(), pointCercle.z());
		}
		glEnd();
		u += 0.1;
	}

	//calcul des points du cercle
	for (double deg = 0; deg <= 2 * M_PI; deg += (2 * M_PI) / nbPointsCercle)
	{
		glBegin(GL_LINE_STRIP);
		glColor3f(0.8, 0.8, 0.8);
		for (int j = 0; j < b1.size(); j++)
		{
			Vector3D pointCercle = b1[j] + (rayonCercle*cos(deg)*orthos1[j] + rayonCercle*sin(deg)*orthos2[j]);
			glVertex3f(pointCercle.x(), pointCercle.y(), pointCercle.z());
		}
		glEnd();
	}

}

Vector3D surfaceCasteljau(std::vector<std::vector<Vector3D>> lesListsCaracs, float u1, float u2)
{
	std::vector<Vector3D> points;
	for (int i = 0; i < lesListsCaracs.size(); i++)
	{
		points.push_back(casteljau(lesListsCaracs[i], u1));
	}
	return casteljau(points, u2);
}

void display(void)
{
/* clear all pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

/* draw white polygon (rectangle) with corners at
 * (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0) 
 */
  
   

   /*Vector3D p0(0, 0, 0);
   Vector3D p1(1, 0, 0);
   Vector3D v0(1, 1, 0);
   Vector3D v1(1, -1, 0);
   
  
  hermite(p0,p1,v0,v1);*/
   

   /************points caracs**************/
	glBegin(GL_LINE_STRIP);
		glColor3f (1.0, 1.0, 1.0);
	for(int i = 0; i < pointsCaracs.size(); i++)
	{
		//std::cout << pointsCaracs.at(i).x() << " | "  << pointsCaracs.at(i).y() << " | " << pointsCaracs.at(i).z() << std::endl;
		glVertex3f (pointsCaracs.at(i).x(), pointsCaracs.at(i).y(), pointsCaracs.at(i).z());
	}
	glEnd();//*/
	
	
	
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 1.0, 0.0);
	for (int i = 0; i < pointsCaracs2.size(); i++)
	{
		//std::cout << pointsCaracs.at(i).x() << " | "  << pointsCaracs.at(i).y() << " | " << pointsCaracs.at(i).z() << std::endl;
		glVertex3f(pointsCaracs2.at(i).x(), pointsCaracs2.at(i).y(), pointsCaracs2.at(i).z());
	}
	glEnd();//*/
	/****************Décomenté pour double bernstein lisse***************************/
	/*bernstein(pointsCaracs, 4);
   bernstein(pointsCaracs2, 4);//*/
 
   /************casteljau**************/
   /*double u = 0;
   glBegin(GL_LINE_STRIP);
   glColor3f (1.0, 0.0, 1.0);
   for(int i = 0; i <= 10; i++)
   {
   Vector3D point = casteljau(pointsCaracs, u);
   //std::cout << point.x() << " | " << point.y() << " | " << point.z() << std::endl;
   glVertex3f (point.x(), point.y(),point.z());
   u+=0.1;
   }
   glEnd();//*/
	

	/************TP-06*************/
	//surfaceBernstein(pointsCaracs, pointsCaracs2, 4);
	//cercleAutourBernstein(pointsCaracs, 4, 10, 0.05);


	/************surface casteljau**************/
   std::vector<std::vector<Vector3D>> lesListsCaracs;
   lesListsCaracs.push_back(pointsCaracs);
   lesListsCaracs.push_back(pointsCaracs2);
   double u1 = 0;
   double u2 = 0;
   
   for (int i = 0; i <= 10; i++)
   {
	   glBegin(GL_LINE_STRIP);
	   glColor3f(1.0, 0.0, 1.0);
	   for (int j = 0; j <= 10; j++)
	   {
		   Vector3D point = surfaceCasteljau(lesListsCaracs, i/10.0, j/10.0);
		   //std::cout << point.x() << " | " << point.y() << " | " << point.z() << std::endl;
		   glVertex3f(point.x(), point.y(), point.z());
	   }
	   glEnd();
   }

   for (int i = 0; i <= 10; i++)
   {
	   glBegin(GL_LINE_STRIP);
	   glColor3f(1.0, 0.0, 1.0);
	   for (int j = 0; j <= 10; j++)
	   {
		   Vector3D point = surfaceCasteljau(lesListsCaracs, j/10.0, i/10.0);
		   //std::cout << point.x() << " | " << point.y() << " | " << point.z() << std::endl;
		   glVertex3f(point.x(), point.y(), point.z());
	   }
	   glEnd();
   }//*/
   

	/************carré autour des points caracs**************/
    glBegin(GL_LINE_LOOP);
		glColor3f (0.0, 1.0, 1.0);
			glVertex3f(temp.x()-0.01, temp.y()-0.01, 0.0);
			glVertex3f(temp.x()-0.01, temp.y()+0.01, 0.0);
			glVertex3f(temp.x()+0.01, temp.y()+0.01, 0.0);
			glVertex3f(temp.x()+0.01, temp.y()-0.01, 0.0);
		glEnd();


/* Swap the buffers to show the one
 * on which we writed
 */
   glutSwapBuffers();
}




void init (void)
{
/* select clearing color     */
   glClearColor (0.0, 0.0, 0.0, 0.0);

/* initialize viewing values  */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
	float tx=0.0;
	float ty=0.0;

   switch (key) {
  
   case '1' : 
	   pointSelect = 0;
	   break;
	case '2' : 
	   pointSelect = 1;
	   break;
	case '3' : 
	   pointSelect = 2;
	   break;
	case '4' : 
	   pointSelect = 3;
	   break;
	case '5':
		pointSelect = 4;
		break;
	case '6':
		pointSelect = 5;
		break;
	case '7':
		pointSelect = 6;
		break;
		/***************décommenter pour surface**************/
	case '8':
		pointSelect = 7;
		break;//*/

   case 'd':
	   //std::cout << '\a';
         tx=0.1;
		 ty=0;
      break;
   case 'q':
	   //std::cout << '\a';
         tx=-0.1;
		 ty=0;
      break;
   case 'z':
	   //std::cout << '\a';
         ty=0.1;
		 tx=0;
      break;
   case 's':
	   //std::cout << '\a';
         ty=-0.1;
		 tx=0;
      break;
   case ESC:
      exit(0);
      break;
   default :
	   tx=0;
	   ty=0;
   }

   if(pointSelect >= 0 && pointSelect <= 3)
   {
		temp = Vector3D(Vector2D(tx + pointsCaracs.at(pointSelect).x(), ty + pointsCaracs.at(pointSelect).y()));
	   if(ty != 0 || tx != 0)
	   {
		   pointsCaracs.erase(pointsCaracs.begin() + pointSelect);
		   pointsCaracs.insert(pointsCaracs.begin() + pointSelect, temp);
	   }
   }
   /**********************Décommenté pour les surfaces béziers et casteljau*************************/
   else
   {

	   if (pointSelect >= 4 && pointSelect <= 7)
	   {
		   pointSelect -= 4;
		   temp = Vector3D(Vector2D(tx + pointsCaracs2.at(pointSelect).x(), ty + pointsCaracs2.at(pointSelect).y()));
		   if (ty != 0 || tx != 0)
		   {
			   pointsCaracs2.erase(pointsCaracs2.begin() + pointSelect);
			   pointsCaracs2.insert(pointsCaracs2.begin() + pointSelect, temp);
		   }
		   pointSelect += 4;
	   }
   }//*/
   

   /****************Décomenté pour double bernstein lisse***************************/
   
   /*if (pointSelect <= 3 && pointSelect >= 0)
   {
	   temp = Vector3D(Vector2D(tx + pointsCaracs.at(pointSelect).x(), ty + pointsCaracs.at(pointSelect).y()));
	   b = false;
   }
   else
   {  
	   if (pointSelect > 3 && pointsCaracs2.size() > 0)
	   {
		   pointSelect -= 3;
		   temp = Vector3D(Vector2D(tx + pointsCaracs2.at(pointSelect).x(), ty + pointsCaracs2.at(pointSelect).y()));
		   pointSelect += 3;
		   b = true;
	   }
   }

   if(pointSelect != -1 && (ty !=0 || tx !=0))
   {	   
	   if (!b)
	   {
			pointsCaracs.erase(pointsCaracs.begin() + pointSelect);
			pointsCaracs.insert(pointsCaracs.begin() +pointSelect, temp);

			if (pointSelect == 2)
			{
				Vector3D pointSuivant = pointsCaracs2.at(0) + (pointsCaracs2.at(0) - pointsCaracs.at(pointSelect));
				pointsCaracs2.erase(pointsCaracs2.begin() + 1);
				pointsCaracs2.insert(pointsCaracs2.begin() + 1, pointSuivant);
			}

		   if (pointSelect == 3)
		   {
			   pointsCaracs2.erase(pointsCaracs2.begin());
			   pointsCaracs2.insert(pointsCaracs2.begin(), temp);

			   Vector3D pointSuivant = pointsCaracs2.at(0) + (pointsCaracs2.at(0) - pointsCaracs.at(2));
			   pointsCaracs2.erase(pointsCaracs2.begin() + 1);
			   pointsCaracs2.insert(pointsCaracs2.begin() + 1, pointSuivant);
		   }
			
	   }
	   else
	   {
		   pointSelect -= 3;
		   pointsCaracs2.erase(pointsCaracs2.begin() + pointSelect);
		   pointsCaracs2.insert(pointsCaracs2.begin() + pointSelect, temp);
		   if (pointSelect == 1)
		   {
			   Vector3D pointPrecedent = pointsCaracs2.at(0)+(pointsCaracs2.at(0)- pointsCaracs2.at(1));
			   pointsCaracs.erase(pointsCaracs.begin() + pointSelect+1);
			   pointsCaracs.insert(pointsCaracs.begin() + pointSelect+1, pointPrecedent);
		   }
		   
		   
		   pointSelect += 3;
	   }
   }//*/

   glutPostRedisplay();
}

/*
 * Declare initial window size, position, and display mode
 * (double buffer and RGB).  Open window with "hello"
 * in its title bar.  Call initialization routines.
 * Register callback function to display graphics.
 * Enter main loop and process events.
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   
   
    Vector3D p0(0.05,0.1,0);
	Vector3D p1(0.167,0.5,0);
	Vector3D v0(0.33,0.7,0.5);
	Vector3D v1(0.5,0.1,0.5);

	pointsCaracs.push_back(p0);
	pointsCaracs.push_back(p1);
	pointsCaracs.push_back(v0);
	pointsCaracs.push_back(v1);//*/


	/****************Décomenté pour double bernstein lisse***************************/
	/*Vector3D p5(0.667, 0.5, 0);
	Vector3D p6(0.83, 0.5, 0);
	Vector3D p7(0.95, 0.1, 0);

	pointsCaracs2.push_back(v1);
	pointsCaracs2.push_back(v1+(v1-v0));
	pointsCaracs2.push_back(p6);
	pointsCaracs2.push_back(p7);//*/


	/****************Décomenté pour surface***************************/
	Vector3D p5(0.05, 0.3, 0.5);
	Vector3D p6(0.167, 0.7, 0.5);
	Vector3D p7(0.33, 0.9, 0.5);
	Vector3D p8(0.5, 0.3, 0.5);

	pointsCaracs2.push_back(p5);
	pointsCaracs2.push_back(p6);
	pointsCaracs2.push_back(p7);
	pointsCaracs2.push_back(p8);//*/
	
   glutCreateWindow ("hello");
   init ();
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}
