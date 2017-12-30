#include "State.h"
//Constructores y destructores
State::State() {
	timeToAction = 1;
	counter = 0;
	closeTreshold = 50;
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
	x = 20;
	y = 3;
	goBank = false;
	//Ejecutar primera funcion y guardarse referencia
	functionInExecution = &Mine::Enter;
	// esto ejecuta la function
}
Mine::~Mine() {
}
Home::Home() {
	x = 8;
	y = 20;
	functionInExecution = &Home::Enter;
}
Home::~Home() {
	
}
Bank::Bank() {
	x = 34;
	y = 20;
	goMine = false;
	functionInExecution = &Bank::Enter;
}
Bank::~Bank() {

}
Saloon::Saloon() {
	x = 21;
	y = 20;
	functionInExecution = &Saloon::Enter;
}
Saloon::~Saloon() {

}

//Enters
void Home::Enter(Agent* agent) {
	std::cout << "entered: home" << std::endl;
	agent->stamina = 0;
	agent->nextTarget = Vector2D(x, y);
	agent->startNewPath = true;
	agent->nextTarget = Vector2D(x, y);
	functionInExecution = &Home::Update;
}
void Mine::Enter(Agent* agent) {
	//Comprobar coordenadas, deberíamos estar en las marcadas en la clase actual
	std::cout << "entered: mine" << std::endl;
	agent->nextTarget = Vector2D(x, y);
	agent->startNewPath = true;
	//Si estamos en las coordenadas correctas, ejecutar funcion de inicio del estado actual
	//Si no estamos en las coordenadas correctas, cambiamos el target del pathfinder para que el agente se dirija a ellas.
	functionInExecution = &Mine::Update;
}
void Bank::Enter(Agent* agent) {

	std::cout << "entered: bank" << std::endl;
	agent->nextTarget = Vector2D(x, y);
	agent->startNewPath = true;
	functionInExecution = &Bank::Update;
}
void Saloon::Enter(Agent* agent) {

	std::cout << "entered: saloon" << std::endl;
	agent->nextTarget = Vector2D(x, y);
	agent->startNewPath = true;
	functionInExecution = &Saloon::Update;
}

//Updates

void Home::Update(Agent* agent) {
	float deltaTime = 0;
	long now = SDL_GetTicks();
	if (now > last) {
		deltaTime = ((float)(now - last)) / 1000;
		last = now;
	}
	//primero comprobar si está en el area o está llegando

	float dist = Vector2D::Distance(agent->getPosition(), Vector2D(x * CELL_SIZE, y * CELL_SIZE));
	if (dist < closeTreshold) {
		counter += deltaTime;
		//efectos del lugar
		if (counter > timeToAction) {

			agent->stamina += 5;
			if (agent->stamina >= agent->maxStamina) {
				std::cout << "Reached max stamina" << std::endl;
				agent->stamina = agent->maxStamina;
				functionInExecution = &Home::Exit;
			}
			else
				std::cout << "Stamina: " << agent->stamina << std::endl;
			counter = 0;
		}
	}
}
void Mine::Update(Agent* agent) {
	float deltaTime = 0;
	long now = SDL_GetTicks();
	if (now > last) {
		deltaTime = ((float)(now - last)) / 1000;
		last = now;
	}

	//primero comprobar si está en el area o está llegando
	//calcula deltatime y lo añade al counter
	float dist = Vector2D::Distance(agent->getPosition(), Vector2D(x * CELL_SIZE, y * CELL_SIZE));
	if (dist < closeTreshold) {

		counter += deltaTime;
		//efectos del lugar
		if (counter > timeToAction) {
			counter += deltaTime;
			agent->goldPocket += 5;
			agent->thirst += 10;
			std::cout << "Thirst: " << agent->thirst << std::endl;
			std::cout << "Gold: " << agent->goldPocket << std::endl;
			if (agent->goldPocket >= agent->maxGoldPocket) { //pockets full
				std::cout << "Pockets full" << std::endl;
				agent->goldPocket = agent->maxGoldPocket;
				goBank = true;
				functionInExecution = &Mine::Exit;
			}
			if (agent->thirst >= agent->maxThirst) { //thirsty
				std::cout << "Thirsty" << std::endl;
				agent->thirst = agent->maxThirst;
				goBank = false;
				functionInExecution = &Mine::Exit;
			}
			counter = 0;
		}
	}
}
void Bank::Update(Agent* agent) { 
	//calcula deltatime y lo añade al counter
	float deltaTime = 0;
	long now = SDL_GetTicks();
	if (now > last) {
		deltaTime = ((float)(now - last)) / 1000;
		last = now;
	}

	float dist = Vector2D::Distance(agent->getPosition(), Vector2D(x * CELL_SIZE, y * CELL_SIZE));
	if (dist < closeTreshold) {
		counter += deltaTime;
		//efectos del lugar
		if (counter > timeToAction) {

			agent->goldBank += agent->goldPocket;
			agent->goldBankDay += agent->goldPocket;
			agent->goldPocket = 0;
			std::cout << "Gold in bank for today: " << agent->goldBankDay << std::endl;
			if (agent->goldBankDay >= agent->goldBankDayMax) {
				std::cout << "Enough money" << std::endl;
				goMine = false;
				agent->goldBankDay = 0;
				functionInExecution = &Bank::Exit;
			}
			else {
				std::cout << "Not enough money" << std::endl;
				goMine = true;
				functionInExecution = &Bank::Exit;
			}
			counter = 0;
		}
	}

}
void Saloon::Update(Agent* agent) {

	//calcula deltatime y lo añade al counter
	float deltaTime = 0;
	long now = SDL_GetTicks();
	if (now > last) {
		deltaTime = ((float)(now - last)) / 1000;
		last = now;
	}

	float dist = Vector2D::Distance(agent->getPosition(), Vector2D(x * CELL_SIZE, y * CELL_SIZE));
	if (dist < closeTreshold) {
		counter += deltaTime;
		//efectos del lugar
		if (counter > timeToAction) {

			agent->thirst -= 10;
			std::cout << "Thirst: " << agent->thirst << std::endl;
			if (agent->thirst <= 0) {

				std::cout << "Not thirsty" << std::endl;
				agent->thirst = 0;
				functionInExecution = &Saloon::Exit;
			}
			counter = 0;
		}
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
std::pair<int, int> Home::getLocation() {
	return std::make_pair(x, y);
}
std::pair<int, int> Mine::getLocation() {
	return std::make_pair(x, y);
}
std::pair<int, int> Bank::getLocation() {
	return std::make_pair(x, y);
}
std::pair<int, int> Saloon::getLocation() {
	return std::make_pair(x, y);
}