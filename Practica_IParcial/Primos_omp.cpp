#include <iostream>
#include <omp.h>
#include <vector>
#include <math.h>
using namespace std;

bool tieneDivisor(int num, int limSuperior) {
	bool tieneDivisor = false;

	for (int i = 2; i <= limSuperior; ++i) {
		if (num%i == 0) {
			tieneDivisor = true;
			break;
		}
	}
	return tieneDivisor;
}


int main() {
	
	int p = 0; //Limite superior en el rango a analizar [2,P].
	int cntPrimos = 1; //Contador de numeros primos. Comienza en 1 porque el 2 ya es tomado como primer numero primo.
	int max_thread_num = omp_get_max_threads();
	int thread_count = 0;
	double wtime = 0.0;
	vector<int> primos;

	while (p <= 2 && thread_count < 1) {
		cout << "Ingrese la cantidad de hilos: " << endl;
		cin >> thread_count;
		cout << "Ingrese el numero hasta el cual quiere determinar los numeros primos: " << endl;
		cin >> p;
	}
	
	primos.push_back(2); //2 primer numero primo
	wtime = omp_get_wtime();

#	pragma omp parallel for num_threads(thread_count)
	for (int num = 2; num <= p ; ++num) {
		if (num%2 != 0) {
			//Si el numero no es par.
			int raiz = (int)sqrt(num); //Cada hilo crea su propia variable
			if (!tieneDivisor(num, raiz)) {
#			pragma omp atomic
				++cntPrimos;
#			pragma omp critical
				{
					primos.push_back(num);
				}
				
			}
		}
	}
	wtime = omp_get_wtime() - wtime;

	cout << "\nCantidad de numeros primos: " << cntPrimos << endl;
	cout << "Con un tiempo de duracion de: " << wtime << " segundos." << endl;
	
	/*
	for (int i = 0; i < primos.size(); ++i) {
		cout << primos[i] << "  ";
	}
	*/

	cin.ignore();
	cin.ignore();

	return 0;
}
