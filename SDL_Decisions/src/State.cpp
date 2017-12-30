#include "State.h"
//Constructores y destructores
State::State() {
	timeToAction = 0;
	counter = 1;
}
State::~State() {

}
void State::Enter(Agent*) {

}
void State::Update(Agent*) {

}
void State::Exit(Agent*) {

}
void State::ExecuteCurFunction(Agent*) {
}
Mine::Mine() {
	//Establecer que coordenadas tiene este estado
	statePos.x = 20;
	statePos.y = 3;
	goBank = false;
	//Ejecutar primera funcion y guardarse referencia
	functionInExecution = &Mine::Enter;
	// esto ejecuta la function
}
Mine::~Mine() {
}
Home::Home() {
	statePos.x = 8;
	statePos.y = 20;
	functionInExecution = &Home::Enter;
}
Home::~Home() {
	
}
Bank::Bank() {
	statePos.x = 34;
	statePos.y = 20;
	goMine = false;
	functionInExecution = &Bank::Enter;
}
Bank::~Bank() {

}
Saloon::Saloon() {
	statePos.x = 21;
	statePos.y = 20;
	functionInExecution = &Saloon::Enter;
}
Saloon::~Saloon() {

}

//Enters
void Home::Enter(Agent* agent) {
	agent->nextTarget = statePos;
	agent->startNewPath = true;

	agent->stamina = 0;
	functionInExecution = &Home::Update;
}
void Mine::Enter(Agent* agent) {
	//Comprobar coordenadas, deberíamos estar en las marcadas en la clase actual

	agent->nextTarget = statePos;
	agent->startNewPath = true;
	//Si estamos en las coordenadas correctas, ejecutar funcion de inicio del estado actual
	//Si no estamos en las coordenadas correctas, cambiamos el target del pathfinder para que el agente se dirija a ellas.
	functionInExecution = &Mine::Update;
}
void Bank::Enter(Agent* agent) {
	agent->nextTarget = statePos;
	agent->startNewPath = true;

	functionInExecution = &Bank::Update;
}
void Saloon::Enter(Agent* agent) {
	agent->nextTarget = statePos;
	agent->startNewPath = true;

	functionInExecution = &Saloon::Update;
}

//Updates

void Home::Update(Agent* agent) {
	//primero comprobar si está en el area o está llegando

	//calcula deltatime y lo añade al counter
	long now = SDL_GetTicks();
	if (now > last) {
		float deltaTime = ((float)(now - last)) / 1000;
		counter += deltaTime;
		last = now;
	}
	//efectos del lugar
	if (counter > timeToAction) {
		
		agent->stamina += 5;
		if (agent->stamina >= agent->maxStamina) {
			agent->stamina = agent->maxStamina;
			functionInExecution = &Home::Exit;
		}
		counter = 0;
		
	}
}
void Mine::Update(Agent* agent) {
	//primero comprobar si está en el area o está llegando

	//calcula deltatime y lo añade al counter
	long now = SDL_GetTicks();
	if (now > last) {
		float deltaTime = ((float)(now - last)) / 1000;
		counter += deltaTime;
		last = now;
	}
	//efectos del lugar
	if (counter > timeToAction) {

		agent->goldPocket += 5;
		agent->thirst += 5;
		if (agent->goldPocket >= agent->maxGoldPocket) { //pockets full
			agent->goldPocket = agent->maxGoldPocket;
			goBank = true;
			functionInExecution = &Mine::Exit;
		}
		if (agent->thirst >= agent->maxThirst) { //thirsty
			agent->thirst = agent->maxThirst;
			goBank = false;
			functionInExecution = &Mine::Exit;
		}
		counter = 0;
	}
}
void Bank::Update(Agent* agent) {//calcula deltatime y lo añade al counter
	long now = SDL_GetTicks();
	if (now > last) {
		float deltaTime = ((float)(now - last)) / 1000;
		counter += deltaTime;
		last = now;
	}
	//efectos del lugar
	if (counter > timeToAction) {

		agent->goldBank += agent->goldPocket;
		agent->goldBankDay += agent->goldPocket;
		agent->goldPocket = 0;
		if (agent->goldBankDay >= agent->goldBankDayMax) {
			goMine = false;
			agent->goldBankDay = 0;
			functionInExecution = &Bank::Exit;
		}
		else {
			goMine = true;
			functionInExecution = &Bank::Exit;
		}
		counter = 0;
	}

}
void Saloon::Update(Agent* agent) {//primero comprobar si está en el area o está llegando

	//calcula deltatime y lo añade al counter
	long now = SDL_GetTicks();
	if (now > last) {
		float deltaTime = ((float)(now - last)) / 1000;
		counter += deltaTime;
		last = now;
	}
	//efectos del lugar
	if (counter > timeToAction) {

		agent->thirst -= 10;
		if (agent->thirst <= 0) {
			agent->thirst = 0;
			functionInExecution = &Saloon::Exit;
		}
		counter = 0;
	}
}

//Exits

void Home::Exit(Agent* agent) {
	agent->currentState = new Mine;
}
void Mine::Exit(Agent* agent) {
	//dependiendo del bool cambia a uno o otro
	if (goBank) 
		agent->currentState = new Bank;
	else 
		agent->currentState = new Saloon;
}
void Bank::Exit(Agent* agent) {
	if (goMine)
		agent->currentState = new Mine;
	else
		agent->currentState = new Home;

}
void Saloon::Exit(Agent* agent) {
	agent->currentState = new Mine;
}

void Home::ExecuteCurFunction(Agent* agent) {
	(*this.*functionInExecution)(agent);
}
void Mine::ExecuteCurFunction(Agent* agent) {
	(*this.*functionInExecution)(agent);
}
void Bank::ExecuteCurFunction(Agent* agent) {
	(*this.*functionInExecution)(agent);
}
void Saloon::ExecuteCurFunction(Agent* agent) {
	(*this.*functionInExecution)(agent);
}
//Localizaciones:
Vector2D State::GetLocation() {
	return statePos;
}
Vector2D Home::GetLocation() {
	return statePos;
}
Vector2D Mine::GetLocation() {
	return statePos;
}
Vector2D Bank::GetLocation() {
	return statePos;
}
Vector2D Saloon::GetLocation() {
	return statePos;
}