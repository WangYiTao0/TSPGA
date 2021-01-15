#include <iostream>
#include <stdlib.h> 
#include <queue>
#include <stack>
#include <fstream>
#include <iomanip>	
#include <ctime>
#include <algorithm>

#include "SA.h"

using namespace std;

 void CityDataTranslate() {
	ifstream read_in;
	read_in.open("att48.txt");      
	if (!read_in.is_open())
	{
		cout << "Read file Failed." << endl;
		return;
	}

	ofstream fout("city_48.txt");      

	double city_table[MAX_CITYNUM][MAX_CITYNUM];
	int city_No[MAX_CITYNUM];
	double city_x[MAX_CITYNUM];
	double city_y[MAX_CITYNUM];

	int vex_num;
	read_in >> vex_num;
	fout << vex_num << endl;

	for (int i = 0; i < vex_num; i++)
	{
		read_in >> city_No[i] >> city_x[i] >> city_y[i];

		fout << i + 1 << " ";
	}
	fout << endl;

	for (int i = 0; i < vex_num; i++)
	{
		city_table[i][i] = 0;
		for (int j = 0; j < vex_num; j++)
		{
			double temp = (city_x[i] - city_x[j]) * (city_x[i] - city_x[j]) + (city_y[i] - city_y[j]) * (city_y[i] - city_y[j]);
			city_table[i][j] = sqrt(temp);
			fout << city_table[i][j] << " ";
		}
		fout << endl;
	}
}

int main() {
 
	time_t T_begin = clock();
	Graph G;
	CreateGraph(G);

    
	TSP_solution bestSoluion = SA_TSP(G);
	Display(G, bestSoluion);

	time_t T_end = clock();
	double RunningTime = double(T_end - T_begin) / CLOCKS_PER_SEC;

	cout << "RunningTime = " << RunningTime << endl;

	system("pause");
	return 0;
}

void CreateGraph(Graph& G) {
	ifstream read_in;
	read_in.open("city_48.txt");
	if (!read_in.is_open())
	{
		cout << "read file failed." << endl;
		return;
	}

	read_in >> G.vex_num;
 	G.arc_num = 0; 
	for (int i = 0; i < G.vex_num; i++)
	{
		read_in >> G.vexs[i];
	}
	G.vexs[G.vex_num] = '\0';	

	for (int i = 0; i < G.vex_num; i++)
	{
		for (int j = 0; j < G.vex_num; j++)
		{
			read_in >> G.arcs[i][j];

 			if (G.arcs[i][j] > 0)
			{
				G.arc_num++;
			}
		}
	}

	cout << " –³ŒüƒOƒ‰ƒtF" << endl;
	cout << "yVertexz G.vex_num = " << G.vex_num << endl;
	cout << "yArcz G.arc_num = " << G.arc_num << endl;
	cout << "yVertex vectorz vexs[max_vexNum] = ";

	for (int i = 0; i < G.vex_num; i++)
	{
		cout << G.vexs[i] << " ";
	}
	cout << endl << "y—×Ús—ñz arcs[max_vexNum][max_vexNum] ”@‰ºF" << endl;
	//for (int i = 0; i < G.vex_num; i++)
	//{
	//	for (int j = 0; j < G.vex_num; j++)
	//	{
	//		cout << std::right << setw(4) << G.arcs[i][j] << " ";
	//	}
	//	cout << endl;
	//}
}

TSP_solution SA_TSP(Graph G) {
	srand(unsigned(time(0)));

 	double Current_Temperature = INITIAL_TEMPERATURE;

 	TSP_solution Best_solution;
	Best_solution.length_path = MAX_INT;

 	for (int i = 0; i < G.vex_num; i++)
	{
		Best_solution.path[i] = 'A' + i;
	}
	random_shuffle(Best_solution.path + 1, Best_solution.path + G.vex_num);

 	TSP_solution Current_solution;

 	while (MIN_TEMPERATURE < Current_Temperature) {
   		for (int i = 0; i < LEGNTH_Mapkob; i++)
		{
			Current_solution = FindNewSolution(G, Best_solution);
			if (Current_solution.length_path <= Best_solution.length_path)	// ÚŽóVŒ¦E
			{
				if (Current_solution.length_path == Best_solution.length_path)
				{
 				}
				Best_solution = Current_solution;
			}
			else {	
				if ((int)exp((Best_solution.length_path - Current_solution.length_path) / Current_Temperature) * 100 > (rand() * 101))
				{
					Best_solution = Current_solution;
				}
				else {
 				}
			}
		}
		Current_Temperature *= SPEED; 

	} 

	return Best_solution;
}

TSP_solution FindNewSolution(Graph G, TSP_solution bestSolution) {
 	TSP_solution newSolution;

 	int i = rand() % (G.vex_num - 1) + 1;	
	int j = rand() % (G.vex_num - 1) + 1;

	if (i > j)
	{
		int temp = i;
		i = j;
		j = temp;
	}
	else if (i == j)
	{	

		newSolution = bestSolution;
		return newSolution;
	}


	int choose = rand() % 3;
	if (choose == 0)
	{	
		char temp = bestSolution.path[i];
		bestSolution.path[i] = bestSolution.path[j];
		bestSolution.path[j] = temp;
	}
	else if (choose == 1)
	{	
		reverse(bestSolution.path + i, bestSolution.path + j);
	}
	else {	
		if (j + 1 == G.vex_num) 
		{
			newSolution = bestSolution;
			return newSolution;
		}
		rotate(bestSolution.path + i, bestSolution.path + j, bestSolution.path + j + 1);
	}
	newSolution = bestSolution;
	newSolution.path[G.vex_num] = newSolution.path[0];   
	newSolution.path[G.vex_num + 1] = '\0';


	newSolution.length_path = CalculateLength(G, newSolution);

	return newSolution;
}

int CalculateLength(Graph G, TSP_solution newSolution) {
	int _length = 0;

	for (int i = 0; i < G.vex_num - 1; i++)
	{
		int _startCity = (int)newSolution.path[i] - 65;
		int _endCity = (int)newSolution.path[i + 1] - 65;
		if (G.arcs[_startCity][_endCity] == -1)
		{
			return MAX_INT;
		}
		else {
			_length += G.arcs[_startCity][_endCity];
		}
	}

 	if (G.arcs[(int)newSolution.path[G.vex_num - 1] - 65][(int)newSolution.path[0] - 65] == -1)
	{
		return MAX_INT;
	}
	else {
		_length += G.arcs[(int)newSolution.path[G.vex_num - 1] - 65][(int)newSolution.path[0] - 65];
 		return _length;
	}

}

void Display(Graph G, TSP_solution bestSoluion) {
	cout << "****************************** TSP_SA - BestSolution ******************************" << endl;

	cout << "bestSoluion.path[ ] = ";
	for (int i = 0; i < G.vex_num; i++)
	{
		cout << bestSoluion.path[i] << "-->";
	}
	cout << bestSoluion.path[G.vex_num] << endl;

	cout << "bestSoluion.length_path = " << bestSoluion.length_path << endl;;


	cout << "***********************************************************************************" << endl;
}