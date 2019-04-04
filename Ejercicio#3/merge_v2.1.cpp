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
	int end;
	int carga;
	int thread_location;
	double wtime;
	long cntNum = 0;
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
	carga = cntNum / thread_count;
	thread_count /= 2;

	while (thread_count != 1) {

#	pragma omp parallel num_threads(thread_count) \
	firstprivate(it, it_copy, thread_id, end, thread_location, carga)
		{
			thread_id = omp_get_thread_num();
			thread_location = (thread_id * carga) * 2;
			it = messy.begin() + thread_location;  //Iterador para recorrer el vector y pasarlo al sort.
			end = carga * 2;

			it_copy = copy(it, it+carga, it);
			it_copy = copy(it+carga, it + end, it_copy);

			inplace_merge(it, it + carga, it_copy);
		}
		carga *= 2;
		thread_count /= 2;
	}
	it = messy.begin();
	inplace_merge(it, it + carga, messy.end());
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
