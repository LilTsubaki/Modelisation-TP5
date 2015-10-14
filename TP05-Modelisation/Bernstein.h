#pragma once
#include <vector>

class Bernstein
{
	int i;
	int n;
	double factoriel;

public:
	Bernstein(void);
	Bernstein(int a, int b);
	int factorielN(int nb);
	double getInfluence(double t);
	~Bernstein(void);
	double getFactoriel() const;
    void setFactoriel(const double &value);
};

