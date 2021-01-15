#include <iostream>
#include <fstream>
#include <iomanip>	
#include <stdlib.h> 
#include <ctime>
#include <algorithm>

#include "GA.h"

using namespace std;

int IndexCross_i;
int IndexCross_j;

 
 
#define MAX_CITYNUM 150
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

	srand(unsigned(time(0)));
	InitialGroup(G);

	TSP_Evolution(G);	

	time_t T_end = clock();
	double RunningTime = double(T_end - T_begin) / CLOCKS_PER_SEC;
	cout << endl << "【RunningTime = " << RunningTime << " 】" << endl;


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

 	cout << " 無向グラフ：" << endl;
	cout << "【Vertex】 G.vex_num = " << G.vex_num << endl;
	cout << "【Arc】 G.arc_num = " << G.arc_num << endl;
	cout << "【Vertex vector】 vexs[max_vexNum] = ";

	for (int i = 0; i < G.vex_num; i++)
	{
		cout << G.vexs[i] << " ";
	}

	cout << endl << "【隣接行列】 arcs[max_vexNum][max_vexNum] 如下：" << endl;
	
	//for (int i = 0; i < G.vex_num;i++)
	//{
	//	for (int j = 0; j < G.vex_num; j++)
	//	{
	//		cout << G.arcs[i][j]<<" ";
	//	}
	//	cout<<endl;
	//}
	
}

void InitialGroup(Graph G) {

	cout << "----------------------【GA PARAMATER】-----------------------" << endl;
	cout << "CITY_NUM =" << CITY_NUM << endl;
	cout << "GROUP_NUM = " << GROUP_NUM << endl;
	cout << "SON_NUM = " << SON_NUM << endl;
	cout << "P_INHERIATANCE = " << P_INHERIATANCE << endl;
	cout << "P_COPULATION = " << P_COPULATION << endl;
	cout << "ITERATION_NUM = " << ITERATION_NUM << endl;

	double total_length = 0.0;
	for (int i = 0; i < GROUP_NUM; i++) {
		for (int j = 0; j < G.vex_num; j++)
		{
			TSP_Groups[i].path[j] = G.vexs[j];
		}
		random_shuffle(TSP_Groups[i].path + 1, TSP_Groups[i].path + G.vex_num);
		if (Check_path(G, TSP_Groups[i]))
		{
			TSP_Groups[i].length_path = CalculateLength(G, TSP_Groups[i]);
			total_length += TSP_Groups[i].length_path;
		}
		else {
			cout << "【error！】" << endl;
			TSP_Groups[i].length_path = MAX_INT;
			TSP_Groups[i].P_Reproduction = 0;
		}
	}

	Calc_Probablity(G, total_length);
	TSP_Evaluate(G);
}

    void Calc_Probablity(Graph G, double total_length) {
	double TempTotal_P = 0.0;

	for (int i = 0; i < GROUP_NUM; i++)
	{
		TSP_Groups[i].P_Reproduction = (1.0 / TSP_Groups[i].length_path) * total_length;
		TempTotal_P += TSP_Groups[i].P_Reproduction;
	}

	for (int i = 0; i < GROUP_NUM; i++)
	{
		TSP_Groups[i].P_Reproduction = TSP_Groups[i].P_Reproduction / TempTotal_P;
	}
}

