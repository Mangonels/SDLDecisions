#include "Grid.h"
#include <iostream>
#include <cmath>

using namespace std;

Grid::Grid() { //Generador de grid
	num_cell_x = SRC_WIDTH / CELL_SIZE; //Calcula la cantidad de celdas en X basandose en el tamaño de la pantalla y el tamaño de una casilla
	num_cell_y = SRC_HEIGHT / CELL_SIZE; //Lo mismo en Y
	nodeGrid.resize(num_cell_x); //Empezamos estableciendo el tamaño del vector de vectores en X
	
	//Hemos implementado la grid como un vector de vectores de nodos (Vector 2D), aquí expandimos en Y:
	for (vector<vector<Node>>::iterator it = nodeGrid.begin(); it != nodeGrid.end(); ++it) {
		it->resize(num_cell_y);
	}
}

Grid::~Grid() {

}
void Grid::InitGrid(std::vector<std::vector<int>> terrain) { //Inicializa la grid con valores por defecto

	for (int i = 0; i < num_cell_x; i++) {
		for (int j = 0; j < num_cell_y; j++) {
			nodeGrid[i][j].x = i;
			nodeGrid[i][j].y = j;
			nodeGrid[i][j].start = false;
			nodeGrid[i][j].goal = false;
			nodeGrid[i][j].explored = false;
			nodeGrid[i][j].isValid = terrain[i][j]; //false --> obstacle
			nodeGrid[i][j].came_from = NULL;
			nodeGrid[i][j].cost = 1;
		}
	}
}

void Grid::Clear() { //Reinicia la grid de nodos. Se utiliza a la hora de localizar nuevos objetivos

	for (int i = 0; i < num_cell_x; i++) {
		for (int j = 0; j < num_cell_y; j++) {
			nodeGrid[i][j].start = false;
			nodeGrid[i][j].goal = false;
			nodeGrid[i][j].explored = false;
			nodeGrid[i][j].came_from = NULL;
			nodeGrid[i][j].cost = 1;
			nodeGrid[i][j].cost_so_far = NULL;
			nodeGrid[i][j].priority = NULL;
		}
	}
}

vector<Node> Grid::Neighbors(Node n) { //Devuelve vector con nodos vecinos al nodo dado
	vector<Node> neighbors;
	//up
	if (n.y - 1 >= 0) {
		neighbors.push_back(nodeGrid[n.x][n.y - 1]);
	}
	//down
	if (n.y + 1 < num_cell_y) {
		neighbors.push_back(nodeGrid[n.x][n.y + 1]);
	}
	//right
	if (n.x + 1 < num_cell_x) {
		neighbors.push_back(nodeGrid[n.x + 1][n.y]);
	}
	//left
	if (n.x - 1 >= 0) {
		neighbors.push_back(nodeGrid[n.x - 1][n.y]);
	}

	if (useDiagonalNeighbours) { //Considerar tambien nodos diagonales para ser incluidos en el vector de vecinos?
		//up left
		if (n.y - 1 >= 0 && n.x - 1 >= 0) {
			neighbors.push_back(nodeGrid[n.x - 1][n.y - 1]);
		}
		//up right
		if (n.y - 1 >= 0 && n.x + 1 < num_cell_x) {
			neighbors.push_back(nodeGrid[n.x + 1][n.y - 1]);
		}
		//down left
		if (n.y + 1 < num_cell_y && n.x - 1 >= 0) {
			neighbors.push_back(nodeGrid[n.x - 1][n.y + 1]);
		}
		//down right
		if (n.y + 1 < num_cell_y && n.x + 1 < num_cell_x) {
			neighbors.push_back(nodeGrid[n.x + 1][n.y + 1]);
		}
	}
	
	if (useLeftRightTunnels) { //Activar tuneles a derecha e izquierda de la node grid?
		if (n.x == 0 && n.y == 10 || n.x == 0 && n.y == 11 || n.x == 0 && n.y == 12) { //Portal izquierdo
			neighbors.push_back(nodeGrid[num_cell_x - 1][n.y]);
		}

		else if (n.x == num_cell_x - 1 && n.y == 10 || n.x == num_cell_x - 1 && n.y == 11 || n.x == num_cell_x - 1 && n.y == 12) { //Portal derecho
			neighbors.push_back(nodeGrid[0][n.y]);
		}
	}

	return neighbors;
}
void Grid::NewTarget(Vector2D target) { //Recibe el objetivo
	if (goal != NULL) goal->goal = false;
	nodeGrid[target.x][target.y].goal = true; //El objetivo recibido es el objetivo (xD)
	goal = &nodeGrid[target.x][target.y]; //Directamente establecemos el objetivo
}

