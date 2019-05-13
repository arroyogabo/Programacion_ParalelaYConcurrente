#include <iostream>
#include "Simulador.h"
#include "Tortuga.h"
#include "Contador.h"
#include "Aleatorizador.h"

using namespace std;

int main() {

	Simulador s;
	Aleatorizador::inicializar_generador_random();
	s.simular(372);

	cin.ignore();
	return 0;
}
