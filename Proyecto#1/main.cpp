#include <iostream>
#include "Simulador.h"
#include "Tortuga.h"
#include "Contador.h"
#include "Aleatorizador.h"
#include <vector>
#include <fstream>
#include <omp.h>
using namespace std;

int main() {

	int thread_count = 0;
	int cntTortugas = 0;
	int totalTics = 372;
	int max_thread_count = omp_get_max_threads();
	int estimadoCuadrantes = 0;
	int estimadoTV = 0;
	int estimadoTPB = 0;
	double wtime = 0.0;

	ifstream entrada;
	ofstream salida;
	Simulador s;
	vector< vector<int> > experimentos;
	Aleatorizador::inicializar_generador_random();
	/*
	while (thread_count < 1 && cntTortugas <= 0) {
		cout << "Ingrese la cantidad de tortugas que desea simular: " << endl;
		cin >> cntTortugas;
		cout << "Ingrese la cantidad de hilos con los que desea trabajar: " << endl;
		cout << "Cantidad maxima recomendada: " << max_thread_count << endl;
		cin >> thread_count;
	}

	cout << "Simulando " << cntTortugas << " tortugas con " << thread_count << " hilos" << endl;
	
	wtime = omp_get_wtime();
	s.simular(totalTics, cntTortugas, thread_count);
	wtime = omp_get_wtime() - wtime;
	cout << "En un tiempo de duracion: " << wtime << " segundos." << endl;

	estimadoCuadrantes = s.obtEstimacionXcuadrantes();
	estimadoTV = s.obtEstimacionXtransectosSobreBerma();
	estimadoTPB = s.obtEstimacionXtransectoHorizontal();

	cout << "\nGenerando archivo con resultados..." << endl;
	
	salida.open("resultado.csv");

	salida << cntTortugas << "," << thread_count << "," << estimadoCuadrantes << "," << estimadoTV << "," << estimadoTPB << "," << wtime;

	salida.close();

	cout << "\nArchivo generado." << endl;
	*/
	///CODIGO PARA CORRER TODOS LOS EXPERIMENTOS
	
	s.leerDatos(entrada, experimentos, "experimentos.csv");
	
	for (int i = 0; i < experimentos.size(); ++i) {
		int cntTortugas = experimentos[i][2];
		for (int j = 3; j < experimentos[i].size(); ++j) {
			int thread_count = experimentos[i][j];
			Simulador * s1 = new Simulador();
			cout << "Experimento con: " << cntTortugas << " tortugas y " << thread_count << " hilos" << endl;

			s1->simular(totalTics, cntTortugas, thread_count);
			delete s1;
		}
		cout << endl;
	}
	

	cin.ignore();
	cin.ignore();
	return 0;
}
