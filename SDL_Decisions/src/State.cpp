#include "State.h"

//Constructores y destructores
State::State() {
	
}
State::~State() {

}

Mine::Mine() {
	//Establecer que coordenadas tiene este estado
	x = 20;
	y = 3;
	//Ejecutar primera funcion y guardarse referencia
	functionInExecution = &Enter;
	//functionInExecution(); NO CHUTA ESTA MIERDA
}
Mine::~Mine() {

}
Home::Home() {
	x = 8;
	y = 20;
	functionInExecution = &Enter;
}
Home::~Home() {

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