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

	int totalTics = 372;
	int max_thread_count = omp_get_max_threads();
	int estimadoCuadrantes = 0;
	int estimadoTV = 0;
	int estimadoTPB = 0;
	double wtime = 0.0;

	ifstream entrada;
	ofstream salida;
	ofstream cuadro;
	Simulador s;
	vector<double> tiemposPromedio;
	vector< vector<int> > experimentos;
	Aleatorizador::inicializar_generador_random();
	
	
	///CODIGO PARA CORRER TODOS LOS EXPERIMENTOS
	
	s.leerDatos(entrada, experimentos, "experimentos.csv");
	salida.open("resultado.csv");
	for (int i = 0; i < experimentos.size(); ++i) {
		int cntTortugas = experimentos[i][2];
		for (int j = 3; j < experimentos[i].size(); ++j) {
			int thread_count = experimentos[i][j];
			int contador = 0;
			double wtime = 0.0;
			salida << "===================== EXPERIMENTO # " << i+1 << " ==========================\n";
			while (contador < 10) {
				double wtime_local = 0.0;
				Simulador * s1 = new Simulador();

				cout << "\nExperimento con: " << cntTortugas << " tortugas y " << thread_count << " hilos" << endl;

				s1->simular(totalTics, cntTortugas, thread_count);
				wtime += s1->getWtime();
				wtime_local += s1->getWtime();
				estimadoCuadrantes = s1->obtEstimacionXcuadrantes();
				estimadoTV = s1->obtEstimacionXtransectosSobreBerma();
				estimadoTPB = s1->obtEstimacionXtransectoHorizontal();

				salida << contador+1 << " Cantidad de Tortugas simuladas: " << cntTortugas << "  ";
				salida << "Cantidad de hilos: " << thread_count << "  ";
				salida << "Cuadrantes: " << estimadoCuadrantes << "  ";
				salida << "TV: " << estimadoTV << "  ";
				salida << "TPB: " << estimadoTPB << "  ";
				salida << "Duracion (s): " << wtime_local << "\n";

				delete s1;
				++contador;
			}
			cout << "Tiempo de duracion total entre las 10 corridas: " << wtime << endl;
			cout << "\n" << endl;
			salida << "Tiempo total de las 10 corridas: " << wtime << "  ";
			double tPromedio = wtime / 10.0;
			tiemposPromedio.push_back(tPromedio);
			salida << "Tiempo promedio por corrida: " << tPromedio << "\n\n";
		}
	}
	
	salida.close();
	cout << "\nArchivo generado." << endl;

	/*
	cuadro.open("Cuadro_Comparativo.txt");

	cuadro << "# Hilos\t\t\t" << "Experimento #1\t\t\t" << "Experimento #2\t\t\t" << "Experimento #3\n";
	for (int i = 0; i < experimentos.size(); ++i) {
		double wtime = 0.0;
		double aceleracion = 0.0;
		if (i == 0) {
			wtime = tiemposPromedio[i];
			aceleracion = wtime;
		}else {
			if (i < 3) {
				wtime = tiemposPromedio[i];
				aceleracion = tiemposPromedio[i - 1] / wtime;
			}else {
				if (i < 6) {
					if (i == 3) {
						wtime = tiemposPromedio[i];
						aceleracion = wtime;
					}
					wtime = tiemposPromedio[i];
					aceleracion = tiemposPromedio[i - 1] / wtime;
				}else {
					if (i == 6) {
						wtime = tiemposPromedio[i];
						aceleracion = wtime;
					}else {
						wtime = tiemposPromedio[i];
						aceleracion = tiemposPromedio[i - 1] / wtime;
					}
				}
			}
		}

	}

	cuadro.close();
	*/

	cin.ignore();
	cin.ignore();
	return 0;
}
