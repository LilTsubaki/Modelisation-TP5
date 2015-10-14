#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "vector3d.h"
#include <string>

class Maillage
{
	Vector3D test;
	std::vector<Vector3D> geom; //contient les points du maillage
    std::vector<int> topo; //contient des triplets d'indice pour chaque triangle du maillage
    std::vector<Vector3D> normales; //contient les normales au point
	Vector3D centreGravite;
	double scale;

    public:
        Maillage();
        Maillage(std::vector<Vector3D> geo, std::vector<int> top, std::vector<Vector3D> norm) : geom(geo), topo(top), normales(norm) {}
        Maillage(std::vector<Vector3D> geo, std::vector<int> top) : geom(geo), topo(top) {}
		Maillage(std::vector<Vector3D> geo, std::vector<int> top, Vector3D grav, double sc) : geom(geo), topo(top), centreGravite(grav), scale(sc) {}
		void calculNormalTriangle();


        // Utilitaires
        static Maillage lectureOff(const std::string nom, double taille);
        void Ecriture(std::string nom);
        ~Maillage();

		std::vector<Vector3D> getGeom() const;
        void setGeom(const std::vector<Vector3D> &value);
        std::vector<int> getTopo() const;
        void setTopo(const std::vector<int> &value);
        std::vector<Vector3D> getNormales() const;
        void setNormales(const std::vector<Vector3D> &value);
		Vector3D getCentreGravite() const;
        void setCentreGravite(const Vector3D &value);
		double getScale() const;
        void setScale(const double &value);
	
};

