#include "State.h"

//Constructores y destructores
State::State() {
	
}
State::~State() {

}

/*Mina: x20 y3
Home: x8 y20
Saloon: x21 y20
Banco: x34 y20
Nowhere: x20 y11*/

Home::Home() {
	x = 8;
	y = 20;
	functionInExecution = &Enter;
}
Home::~Home() {

}
Mine::Mine() {
	x = 20;
	y = 3;
	functionInExecution = &Enter;
}
Mine::~Mine() {

}
Bank::Bank() {
	x = 34;
	y = 20;
	functionInExecution = &Enter;
}
Bank::~Bank() {

}
Saloon::Saloon() {
	x = 21;
	y = 20;
	functionInExecution = &Enter;
}
Saloon::~Saloon() {

}

//Enters
void State::Enter(){

}
void Home::Enter() {

}
void Mine::Enter() {
	//Comprobar coordenadas, deberíamos estar en las marcadas en la clase actual
	if (x ==  && y == ) {

	}
	else {

	}
	//Si estamos en las coordenadas correctas, ejecutar funcion de inicio del estado actual
	//Si no estamos en las coordenadas correctas, cambiamos el target del pathfinder para que el agente se dirija a ellas.
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