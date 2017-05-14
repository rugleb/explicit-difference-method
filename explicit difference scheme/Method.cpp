#include "Method.h"

Method::Method()
{
	dx = dy = STEP_DEFAULT;
	findTime = FIND_TIME_DEFAULT;
}

Method::Method(double x, double y, double time)
{
	dx = x;
	dy = y;
	findTime = time;
}

Method::~Method()
{
	dx = dy = findTime = (double)NULL;
	grids.clear();
}

void Method::initStartGrid()
{
	Grid * startGrid = new Grid(START_TIME, dx, dy);

	startGrid->setDirichletConditions();
	startGrid->setDerivatives();
	
	grids.push_back(startGrid);
}

std::vector<Grid*> Method::getGrids()
{
	return grids;
}

void Method::initGrids()
{
	double 	time = START_TIME;
	int 	k;
	
	for (k = 1; time < findTime; k++)
	{
		Grid * newGrid = new Grid(time, dx, dy);

		/* init new grid */
		newGrid->setDirichletConditions();
		newGrid->setDifferenceAnalogue(grids[k-1]);
		newGrid->setNeumannConditions();
		newGrid->setDerivatives();

		grids.push_back(newGrid);
		time += grids[k-1]->getDeltaT();
	}
}

void Method::writeToFile()
{
	std::vector< std::vector<double> > temps;
	std::ofstream fout(FILE_NAME);

	for (int k = 0; k < grids.size(); k++)
	{
		temps = grids[k]->getTemperatures();

		for (int i = 0; i < temps.size(); i++)
		{
			for (int j = 0; j < temps[i].size(); j++)
			{
				fout << dx * i << "\t" << dy * j << "\t" << temps[i][j] << std::endl;
			}
			fout << "\n";
		}

		fout << "\n";
	}

	fout.close();
}

void Method::showDynamicsPlot()
{
	createRunFile(findTime);
	writeToFile();
	system("gnuplot commands; clear");

	char rmRun[3 + strlen(FILE_NAME) + 1] 		= "rm ";
	char rmPlot[3 + strlen(FILE_NAME_RUN) + 1] 	= "rm ";

	strcat(rmRun, FILE_NAME);
	strcat(rmPlot, FILE_NAME_RUN);

	system(rmRun);
	system(rmPlot);
}

void Method::createRunFile(double findTime)
{
	std::ofstream fout(FILE_NAME_RUN);
	
	fout << "do for [i=0:" << grids.size() - 1 << "] { \n";
	fout << "splot 'plot' index i using 1:2:3 with pm3d \n";
	fout << "print 'step #', i \n";
	fout << "pause " << PAUSE_TIME << "\n";
	fout << "}" << "\n";
	fout << "print 'Done. Time to close: \n";
	fout << "do for [i=0:" << PAUSE_WAIT << "] { \n";
	fout << "print " << PAUSE_WAIT << "-i \n";
	fout << "pause 1 \n";
	fout << "} \n";

	fout.close();
}