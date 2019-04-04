#include <iostream>
#include <omp.h>
#include <algorithm>
#include <vector>
#include <math.h>
#include <time.h>

using namespace std;

bool valid_cntHilos(int cH); //cH = Cantidad de hilos.
bool valid_cntNum(long cN);
void gen_random_data(long cN, vector<int>& messy); //cN = Cantidad de numeros.

int main(int argc, char* argv[]) {

	int thread_count = 0;
	int thread_id;
	int begin;
	int mid;
	int end;
	int carga;
	double wtime;
	long cntNum = 0;
	bool ready;
	vector<int> messy;
	vector<int>::iterator it;
	vector<int>::iterator it_copy;


	srand(time(NULL));

	while (valid_cntHilos(thread_count) || valid_cntNum(cntNum)) {
		cout << "Ingrese la cantidad de hilos: " << endl;
		cin >> thread_count;
		cout << "Ingrese la cantidad de numeros que van a ser generados al azar: " << endl;
		cin >> cntNum;
	}

	messy.resize(cntNum);

	gen_random_data(cntNum, messy);

	//Imprimir los datos generados aleatoriamente.
	cout << "Datos generados aleatoriamente: ";
	//for (int i = 0; i < cntNum; ++i) {
	//cout << messy[i] << " ";
	//}
	cout << endl;

	//Tiempo de inicio del algoritmo.
	wtime = omp_get_wtime();

#	pragma omp parallel num_threads(thread_count) \
	private(thread_id, begin, end, it)
	{
		it = messy.begin();  //Iterador para recorrer el vector y pasarlo al sort.
		thread_id = omp_get_thread_num();  //Identificador del thread.
		begin = thread_id * (cntNum / thread_count);
		end = (begin + (cntNum / thread_count)) - 1;

		++end; // Esto debido a que el algoritmo sort no incluye el ultimo elemento, sino los de en medio.

			   //#		pragma omp critical
			   //	{
			   //		cout << thread_id << " tiene rangos: " << " inicio: " << begin << " final: " << end - 1 << endl;
			   //}

		sort(it + begin, it + end); //Ordenamiento en paralelo.

	}

	mid = cntNum / 2;
	carga = cntNum / thread_count;

	//Primer ordenamiento por rangos asignados a los hilos.
	//cout << "\nPrimer ordenamiento paralelo de bloques: ";
	//for (int i = 0; i < cntNum; ++i) {
	//cout << messy[i] << " ";
	//}
	//cout << endl;

	for (int i = 0; i < log2(thread_count); ++i) {

		it = messy.begin();

			ready = false;
			//Arreglar los rangos de ordenamiento para una segunda ronda.
			while (!ready) {
				end = carga * 2;

				it_copy = copy(it, it + carga, it);
				it_copy = copy(it + carga, it + end, it_copy);

				inplace_merge(it, it + carga, it_copy);

				if ((it + carga * 2) != messy.end()) {
					
					it = it + (carga * 2);

				}
				else {
					ready = true;
				}
			}
			carga *= 2;
	}
	//Tiempo de finalizacion
	wtime = omp_get_wtime() - wtime;


	//Resultado final despues de que el hilo maestro se encarga del merge.
	cout << "Resultado final: ";
	//for (int i = 0; i < cntNum; ++i) {
		//cout << messy[i] << " ";
	//}

	cout << "\nTiempo de duracion: " << wtime << endl;

	cin.ignore();
	cin.ignore();

	return 0;
}

bool valid_cntHilos(int cH) {
	return cH <= 1;
}

bool valid_cntNum(long cN) {
	return cN <= 1;
}

void gen_random_data(long cN, vector<int>& messy) {

	for (long i = 0; i < cN; ++i) {
		messy[i] = rand() % 1000;
	}
}
