#include "State.h"

State::State() {

}
State::~State() {

}

//Enters
void State::Enter(){
	//Comprobar coordenadas, deberíamos estar en las marcadas en la clase actual
	  //Si estamos en las coordenadas correctas, ejecutar funcion de inicio del estado actual
	  //Si no estamos en las coordenadas correctas, cambiamos el target del pathfinder para que el agente se dirija a ellas.
}
void Home::Enter() {

}
void Mine::Enter() {

}
void Bank::Enter() {

}
void Saloon::Enter() {

}

//Updates
void State::Update(){
	//Si estamos en las coordenadas correctas, ejecutar funcion de update del estado actual
}

void Home::Update() {

}
void Mine::Update() {

}
void Bank::Update() {

}
void Saloon::Update() {

}

//Exits
void State::Exit() {
	//Ejecutar funcion de salida del estado actual
}

void Home::Exit() {

}
void Mine::Exit() {

}
void Bank::Exit() {

}
void Saloon::Exit() {

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