void TSP_Evolution(Graph G) {
	/* */
	int iter = 0;
	while (iter < ITERATION_NUM) {
 
 		int Father_index = Evo_Select(G);
		int Mother_index = Evo_Select(G);

		while (Mother_index == Father_index)
		{
  			Mother_index = Evo_Select(G);
		}

 		TSP_solution Father = TSP_Groups[Father_index];
		TSP_solution Mother = TSP_Groups[Mother_index];

 		int M = GROUP_NUM - GROUP_NUM / 2;
		Length_SonSoliton = 0;	
		while (M) {
			double Is_COPULATION = ((rand() % 100 + 0.0) / 100);
			if (Is_COPULATION > P_COPULATION)
			{
 			}
			else {
 				Evo_Cross(G, Father, Mother);
				M--;
			}
		}

 		double total_length = 0.0;	

		for (int IndexVariation = 0; IndexVariation < Length_SonSoliton; IndexVariation++)
		{
			double RateVariation = (rand() % 100) / 100;
 			if (RateVariation < P_INHERIATANCE)
			{
				Evo_Variation(G, IndexVariation);
			}

 			if (!Check_path(G, Son_solution[IndexVariation]))
			{
				cout << "【Error! 】" << endl;
			}

 			Son_solution[IndexVariation].length_path = CalculateLength(G, Son_solution[IndexVariation]);
			total_length += Son_solution[IndexVariation].length_path;
		}

		Calc_Probablity(G, total_length);

  		Evo_UpdateGroup(G);

		iter++;
 	}


}

int Evo_Select(Graph G) {
	double selection_P = ((rand() % 100 + 0.0) / 100);
 
	double distribution_P = 0.0;
	for (int i = 0; i < GROUP_NUM; i++)
	{
		distribution_P += TSP_Groups[i].P_Reproduction;
		if (selection_P < distribution_P)
		{
			return i;
		}
	}
	cout << "【ERROR!】Evo_Select().." << endl;
	return 0;
}

void Evo_Cross(Graph G, TSP_solution TSP_Father, TSP_solution TSP_Mother) {
 	IndexCross_i = rand() % (CITY_NUM - 1) + 1;	
	IndexCross_j = rand() % (CITY_NUM - 1) + 1;
	if (IndexCross_i > IndexCross_j)
	{
		int temp = IndexCross_i;
		IndexCross_i = IndexCross_j;
		IndexCross_j = temp;
	}
	if (IndexCross_j == CITY_NUM || IndexCross_i == 0)
	{
		cout << "[ ... ]" << endl;
	}

 	int Father_Cross[CITY_NUM];	
	int Mother_Cross[CITY_NUM];	
	int Length_Cross = 0;		
	for (int i = IndexCross_i; i <= IndexCross_j; i++)
	{
		Father_Cross[Length_Cross] = TSP_Father.path[i];
		Mother_Cross[Length_Cross] = TSP_Mother.path[i];
		Length_Cross++;
	}

 	int* Conflict_Father;		
	int* Conflict_Mother;
	int Length_Conflict = 0;	
	Conflict_Father = Get_Conflict(Father_Cross, Mother_Cross, Length_Cross, Length_Conflict);
	Conflict_Mother = Get_Conflict(Mother_Cross, Father_Cross, Length_Cross, Length_Conflict);

 	int city_temp;
	for (int i = IndexCross_i; i <= IndexCross_j; i++)
	{
		city_temp = TSP_Father.path[i];
		TSP_Father.path[i] = TSP_Mother.path[i];
		TSP_Mother.path[i] = city_temp;
	}

 	TSP_solution Descendant_ONE = Handle_Conflict(G, TSP_Father, Conflict_Father, Conflict_Mother, Length_Conflict);	
	TSP_solution Descendant_TWO = Handle_Conflict(G, TSP_Mother, Conflict_Mother, Conflict_Father, Length_Conflict);	

	Son_solution[Length_SonSoliton++] = Descendant_ONE;
	Son_solution[Length_SonSoliton++] = Descendant_TWO;
}

TSP_solution Handle_Conflict(Graph G, TSP_solution ConflictSolution, int* Detection_Conflict, int* Model_Conflict, int Length_Conflict) {

	for (int i = 0; i <= Length_Conflict; i++)
	{
		bool flag_FindCity = false;
		int index = 0;
 		for (index = 0; index < IndexCross_i; index++)
		{
			if (Model_Conflict[i] == ConflictSolution.path[index])
			{
				flag_FindCity = true;
				break;
			}
		}

 		if (!flag_FindCity)
		{
 			for (index = IndexCross_j + 1; index < G.vex_num; index++)
			{
				if (Model_Conflict[i] == ConflictSolution.path[index])
				{
					break;
				}
			}
		}

      		ConflictSolution.path[index] = Detection_Conflict[i];
	}


	if (!Check_path(G, ConflictSolution))
	{
		cout << "【error - ......】" << endl;
	}
 
	return ConflictSolution;
}

