#ifndef _GA_H_
#define _GA_H_
#define MAX_CITYNUM 150
#define CITY_NUM 48				
#define GROUP_NUM 30			
#define SON_NUM 32				

const double P_INHERIATANCE = 0.05;	
const double P_COPULATION = 0.8;	
const int ITERATION_NUM = 1500;		
const double MAX_INT = 9999999.0;

typedef struct {
	int vex_num, arc_num;				
	int vexs[CITY_NUM];					
	double arcs[CITY_NUM][CITY_NUM];	
}Graph;

typedef struct {
	double length_path;
	int path[CITY_NUM];
	double P_Reproduction;
}TSP_solution;

TSP_solution TSP_Groups[GROUP_NUM];
TSP_solution Son_solution[SON_NUM];
int Length_SonSoliton = 0;

void CreateGraph(Graph& G);
void InitialGroup(Graph G);
double CalculateLength(Graph G, TSP_solution newSolution);
void TSP_Evolution(Graph G);	

int Evo_Select(Graph G);		
void Evo_Cross(Graph G, TSP_solution TSP_Father, TSP_solution TSP_Mother);	// 杂交函数
void Evo_Variation(Graph G, int Index_Variation);	
void Evo_UpdateGroup(Graph G);
void TSP_Evaluate(Graph G);		

int* Get_Conflict(int Conflict_Father[], int Conflict_Mother[], int Length_Cross, int& Length_Conflict);	// 返回冲突的数组
TSP_solution Handle_Conflict(Graph G, TSP_solution ConflictSolution, int* Detection_Conflict, int* Model_Conflict, int Length_Conflict);	// 解决冲突

void Calc_Probablity(Graph G, double total_length);	
bool Check_path(Graph G, TSP_solution CurrentSolution);
void Display(Graph G);
#endif


