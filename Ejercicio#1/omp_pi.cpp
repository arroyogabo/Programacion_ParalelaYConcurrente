#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

bool cntHilos(int ch);
bool cntTerminos(long long ct);
double random(long begin, long end);

int main(int argc, char const *argv[]) {
    long long n = 0;
    long long i = 0;
    int thread_count = 0;
    int number_in_circle = 0;
	double sum = 0;
    double distance_squared = 0;
    double x = 0;
    double y = 0;
	double wtime;
	
	srand(time(NULL));

    while (!cntHilos(thread_count) || !cntTerminos(n)) {
		
		cout << "Digite la cantidad de hilos ( >=1 ): ";
		cin >> thread_count;
		cout << "Digite la cantidad de términos ( >=1 ): ";
		cin >> n;
	}

	// realiza el cálculo de pi:
	wtime = omp_get_wtime();
    #  pragma omp parallel for num_threads(thread_count) \
       reduction(+: number_in_circle) private(x,y,distance_squared)  //Mantengo privadas esas variables para que cada hilo mantenga sus calculos independientes.
		for (i = 0; i < n; i++) {
			
			x = ((double)rand() / RAND_MAX);
			y = ((double)rand() / RAND_MAX);
			
			distance_squared = (x*x) + (y*y);
			
			if(distance_squared <= 1){
				
				++number_in_circle;
			}
		}
		
	sum = 4.0*number_in_circle / ((double)n);
	wtime = omp_get_wtime() - wtime;
	
	cout << "Con " << n << " términos y " << thread_count << " hilos" << endl;
	cout << "el estimado de pi es " << sum << endl;
	cout << "el valor de pi es " << 4.0*atan(1.0) << endl;
	cout << "El calculo obtuvo un tiempo de duracion: " << wtime << endl;
	cin >> i; // necesario para que la pantalla de la consola no cierre al final inmediatamente.
	return 0;
}

bool cntHilos(int ch) {
	return ch >= 1;
}

// ct es la cantidad de términos de la sumatoria
bool cntTerminos(long long ct) {
	return ct >= 1;
}

double random(long begin, long end){
    //double num = begin + rand() % (end+1 - begin);
	double num = ((double)rand() / RAND_MAX);
	//cout << num << endl;
    return num;
}