int Grid::Heuristic(Vector2D current, Vector2D goal) {
	
	return abs(current.x - goal.x) + abs(current.y - goal.y);
}
Path Grid::BreadthFirst(Vector2D startPos) { //Buscador de caminos mediante Breadth First Search
	
	Node *current; //Nodo que estamos tratando en el momento
	Node *start = &nodeGrid[startPos.x][startPos.y]; //Define qual es el nodo de inicio. Recibiremos las coordenadas de inicio del agente desde el ScenePathFinding.cpp
	start->start = true; //Confirmar que el nodo start es el nodo de inicio (Útil para el camino mas corto de vuelta) Queda grabado en la grid ya que la pasamos por referencia

	//BUSQUEDA DE CAMINOS:
	queue<Node> frontier; //Frontera de nodos que estamos tratando
	frontier.push(*start); //El nodo inicial forma parte de la frontera
	while (!frontier.empty()) { //Mientras la frontera no quede vacía = ya no quedan nodos en la grid o no son accesibles, ejecutamos lo siguiente...
		current = &nodeGrid[frontier.front().x][frontier.front().y]; //El primer elemento de la cola es el ultimo que hemos añadido y por lo tanto el actual que tratamos
		frontier.pop(); //elimina el primer elemento de la cola (el mas viejo, cola FIFO)
		vector<Node> neighbors = this->Neighbors(*current); //Busca los vecinos del nodo actual
		for (vector<Node>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) { //Itera de principio a fin el array de vecinos recibido en la linea anterior
			if (it->isValid && !it->start && !it->explored) { //Mientras sea valido, no sea el nodo inicial y no haya sido ya explorado
				nodeGrid[it->x][it->y].came_from = current; //Anotamos al vecino, el nodo que estamos tratando, como anterior (Esto sirve para poder dibujar el camino de vuelta)
				nodeGrid[it->x][it->y].explored = true; //No usamos array de nodos explorados, simplemente lo anotamos como explorado directamente en la grid. (Por esta razón entre otras hacemos un reset entre busquedas)
				frontier.push(nodeGrid[it->x][it->y]); //Añadimos el vecino admitido en la frontera
			}
		} 
	} //- En este punto hemos conseguido que la nodeGrid contenga todos los posibles caminos hasta el objetivo -

	//ASIGNACIÓN DE CAMINO:
	current = goal; //Nos posicionamos en el objetivo como nodo actual
	Path path; //El camino que devolveremos a ScenePathFinding.cpp para realizar la trayectoria hasta el goal
	while (current != NULL) { //Esta parte del codigo define el camino a realizar por el agente
		Vector2D point; //Punto a insertar en el camino
		int offset = CELL_SIZE / 2; //El camino contiene puntos y no casillas, el Agente necesita un punto al qual moverse, por lo que usamos un offset para...
		//Establecer el punto central de la casilla como punto:
		point.x = current->x * CELL_SIZE + offset;
		point.y = current->y * CELL_SIZE + offset;
		path.points.insert(path.points.begin(), point); //Insertamos al inicio del vector del path nuestro punto
		current = current->came_from; //El siguiente nodo que insertarenos al path será el nodo desde el que llegamos al goal o nodo current, cuando estabamos configurando la grid
	}
	return path; //Devolvemos el camino
}

