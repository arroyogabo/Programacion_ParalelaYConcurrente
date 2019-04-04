#include <iostream>
#include <omp.h>
#include <fstream>
#include <string>
#include <queue>
using namespace std;

bool validNumThreads(int ch);
bool validNumFiles(int nf);

struct tupla
{
	string linea;
	int thread_id;
};

int main(int argc, char const *argv[]) {

	int num_thread_prod = 0;// Numero de productores
	int num_thread_con = 0; // Numero de consumidores
	int thread_id;
	tupla linea_id;
	ifstream entrada;
	string linea;
	string archivo;
	string root = "C:/Users/Sergio/Documents/Universidad de Costa Rica/Cursos_I2019/Programación_Paralela_y_Concurrente/LAB1/" ;
	queue<tupla> prod_queue;

	while (!validNumThreads(num_thread_prod) || !validNumThreads(num_thread_con)) {
		cout << "Ingresa la cantidad de hilos productores: ";
		cin >> num_thread_prod;
		cout << "\nIngresa la cantidad de hilos consumidores: ";
		cin >> num_thread_con;
		
	}


#   pragma omp parallel num_threads(num_thread_prod) \
	shared(prod_queue) private(archivo, entrada, linea, linea_id, thread_id)
	{
		archivo = root + "archivo_" + to_string(omp_get_thread_num()) + ".txt";

		entrada.open(archivo); //Ubicacion relativa del archivo.
		thread_id = omp_get_thread_num();
		//Mientras no sea el final del archivo lea linea por linea.
		while (!entrada.eof()) {
			getline(entrada, linea);
			linea_id.linea = linea;
			linea_id.thread_id = thread_id;
			
#	pragma omp critical
			{
				prod_queue.push(linea_id);
			}
		}
		entrada.close();
	}
	
#	pragma omp parallel num_threads(num_thread_con) \
	shared(prod_queue) private(linea_id)
	{
		while (!prod_queue.empty()) {
#	pragma omp critical 
			{
				linea_id = prod_queue.front();
				prod_queue.pop();
			}
			replace(linea_id.linea.begin(), linea_id.linea.end(), ' ', '_');

#	pragma omp critical
			{
				cout << "Linea tokenizada por el thread: " << linea_id.thread_id << " " << linea_id.linea << endl;
			}

		}
	}

	cin.ignore();
	cin.ignore();

	return 0;
};

bool validNumThreads(int ch)
{
	return ch >= 1;
}

bool validNumFiles(int nf)
{
	return nf >= 1;
}
