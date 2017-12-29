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

Mine::Mine(Agent* theAgent) {
	//Establecer que coordenadas tiene este estado
	x = 20;
	y = 3;
	//Establecer referencia de la funcion a ejecutar y...
	//Se ejecutará utilizando el puntero del agente contenido en el array de agentes, y llamando a la función ExecuteCurFunction() del estado del agente:
	functionInExecution = &Mine::Enter; //Enter (Entrando en el modo mine), Porque acabamos de asignar el estado mine.
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
void Mine::Enter(Agent* theAgent) {
	
	//Comprobar coordenadas, deberíamos estar en las marcadas en la clase actual
	if (x == theAgent->getPosition().x && y == theAgent->getPosition().y) 
	{
		
	}
	else //T 
	{
		
	}

	//Si estamos en las coordenadas correctas, ejecutar funcion de inicio del estado actual
	//Si no estamos en las coordenadas correctas, cambiamos el target del pathfinder para que el agente se dirija a ellas.

	//Establecer update como funcion a ejecutar:
	functionInExecution = &Mine::Update;
}
void Bank::Enter() {


}
void Saloon::Enter() {

}

//Updates

void Home::Update() {

}
void Mine::Update(Agent*) {

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