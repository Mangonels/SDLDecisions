#include "State.h"
//Constructores y destructores
State::State() {
	
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

}
void Mine::Enter(Agent* agent) {
	//Comprobar coordenadas, deberíamos estar en las marcadas en la clase actual
	int x = 545;
	float y = 15;
	functionInExecution = &Mine::Update;

	//Si estamos en las coordenadas correctas, ejecutar funcion de inicio del estado actual
	//Si no estamos en las coordenadas correctas, cambiamos el target del pathfinder para que el agente se dirija a ellas.
}
void Bank::Enter(Agent* agent) {

	int x = 545;
	float y = 15;
}
void Saloon::Enter(Agent* agent) {

}

//Updates

void Home::Update(Agent* agent) {

}
void Mine::Update(Agent* agent) {
	int kik = 0;
}
void Bank::Update(Agent* agent) {

}
void Saloon::Update(Agent* agent) {

}

//Exits

void Home::Exit(Agent* agent) {

}
void Mine::Exit(Agent* agent) {

}
void Bank::Exit(Agent* agent) {

}
void Saloon::Exit(Agent* agent) {

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