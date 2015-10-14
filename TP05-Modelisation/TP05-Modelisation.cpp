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

GLvoid bernstein(std::vector<Vector3D> pointCarac, int n)
{
	std::vector<Bernstein> lesBernstein;
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
		u+=0.1;
	}
	glEnd();
}

void display(void)
{
/* clear all pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

/* draw white polygon (rectangle) with corners at
 * (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0) 
 */
  
   

	glBegin(GL_LINE_STRIP);
		glColor3f (1.0, 1.0, 1.0);
	for(int i = 0; i < pointsCaracs.size(); i++)
	{
		//std::cout << pointsCaracs.at(i).x() << " | "  << pointsCaracs.at(i).y() << " | " << pointsCaracs.at(i).z() << std::endl;
		glVertex3f (pointsCaracs.at(i).x(), pointsCaracs.at(i).y(), pointsCaracs.at(i).z());
	}
	glEnd();



  //hermite(p0,p1,v0,v1);

   bernstein(pointsCaracs, 4);


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

   case 'd':
         tx=0.1;
		 ty=0;
      break;
   case 'q':
         tx=-0.1;
		 ty=0;
      break;
   case 'z':
         ty=0.1;
		 tx=0;
      break;
   case 's':
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

   if(pointSelect != -1 && (ty !=0 || tx !=0))
   {
	   //std::cout << "iiiiiiiiiiiiiif" << std::endl;
	   
	   //std::cout << pointsCaracs.size() << std::endl;
	   Vector3D temp(Vector2D(tx+pointsCaracs.at(pointSelect).x(), ty+pointsCaracs.at(pointSelect).y()));
	   pointsCaracs.erase(pointsCaracs.begin() + pointSelect);
	   pointsCaracs.insert(pointsCaracs.begin() +pointSelect, temp);
   }

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
   
   
   Vector3D p0(0,0,0);
	Vector3D p1(0.25,0.5,0);
	Vector3D v0(0.75,0.5,0);
	Vector3D v1(1,0,0);

	pointsCaracs.push_back(p0);
	pointsCaracs.push_back(p1);
	pointsCaracs.push_back(v0);
	pointsCaracs.push_back(v1);


   glutCreateWindow ("hello");
   init ();
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}
