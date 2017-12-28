#pragma once
#include <utility> //pairs

class State
{
private:

public:
	State();
	~State();
	void Enter();
	void Update();
	void Exit();
};

/*Mina: x20 y3
Home: x8 y20
Taberna: x21 y20
Banco: x34 y20
Nowhere: x20 y11*/

class Home : public State{ //Estado que determina que el agente debe dirigirse o permanecer en casa
private:
	//Casilla posicionamiento:
	int x = 8;
	int y = 20;
public:
	std::pair<int, int> getLocation();
	void Enter();
	void Update();
	void Exit();
};

class Mine : public State{
private:
	int x = 20;
	int y = 3;
public:
	std::pair<int, int> getLocation();
	void Enter();
	void Update();
	void Exit();
};

class Bank : public State{
private:
	int x = 34;
	int y = 20;
public:
	std::pair<int, int> getLocation();
	void Enter();
	void Update();
	void Exit();
};

class Saloon : public State{
private:
	int x = 21;
	int y = 20;
public:
	std::pair<int, int> getLocation();
	void Enter();
	void Update();
	void Exit();
};