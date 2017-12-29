#include "State.h"

//Constructores y destructores
State::State() {
	
}
State::~State() {

}
void State::Enter() {

}
void State::Update() {

}
void State::Exit() {

}
void State::ExecuteCurFunction() {
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
void Home::Enter() {

}
void Mine::Enter() {
	//Comprobar coordenadas, deberíamos estar en las marcadas en la clase actual
	int x = 545;
	float y = 15;
	//Si estamos en las coordenadas correctas, ejecutar funcion de inicio del estado actual
	//Si no estamos en las coordenadas correctas, cambiamos el target del pathfinder para que el agente se dirija a ellas.
}
void Bank::Enter() {

	int x = 545;
	float y = 15;
}
void Saloon::Enter() {

}

//Updates

void Home::Update() {

}
void Mine::Update() {

}
void Bank::Update() {

}
void Saloon::Update() {

}

//Exits

void Home::Exit() {

}
void Mine::Exit() {

}
void Bank::Exit() {

}
void Saloon::Exit() {

}

void Home::ExecuteCurFunction() {
	(*this.*functionInExecution)();
}
void Mine::ExecuteCurFunction() {
	(*this.*functionInExecution)();
}
void Bank::ExecuteCurFunction() {
	(*this.*functionInExecution)();
}
void Saloon::ExecuteCurFunction() {
	(*this.*functionInExecution)();
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