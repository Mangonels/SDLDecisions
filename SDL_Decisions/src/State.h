#pragma once
#include <utility> //pairs
#include "CurrentScene.h"

class State //Clase abstracta representativa de un estado
{
protected:
	//Posición en la que debemos estar según el estado
	int x;
	int y;
public:
	State();
	~State();

	//Funciones del loop de un estado
	virtual void Enter();
	virtual void Update();
	virtual void Exit();
	virtual void ExecuteCurFunction();
};

class Mine : public State { //Estado que determina que el agente debe dirigirse o permanecer en la mina
private:
	void(Mine::*functionInExecution)(Agent*) = NULL; // &functionInExecution (Referencia a la función que se está ejecutando
public:
	Mine(Agent* theAgent);
	~Mine();
	std::pair<int, int> getLocation();
	void ExecuteCurFunction();
	void Enter(Agent*);
	void Update(Agent*);
	void Exit();
};

class Home : public State{ 
private:
	void(Home::*functionInExecution)();
public:
	std::pair<int, int> getLocation();
	Home();
	~Home();
	void ExecuteCurFunction();
	void Enter();
	void Update();
	void Exit();
};

class Bank : public State{
private:
	void(Bank::*functionInExecution)();
public:
	Bank();
	~Bank();
	std::pair<int, int> getLocation();
	void ExecuteCurFunction();
	void Enter();
	void Update();
	void Exit();
};

class Saloon : public State{
private:
	void(Saloon::*functionInExecution)();
public:
	Saloon();
	~Saloon();
	std::pair<int, int> getLocation();
	void ExecuteCurFunction();
	void Enter();
	void Update();
	void Exit();
};