#include <iostream>
#include <omp.h>
#include <string>
#include <math.h>
#include <sstream>
#include <vector>
using namespace std; 

bool esPrimo(int numero) {
	bool esPrimo = true; //De entrada se asume que es primo.
	
	//Si el numero es 2 devuelve true
	if (numero != 2) {
		if (numero % 2 != 0) {
			//Si el numero no es divisible entre 2
			for (int i = 2; i <= sqrt(numero); ++i) {
				if (numero % i == 0) {
					esPrimo = false;
					break;
				}
			}
		}
		else {
			esPrimo = false;
		}
	}
	
	return esPrimo;

}

int sumaFactores(int numero) {
	
	int suma = 0;
	int divisor = 2;
	int aux = numero; //Para guardar el numero ya que despues va a ser alterado.
	ostringstream tira;

	while(divisor < aux && numero != 1) {
		if (esPrimo(divisor)) {
			while(numero%divisor == 0) {
				//Si el numero es divisible entre i, que es primo
				numero /= divisor;
				tira << divisor;
				suma += divisor;
			}
		}
		++divisor;
	}
	return suma;

}

int sumaDigitos(int numero) {
	int suma = 0;
	int divisor = 10;

	while (numero != 0) {
		suma += numero % 10;
		numero /= 10;
	}

	return suma;
}

int sumaFactores(string tira) {
	int suma = 0;
	
	if (tira.size() > 1) {
		
		for (int i = 0; i < tira.size(); ++i) {
			int num = tira.at(i);
			cout << num << endl;
			suma += tira.at(i);
		}

	}else {
		suma = stoi(tira, nullptr ,10);
	}

	return suma;
}

int main() {

	int thread_count = 0;
	int n = 0;  // Limite superior para el calculo de numeros Smith [2,N]
	double wtime = 0.0;
	while (n <= 2 || thread_count < 1) {
		cout << "Ingrese el valor de n: " << endl;
		cin >> n;
		cout << "Ingrese la cantidad de hilos: " << endl;
		cin >> thread_count;
	}
	wtime = omp_get_wtime();
#	pragma omp parallel for num_threads(thread_count) 
	for (int i = 2; i <= n; ++i) {
		if (!esPrimo(i)) {  //2 es primo 
			if (sumaFactores(i) == sumaDigitos(i)) {
				//es Smith
#				pragma omp critical 
				{
					cout << i << endl;
				}
			}

		}
	}
	wtime = omp_get_wtime() - wtime;
	cout << "Tiempo de duracion: " << wtime << " segundos" << endl;
	cin.ignore();
	cin.ignore();
	return 0;
}
