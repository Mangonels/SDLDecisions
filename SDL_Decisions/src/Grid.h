#pragma once
#include <vector>
#include <time.h>
#include <math.h>
#include <queue>
#include "Scene.h"
#include "Agent.h"
#include "Path.h"
#include "Pathfinders.h"


class Grid
{
private:
	std::vector<std::vector<Node>> nodeGrid; 
	int num_cell_x, num_cell_y;
	bool useDiagonalNeighbours = false;
	bool useLeftRightTunnels = true;
public:
	Grid::Grid();
	Grid::~Grid();
	void InitGrid(std::vector<std::vector<int>>);
	void Clear();
	std::vector<Node> Neighbors(Node);
	void NewTarget(Vector2D target);
	Node* goal;

	//Metodos pathfinding:
	int Heuristic(Vector2D, Vector2D);
	Path BreadthFirst(Vector2D);
	Path Dijkstra(Vector2D);
	Path GreedyBreadth(Vector2D);
	Path APlus(Vector2D);
	Path avoiderAPlus(Vector2D, vector<Vector2D>);
	
};