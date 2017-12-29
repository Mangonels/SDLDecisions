#pragma once
#include <utility> //pairs

class State 
{
protected:
	int x;
	int y;
public:
	State();
	~State();
	virtual void Enter();
	virtual void Update();
	virtual void Exit();
};

class Home : public State{ //Estado que determina que el agente debe dirigirse o permanecer en casa
private:
	void(Home::*functionInExecution)(); // &functionInExecution (Referencia a la función que se está ejecutando
public:
	std::pair<int, int> getLocation();
	Home();
	~Home();
	void Enter();
	void Update();
	void Exit();
};

class Mine : public State{
private:
	void(Mine::*functionInExecution)();
public:
	Mine();
	~Mine();
	std::pair<int, int> getLocation();
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
	void Enter();
	void Update();
	void Exit();
};