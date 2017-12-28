#pragma once
#include <iostream>
#define stringify( name ) # name //Pasador a strings

using namespace std;

struct Node {
	int x, y; // Coordenadas (Dentro de la grid)
	bool isValid; //Si no es un obstaculo
	bool explored; //Si la casilla ya ha sido procesada por el pathfinder
	bool start; //Nodo de inicio donde nos encontramos cuando empezamos la busqueda de camino
	bool goal; //Nodo al qual queremos encontrar el camino
	struct Node* came_from;
	int cost; //Coste para llegar a este nodo. Es necesario para Dijkstra
	int cost_so_far; //Guarda el coste acumulado del nodo + todos los anteriores (Coste acumulado del camino que forman). Es necesario para Dijkstra
	int priority; //prioridad que servirá para ordenar los nodos en la cola de prioridad
}; 

static bool operator <(const Node &p1, const Node &p2)
{
	return (p1.priority > p2.priority);
}
static bool operator ==(const Node &p1, const Node &p2)
{
	return (p1.priority == p2.priority);
}

enum Pathfinders
{
	manual,
	breadth,
	dijkstra,
	greedy,
	aplus
};

static Pathfinders pathfinder; //El pathfinder que seleccionamos en el main

static char* pathfinderNames[] = //Array con nombres del enum de pathfinders pasados a string
{
	stringify(manual),
	stringify(breadth),
	stringify(dijkstra),
	stringify(greedy),
	stringify(aplus)
};

static string getCurrentMode(Pathfinders theMode) //Devuelve que pathfinder estamos usando en formato string Ej: //getCurrentMode((Pathfinders)pathfinder);
{
	string mode = pathfinderNames[theMode];
	return mode;
}