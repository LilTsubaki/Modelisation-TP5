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

	double xMax = std::numeric_limits<double>::lowest();
	double yMax = std::numeric_limits<double>::lowest();
	double zMax = std::numeric_limits<double>::lowest();

	double xMin = std::numeric_limits<double>::max();
	double yMin = std::numeric_limits<double>::max();
	double zMin = std::numeric_limits<double>::max();


	Maillage();
	Maillage(std::vector<Vector3D> geo, std::vector<int> top, std::vector<Vector3D> norm) : geom(geo), topo(top), normales(norm) {}
	Maillage(std::vector<Vector3D> geo, std::vector<int> top) : geom(geo), topo(top) {}
	Maillage(std::vector<Vector3D> geo, std::vector<int> top, Vector3D grav, double sc, double xMi, double xMa, double yMi, double yMa, double zMi, double zMa)
		: geom(geo), topo(top), centreGravite(grav), scale(sc), xMin(xMi), xMax(xMa), yMin(yMi), yMax(yMa), zMin(zMi), zMax(zMa) {}
	void calculNormalTriangle();


	// Utilitaires
	static Maillage lectureOff(const std::string nom, double taille);
	void Ecriture(std::string nom);
	void replaceTopo(int ancienIndice, int nouvelIndice);
	void supprDoublon();
	void supprDoublon(int indice);
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

