#include "Method.h"

int main(int argc, char * argv[])
{
	double  findTime;

	argc == 1 ? findTime = FIND_TIME_DEFAULT : findTime = atof(argv[1]);
	
	Method * method = new Method(STEP_DEFAULT, STEP_DEFAULT, findTime);
	method->initStartGrid();
	method->initGrids();
	method->showDynamicsPlot();

	return 0;
}
