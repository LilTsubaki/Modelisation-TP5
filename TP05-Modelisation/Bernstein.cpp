#include "stdafx.h"
#include "Bernstein.h"
#include <iostream>


Bernstein::Bernstein(void)
{
}



Bernstein::~Bernstein(void)
{
}

Bernstein::Bernstein(int a, int b)
{
	i =a;
	n =b;
	factoriel = factorielN(n)/(factorielN(i)*factorielN(n-i));
}

int Bernstein::factorielN(int nb)
{
	int tot = 1;
	for(nb; nb > 0; nb--)
	{
		tot *= nb;
	}
	return tot;
}

double Bernstein::getInfluence(double t)
{
	//std::cout << factoriel*std::pow(t,i)*std::pow((1-t), n-i) << std::endl;
	return factoriel*std::pow(t,i)*std::pow((1-t), n-i);
}


double Bernstein::getFactoriel() const
{
    return factoriel;
}

void Bernstein::setFactoriel(const double &value)
{
    factoriel = value;
}

