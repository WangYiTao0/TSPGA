#ifndef _SA_H_
#define _SA_H_

#define max_vexNum 30
#define MAX_CITYNUM 48

const int LEGNTH_Mapkob = 500;
const double SPEED = 0.98;					
const double INITIAL_TEMPERATURE = 1000.0;	
const double MIN_TEMPERATURE = 0.001;		
const int MAX_INT = 999999;

typedef struct {
	int vex_num, arc_num;					
	int vexs[MAX_CITYNUM];
	double arcs[MAX_CITYNUM][MAX_CITYNUM];
}Graph;

typedef struct {
	int length_path;
	char path[MAX_CITYNUM];
}TSP_solution;

void CreateGraph(Graph& G);
TSP_solution SA_TSP(Graph G);
TSP_solution FindNewSolution(Graph G, TSP_solution bestSolution);
int CalculateLength(Graph G, TSP_solution newSolution);
bool Is_Accepted();
void Display(Graph G, TSP_solution bestSoluion);

#endif