Path Grid::Dijkstra(Vector2D startPos) { //Buscador de caminos mediante Dijkstra
	Node *current;
	Node* start = &nodeGrid[startPos.x][startPos.y];
	start->start = true;

	//BUSQUEDA DE CAMINOS:
	priority_queue<Node> frontier; //La cola es de prioridad en este caso
	frontier.push(*start);
	while (!frontier.empty()) {
		current = &nodeGrid[frontier.top().x][frontier.top().y];
		frontier.pop();

		vector<Node> neighbors = this->Neighbors(*current);
		for (vector<Node>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
			//Como estamos iterando vecinos, vamos a establecer como coste del vecino la suma del coste acumulado del que venimos, con el coste del vecino:
			int new_cost = current->cost_so_far + it->cost; 
			if (it->isValid && !it->start && it->cost_so_far == NULL || new_cost < it->cost_so_far) {
				nodeGrid[it->x][it->y].cost_so_far = new_cost; //Coste acumulado calculado anteriormente
				nodeGrid[it->x][it->y].priority = new_cost; //La prioridad se basa en el coste
				nodeGrid[it->x][it->y].came_from = current; //El nodo del que venimos para analizar el vecino es el actual
				frontier.push(nodeGrid[it->x][it->y]); //Añadimos el nodo vecino a la cola de prioridad frontera
			}
		}
	} //- Como en breadth, hasta aquí ya tenemos la grid mapeada con los valores pertinentes y casillas anteriores. Pero además ahora tenemos el factor prioridad por coste. -

	//ASIGNACIÓN DE CAMINO:
	current = goal;
	Path path;
	while (current != NULL) {
		Vector2D point;
		int offset = CELL_SIZE / 2;
		point.x = current->x * CELL_SIZE + offset;
		point.y = current->y * CELL_SIZE + offset;
		path.points.insert(path.points.begin(), point);
		current = current->came_from;
	}
	return path;
}

Path Grid::GreedyBreadth(Vector2D startPos) { //Buscador de caminos mediante Greedy Breadth First Search
	Node *current;
	Node* start = &nodeGrid[startPos.x][startPos.y];
	start->start = true;

	//BUSQUEDA DE CAMINOS:
	priority_queue<Node> frontier;
	frontier.push(*start);

	Vector2D goalPos = Vector2D(goal->x, goal->y); //Nos guardamos el objetivo en un vector

	while (!frontier.empty()) {
		current = &nodeGrid[frontier.top().x][frontier.top().y];
		frontier.pop();

		if (current == goal) break;

		vector<Node> neighbors = this->Neighbors(*current);
		for (vector<Node>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
			if (it->isValid && !it->start && it->came_from == NULL) {
				Vector2D curPos = Vector2D(it->x, it->y);
				nodeGrid[it->x][it->y].priority = Heuristic(curPos, goalPos); //En este caso utilizamos la distancia entre la posicion actual y el objetivo para saber la prioridad
				nodeGrid[it->x][it->y].came_from = current;
				frontier.push(nodeGrid[it->x][it->y]);
			}
		}
	}

	//ASIGNACIÓN DE CAMINO:
	current = goal;
	Path path;
	while (current != NULL) {
		Vector2D point;
		int offset = CELL_SIZE / 2;
		point.x = current->x * CELL_SIZE + offset;
		point.y = current->y * CELL_SIZE + offset;
		path.points.insert(path.points.begin(), point);
		current = current->came_from;
	}
	return path;
}

Path Grid::APlus(Vector2D startPos) { //Buscador de caminos mediante Aplus.
	//Combinación de Greedy y Dijkstra
	Node *current;
	Node* start = &nodeGrid[startPos.x][startPos.y];
	start->start = true;

	//BUSQUEDA DE CAMINOS:
	priority_queue<Node> frontier;
	frontier.push(*start);

	Vector2D goalPos = Vector2D(goal->x, goal->y);

	while (!frontier.empty()) {
		current = &nodeGrid[frontier.top().x][frontier.top().y];
		frontier.pop();

		if (current == goal) break;

		vector<Node> neighbors = this->Neighbors(*current);
		for (vector<Node>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
			int new_cost = current->cost_so_far + it->cost;
			if (it->isValid && !it->start && it->cost_so_far == NULL || new_cost < it->cost_so_far) {
				Vector2D curPos = Vector2D(it->x, it->y);
				nodeGrid[it->x][it->y].cost_so_far = new_cost; //Vamos a necesitar esto para implementar Dijkstra en nuestro Aplus
				nodeGrid[it->x][it->y].priority = Heuristic(curPos, goalPos) + new_cost; //Combinamos las heuristicas de Greedy y Dijkstra para formar la heuristica de Aplus.
				nodeGrid[it->x][it->y].came_from = current;
				frontier.push(nodeGrid[it->x][it->y]);
			}
		}
	}

	//ASIGNACIÓN DE CAMINO:
	current = goal;
	Path path;
	while (current != NULL) {
		Vector2D point;
		int offset = CELL_SIZE / 2;
		point.x = current->x * CELL_SIZE + offset;
		point.y = current->y * CELL_SIZE + offset;
		path.points.insert(path.points.begin(), point);
		current = current->came_from;
	}
	return path;
}