#include "Grid.h"

Grid::Grid()
{
	/* default time */
	time = 0.0;
}

Grid::Grid(double time_, double dx_, double dy_)
{
	time = time_;
	dx = dx_;
	dy = dy_;

	double	tg = (TOP_SIDE_LENGTH - DOWN_SIDE_LENGTH) / (LEFT_SIDE_LENGTH - RIGHT_SIDE_LENGTH),
			b  = LEFT_SIDE_LENGTH - tg * TOP_SIDE_LENGTH,
			x = 0.0, y;

	int 	i, j;

	for (i = 0; x < TOP_SIDE_LENGTH; i++)
	{
		std::vector<double> tmp;
		x = i * dx;
		y = 0.0;
		for (j = 0; y < LEFT_SIDE_LENGTH; j++)
		{
			y = j * dy;
			tmp.push_back(START_TEMPERATURE);
		}
		temperatures.push_back(tmp);
		derivatives.push_back(tmp);
	}

	for (; x < DOWN_SIDE_LENGTH; i++)
	{
		std::vector<double> tmp;
		x = i * dx;
		y = 0.0;
		for (j = 0; y < tg * x + b; j++)
		{
			y = j * dy;
			tmp.push_back(START_TEMPERATURE);
		}
		temperatures.push_back(tmp);
		derivatives.push_back(tmp);
	}
}

Grid::~Grid()
{
	dx = dy = dt = (double)NULL;

	for (int i = 0; i < temperatures.size(); i++)
	{
		temperatures[i].clear();
	}
	temperatures.clear();

	for (int i = 0; i < derivatives.size(); i++)
	{
		derivatives[i].clear();
	}
	derivatives.clear();
}

void Grid::setDirichletConditions()
{
	for (int i = 0; i < temperatures.size(); i++)
	{
		temperatures[i][0] = DOWN_SIDE_TEMPERATURE;
	}

	for (int j = 0; j < temperatures[0].size(); j++)
	{
		temperatures[0][j] = LEFT_SIDE_TEMPERATURE;
	}
}

void Grid::setDerivatives()
{
	double T, Tmax = 0.0;

	for (int i = 1; i < temperatures.size() - 1; i++)
	{
		std::vector<double> tmp;
		for (int j = 1; j < temperatures[i].size() - 1; j++)
		{
			T  = (temperatures[i + 1][j] - 2 * temperatures[i][j] + temperatures[i - 1][j]) / (dx * dx);
			T += (temperatures[i][j + 1] - 2 * temperatures[i][j] + temperatures[i][j - 1]) / (dy * dy);
			derivatives[i][j] = T;

			if (Tmax < T)
			{
				Tmax = T;
			}
		}
	}

	dt = fabs(TIME_ERROR / Tmax);
}

void Grid::setNeumannConditions()
{
	int i, j;

	for (i = 0, j = temperatures[0].size() - 1; i < TOP_SIDE_LENGTH / dx; i++)
	{
		temperatures[i][j] = temperatures[i][j - 1] / (dx + 1);
	}

	for (; i < temperatures.size(); i++, j--)
	{
		temperatures[i][j] = temperatures[i - 1][j - 1] / (sqrt(dx * dx + dy * dy) + 1);
	}

	for (i -= 1; j >= 0; j--)
	{
		temperatures[i][j] = temperatures[i - 1][j] / (dy + 1);
	}
}

void Grid::setDifferenceAnalogue(Grid * prev)
{	
	for (int i = 1; i < prev->temperatures.size() - 1; i++)
	{
		for (int j = 1; j < prev->temperatures[i].size() - 1; j++)
		{
			temperatures[i][j] = prev->derivatives[i][j] * prev->dt + prev->temperatures[i][j];
		}
	}
}

void Grid::setTime(double t)
{
	time = t;
}

void Grid::setTemperature(int i, int j, double T)
{
	temperatures[i][j] = T;
}

std::vector< std::vector<double> > Grid::getTemperatures()
{
	return temperatures;
}

double Grid::getTime()
{
	return time;
}

double Grid::getDeltaT()
{
	return dt;
}

double Grid::getMaxTemperature()
{
	double tMax = 0.0;

	for (int i = 0; i < temperatures.size() - 1; i++)
	{
		for (int j = 0; j < temperatures[i].size() - 1; j++)
		{
			if (tMax <= temperatures[i][j])
			{
				tMax = temperatures[i][j];
			}
		}
	}

	return tMax;
}

int Grid::checkOnExist(int i, int j)
{
	if (i > temperatures.size())
		return 0;

	for (int k = 0; k < temperatures.size(); k++)
	{
		if (j > temperatures[k].size())
			return 0;
	}

	return 1;
}

double Grid::getTemperature(int i, int j)
{
	return temperatures[i][j];
}

void Grid::printTemperature(int i, int j)
{
	if (checkOnExist(i, j))
	{
		printf("Temperature: #%3ld by (%d, %d), value: %4.2f\n", i * temperatures[i].size() + j, i, j, temperatures[i][j]);
	}
	else
	{
		printf("Temperature not exist.\n");
	}
}

void Grid::print()
{
	printf("Grid on level: %.2f\n", time);
	for (int i = 0; i < temperatures.size(); i++)
	{
		int countPointsY = temperatures[i].size();
		for (int j = 0; j < countPointsY; j++)
		{
			printf("Temperature: #%3d by (%d, %d), value: %4.2f\n", i * countPointsY + j, i, j, temperatures[i][j]);
		}
	}
}
