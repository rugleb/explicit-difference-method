#pragma once

#include <vector>
#include <stdio.h>
#include <math.h>

#define START_TEMPERATURE		0.0
#define LEFT_SIDE_TEMPERATURE	200
#define DOWN_SIDE_TEMPERATURE	100

#define TOP_SIDE_LENGTH			5.0
#define LEFT_SIDE_LENGTH		6.0
#define DOWN_SIDE_LENGTH		8.0
#define RIGHT_SIDE_LENGTH		3.0

#define TIME_ERROR				5

class Grid
{
private:
	double dx;
	double dy;
	double dt;
	double time;
	std::vector< std::vector<double> > temperatures;
	std::vector< std::vector<double> > derivatives;

public:
	Grid();
	Grid(double, double, double);
	~Grid();

	/* setters */
	void setDirichletConditions();
	void setNeumannConditions();
	void setDifferenceAnalogue(Grid *);
	void setTemperature(int, int, double);
	void setDerivatives();

	/* getters */
	double getTime();
	double getTemperature(int, int);
	double getMaxTemperature();
	double getDeltaT();
	std::vector< std::vector<double> > getTemperatures();
	void setTime(double);

	/* debug & print */
	void print();
	void printTemperature(int, int);
	int  checkOnExist(int, int);
};
