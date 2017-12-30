#pragma once
#include <utility> //pairs
#include "Agent.h"
class Agent;
class State //Clase abstracta representativa de un estado
{
protected:
	//Posición en la que debemos estar según el estado
	int x;
	int y;
	long last = 0; //para calcular deltatime
	long now = SDL_GetTicks();
	float counter; //controla el tiempo, se le suma deltatime
	float timeToAction; //se ejecuta el evento del sitio en el update
	int closeTreshold;
public:
	State();
	~State();

	//Funciones del loop de un estado
	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent);
	virtual void Exit(Agent* agent);
	virtual void ExecuteCurFunction(Agent* agent);
};

class Mine : public State { //Estado que determina que el agente debe dirigirse o permanecer en la mina
private:
	void(Mine::*functionInExecution)(Agent*) = NULL; // &functionInExecution (Referencia a la función que se está ejecutando
	bool goBank;
public:
	Mine();
	~Mine();
	std::pair<int, int> getLocation();
	void ExecuteCurFunction(Agent*);
	void Enter(Agent*);
	void Update(Agent*);
	void Exit(Agent*);
};

class Home : public State{ 
private:
	void(Home::*functionInExecution)(Agent*) = NULL; 
	public:
	std::pair<int, int> getLocation();
	Home();
	~Home();
	void ExecuteCurFunction(Agent*);
	void Enter(Agent*);
	void Update(Agent*);
	void Exit(Agent*);
};

class Bank : public State{
private:
	void(Bank::*functionInExecution)(Agent*) = NULL;
	bool goMine;
public:
	Bank();
	~Bank();
	std::pair<int, int> getLocation();
	void ExecuteCurFunction(Agent*);
	void Enter(Agent*);
	void Update(Agent*);
	void Exit(Agent*);
};

class Saloon : public State{
private:
	void(Saloon::*functionInExecution)(Agent*) = NULL;
public:
	Saloon();
	~Saloon();
	std::pair<int, int> getLocation();
	void ExecuteCurFunction(Agent*);
	void Enter(Agent*);
	void Update(Agent*);
	void Exit(Agent*);
};