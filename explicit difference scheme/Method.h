#pragma once

#include "Grid.h"
#include <vector>
#include <cstdlib>
#include <cstring>
#include <fstream>

#define STEP_DEFAULT		0.25
#define STEP_TIME_DEFAULT	STEP_DEFAULT * STEP_DEFAULT / 4

#define START_TIME			0.0
#define PAUSE_TIME			0.001
#define PAUSE_WAIT			10
#define FIND_TIME_DEFAULT	15

#define FILE_NAME			"plot"
#define FILE_NAME_RUN		"run"

class Method
{
private:
	double dx;
	double dy;
	double findTime;
	std::vector<Grid*> grids;

public:
	Method();
	Method(double, double, double);
	~Method();

	/* inits */
	void initStartGrid();
	void initGrids();
	void createRunFile(double);

	/* shows */
	void writeToFile();
	void showDynamicsPlot();

	/* getters */
	std::vector<Grid*> getGrids();
};