int* Get_Conflict(int Detection_Cross[], int Model_Cross[], int Length_Cross, int& Length_Conflict) {
    	int* Conflict = new int[CITY_NUM];
	Length_Conflict = 0;
	for (int i = 0; i < Length_Cross; i++)
	{
		bool flag_Conflict = true;	
		for (int j = 0; j < Length_Cross; j++)
		{
			if (Detection_Cross[i] == Model_Cross[j])
			{
 				j = Length_Cross;
				flag_Conflict = false;	
			}
		}
		if (flag_Conflict)
		{
			Conflict[Length_Conflict] = Detection_Cross[i];
			Length_Conflict++;
		}
	}

	return Conflict;
}

void Evo_Variation(Graph G, int Index_Variation) {
 	int City_i = (rand() % (CITY_NUM - 1)) + 1;	
	int City_j = (rand() % (CITY_NUM - 1)) + 1;	

	while (City_i == City_j) {
		City_j = (rand() % (CITY_NUM - 1)) + 1;
	}

 	int temp_City = Son_solution[Index_Variation].path[City_i];
	Son_solution[Index_Variation].path[City_i] = Son_solution[Index_Variation].path[City_j];
	Son_solution[Index_Variation].path[City_j] = temp_City;
}

  void Evo_UpdateGroup(Graph G) {
	TSP_solution tempSolution;
 	for (int i = 0; i < Length_SonSoliton; i++)
	{
		for (int j = Length_SonSoliton - 1; j > i; j--)
		{
			if (Son_solution[i].length_path > Son_solution[j].length_path)
			{
				tempSolution = Son_solution[i];
				Son_solution[i] = Son_solution[j];
				Son_solution[j] = tempSolution;
			}
		}
	}

 	for (int i = 0; i < Length_SonSoliton; i++)	
	{
		for (int j = 0; j < GROUP_NUM; j++)	
		{
			if (Son_solution[i].length_path < TSP_Groups[j].length_path)
			{
				TSP_Groups[j] = Son_solution[i];	
				break;
			}
		}
	}

	TSP_Evaluate(G);
}

double CalculateLength(Graph G, TSP_solution newSolution) {
	double _length = 0;

	for (int i = 0; i < G.vex_num - 1; i++)
	{
		int _startCity = newSolution.path[i] - 1;	
		int _endCity = newSolution.path[i + 1] - 1;
		if (G.arcs[_startCity][_endCity] == -1)
		{
			return MAX_INT;
		}
		else {
			_length += G.arcs[_startCity][_endCity];
		}
	}

 	if (G.arcs[newSolution.path[G.vex_num - 1]][newSolution.path[0] - 1] == -1)
	{
		return MAX_INT;
	}
	else {
		_length += G.arcs[newSolution.path[G.vex_num - 1] - 1][newSolution.path[0] - 1];
 		return _length;
	}
}

bool Check_path(Graph G, TSP_solution CurrentSolution) {
	for (int i = 0; i < G.vex_num; i++)
	{
		for (int j = i + 1; j < G.vex_num; j++)
		{
			if (CurrentSolution.path[i] == CurrentSolution.path[j])
			{
				return false;
			}
		}
	}
	return true;
}


void TSP_Evaluate(Graph G) {
	TSP_solution bsetSolution;
	bsetSolution = TSP_Groups[0];
	for (int i = 1; i < GROUP_NUM; i++)
	{
		if (bsetSolution.length_path > TSP_Groups[i].length_path)
		{
			bsetSolution = TSP_Groups[i];
		}
	}

	cout << "Current bsetSolution = ";
	for (int i = 0; i < G.vex_num; i++)
	{
		cout << bsetSolution.path[i] << " -> ";
	}
	cout << bsetSolution.path[0] << "    length = " << bsetSolution.length_path << endl;



}
