#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <math.h>
#include <assert.h>     /* assert */
#include <random>
#include <vector>
#include <utility> //Para poder usar pair<>
using namespace std;

#include "Tortuga.h"
#include "Contador.h"
#include "Aleatorizador.h"


double stod_wrapper(string v) throw (invalid_argument, out_of_range) { return std::stod(v); }
int stoi_wrapper(string v)throw (invalid_argument, out_of_range) { return std::stoi(v); }

class Simulador
{
public:

	Simulador();
	~Simulador();


	/* FUNCI�N STATIC PARA LA DISTRIBUCI�N LOG�STICA A USAR EN LA DISTRIBUCI�N TEMPORAL DE LA ARRIBADA */
	// REQ: ( location >= 0.0 ) && ( scale > 0.0).
	// EFE: retorna un valor entre 0.0 y 1.0 que representa una probabilidad basada en la distribuci�n log�stica.
	static double random_logistic(double location, double scale);

	// Objeto compartido que genera los n�meros al azar
	static default_random_engine generator;

	// Objeto compartido que genera n�meros entre 0.0 y 1.0 al azar seg�n la distribuci�n uniforme
	static uniform_real_distribution<double> random_uniform;

	/*
	* INICIALIZADORES DE SIMULACI�N
	*/

	// EFE:inicializa las secciones en que se divide la playa con los datos en el archivo.
	void inicializarPlaya(ifstream& arch_secciones);

	// EFE: inicializa los cuadrantes con los datos en el archivo.
	void inicializarCuadrantes(ifstream& arch_cuadrantes);

	// EFE: inicializa los transectos verticales sobre la berma con los datos en el archivo.
	void inicializarTransectosVerticales(ifstream& arch_transectos_verticales);

	// EFE: inicializa el transecto paralelo a la berma con el ancho y largo indicado.
	void inicializarTransectoBerma(ifstream& arch_transecto_paralelo_berma);

	// EFE: crea la cantidad indicada de tortugas y las inicializa usando la distribuci�n normal
	// con el promedio y desviaci�n dados para la velocidad.
	void inicializarTortugas(int cntTortugas);


	// EFE: distribuye la cantidad total de tortugas que arriban, minuto a minuto, durante 360
	// minutos o 6 horas, siguiendo la distribuci�n log�stica con par�metros u y s.
	void inicializarArribada(ifstream& comportamiento_tortugas);

	// EFE: guarda los par�metros de la funci�n sinusoidal que se usa para generar la altura de la
	// marea minuto a minuto por 360 minutos o 6 horas.
	// NOTA: se debe usar modelo sinusoidal con periodo en minutos.
	void inicializarMarea(ifstream& arch_marea);

	// REQ: total_tics <= 360.
	// EFE: simula el movimiento de las tortugas y el conteo de los contadores durante 6 horas.
	// NOTA: la tarea de simular representada por este m�todo es la que debe paralelizarse usando hilos.
	void simular(int total_tics);

	/*
	* OBTENEDORES DE RESULTADOS DE LA SIMULACI�N
	*/

	// RET: total de tortugas que arribaron en la simulaci�n.
	long obtTotalTortugasArribaron();

	// RET: total de tortugas que anidaron en la simulaci�n.
	long obtTotalTortugasAnidaron();

	// RET: estimaci�n del total de tortugas que anidaron con base en el m�todo de transectos sobre la berma.
	double obtEstimacionXtransectosSobreBerma();

	// RET: estimaci�n del total de tortugas que arribaron con base en el m�todo de transecto paralelo a la berma.
	double obtEstimacionXtransectoHorizontal();

	// RET: estimaci�n del total de tortugas que anidaron con base en el m�todo de cuadrantes.
	double obtEstimacionXcuadrantes();

private:
	//Lector de archivos
	template < typename T, class F >
	vector< vector< T > > carga_valida_datos(ifstream& archivo, F t) throw (invalid_argument, out_of_range);
	template< typename T >
	void leerDatos(ifstream& arch_entrada, vector< vector <T> >& vv, string arch_name);
	int posYfinalTortuga(double indice);
	

	vector< vector<double> > playa;
	vector< vector<int> > cuadrantes;
	vector< vector<int> > transectosVerticales;
	vector< vector<double> > comportamientoTortugas;
	vector< vector<int> > transectoParaleloBerma;
	vector< vector <double> > mareas;
	vector<Contador*> contadoresC; //Contadores cuadrantes
	vector<Contador*> contadoresTV; //Contadores Transecto Vertical
	vector<Contador*> contadoresTPB; //Contadores Transecto Paralelo a la Berma.
	vector<Tortuga*> Tortugas; //Se almacenan las tortugas a simular.
	vector<pair<int, int>> posicionesFinalesTortugas; //Relacion 1:1 con Tortugas, para saber su posicion final.

	int totalTics = 0;
	long totalTortugasArribaron = 0;
	long totalTortugasAnidaron = 0;
	double estimacionXtransectosSobreBerma;
	double estimacionXtransectoHorizontal;
	double estimacionXcuadrantes;

};


Simulador::Simulador()
{
}

Simulador::~Simulador()
{
}

void Simulador::inicializarPlaya(ifstream& arch_secciones)
{
	this->playa.clear();

	this->leerDatos(arch_secciones, this->playa, "terreno.csv");
}

void Simulador::inicializarCuadrantes(ifstream& arch_cuadrantes)
{
	this->cuadrantes.clear();
	this->leerDatos(arch_cuadrantes, this->cuadrantes, "cuadrantes.csv");

	
	int cntContadores = this->cuadrantes[0][0];

	for (int i = 0; i < cntContadores; ++i) {
		Contador* c = new Contador();
		contadoresC.push_back(c);
	}

}

void Simulador::inicializarTransectosVerticales(ifstream& arch_transectos_verticales)
{
	this->transectosVerticales.clear();

	this->leerDatos(arch_transectos_verticales, this->transectosVerticales, "transectos_verticales.csv");

	int cntContadores = this->transectosVerticales[0][0];

	for (int i = 0; i < cntContadores; ++i) {
		Contador* c = new Contador();
		contadoresTV.push_back(c);
	}
}

void Simulador::inicializarTransectoBerma(ifstream& arch_transecto_paralelo_berma)
{
	this->transectoParaleloBerma.clear();

	this->leerDatos(arch_transecto_paralelo_berma, this->transectoParaleloBerma, "transecto_paralelo_berma.csv");

	int cntContadores = this->transectoParaleloBerma[0][0];

	for (int i = 0; i < cntContadores; ++i) {
		Contador* c = new Contador();
		contadoresTPB.push_back(c);
	}
}

void Simulador::inicializarArribada(ifstream& comportamiento_tortugas)
{
	this->comportamientoTortugas.clear();

	this->leerDatos(comportamiento_tortugas, this->comportamientoTortugas, "comportamiento_tortugas.csv");
}

void Simulador::inicializarTortugas(int cntTortugas)
{
	int posXTortuga;
	int posYSalidaTortuga;
	int posTerreno;
	int posYfinalTortuga;
	int cambioDeEstado;
	pair<int, int> posSalidaTortuga;
	pair<int, int> posLlegadaTortuga;
	double pendiente = (this->mareas[0][1] - this->mareas[0][0]) / (this->mareas[0][2]);  //Variacion de la marea  (Y2-Y1)/Tiempo
	double marea = this->mareas[0][0];
	double vPromedio = comportamientoTortugas[0][6]; //Velocidad Promedio.
	double desviacionVelocidad = comportamientoTortugas[0][7]; //Desviacion estandar de la velocidad.
	double promedioDuracionCamada = this->comportamientoTortugas[0][9];
	double desviacionCambio = this->comportamientoTortugas[0][10];
	normal_distribution<double> distribution(vPromedio, desviacionVelocidad);
	normal_distribution<double> distribucion_cambio_estado(promedioDuracionCamada, desviacionCambio);
	
	double escala = this->comportamientoTortugas[0][8];
	
	for (int i = 0; i < cntTortugas; ++i) {
		//Generacion aleatoria de la velocidad de la tortuga.
		double vT = distribution(Aleatorizador::generador); //vT velocidad de la tortuga.
		double ticSalida = Aleatorizador::random_logistic(0, escala);
		ticSalida += totalTics / 2;

		//Calculo de la posicion de salida de la tortuga.
		uniform_real_distribution<double> random_uniform_real_X(0, this->transectoParaleloBerma[1][1]); //Pos X inicial tortuga;
		posXTortuga = (int)random_uniform_real_X(Aleatorizador::generador); 
		posYSalidaTortuga = marea + (pendiente * ticSalida);
		posSalidaTortuga = make_pair(posXTortuga, posYSalidaTortuga); //Posicion de salida(X,Y).

		//Calculo de la posicion final de la tortuga
		uniform_real_distribution<double> random_uniform_real_posTerreno(0, 100); //Probabilidad de caer antes o despues de la berma.
		posTerreno = (int)random_uniform_real_posTerreno(Aleatorizador::generador); //Entre 0 y 100
		if (posTerreno <= 25) {
			//Tortuga debe posicionarse antes de la Berma
			int i = this->posYfinalTortuga(posTerreno);
			uniform_real_distribution<double> random_uniform_real_posYFinal(marea, this->playa[i][1]);
			posYfinalTortuga = (int)random_uniform_real_posYFinal(Aleatorizador::generador);
			//cout << posYfinalTortuga << endl;

		}
		else {
			//Tortuga debe posicionarse despues de la Berma.
			int i = this->posYfinalTortuga(posTerreno);
			uniform_real_distribution<double> random_uniform_real_posYFinal(marea, this->playa[i][3]);
			posYfinalTortuga = (int)random_uniform_real_posYFinal(Aleatorizador::generador) + this->playa[i][1]; //Se le debe sumar el trayecto de la marea a la berma
																												 //cout << posYfinalTortuga << endl;
		}

		posLlegadaTortuga = make_pair(posXTortuga, posYfinalTortuga);

		cambioDeEstado = (int)distribucion_cambio_estado(Aleatorizador::generador) / 5;

		Tortuga * tortuga = new Tortuga();
		tortuga->asgVelocidad(vT);
		tortuga->asgTicSalida(ticSalida);
		tortuga->asgPosicion(posSalidaTortuga);
		tortuga->asgPosFinal(posLlegadaTortuga);
		tortuga->asgTicCambioEstado(cambioDeEstado);

		this->Tortugas.push_back(tortuga);
	}
}

void Simulador::inicializarMarea(ifstream& arch_marea)
{
	this->mareas.clear();

	this->leerDatos(arch_marea, this->mareas, "marea.csv");
}

void Simulador::simular(int total_tics)
{
	this->totalTics = total_tics;
	//Pruebas inicializadores
	ifstream e;
	this->inicializarPlaya(e);
	this->inicializarCuadrantes(e);
	this->inicializarTransectosVerticales(e);
	this->inicializarArribada(e);
	this->inicializarMarea(e);
	this->inicializarTransectoBerma(e);
	this->inicializarTortugas(10);

	
	for (int i = 0; i < 10; ++i) {
		Tortuga * t = Tortugas[i];
		cout<< i << " Velocidad "<< t->obtVelocidad() << endl;
		cout << "Tic de salida " << t->obtTicSalida() << endl;
		cout << "Posicion " << t->obtPosicion().first << " , "<< t->obtPosicion().second << endl;
		cout << "Posicion Final " << t->obtPosFinal().first << " , "<< t->obtPosFinal().second << "\n"<< endl;
	}
	
	
	int tic = 0;
	int estado;
	double proba;
	double pendiente = (this->mareas[0][1] - this->mareas[0][0]) / (this->mareas[0][2]);  //Variacion de la marea  (Y2-Y1)/Tiempo
	double marea = this->mareas[0][0];
	//Tortuga::EstadoTortuga estadoTortuga;
	

	while (tic < total_tics) {
		marea += pendiente;  //Controla el crecimiento de la marea. Funciona como la coordena Y inicial de la tortuga.
		if (tic >= 170) {
			for (int i = 0; i < Tortugas.size(); ++i) {
				//Salida de las tortugas.
				if (Tortugas[i]->obtTicSalida() == tic && !Tortugas[i]->obtSalio()) {
					Tortugas[i]->avanzar(tic);
					Tortugas[i]->asgSalio(); //Cambia el booleano de salida de la Tortuga a true.
 					//cout << i << " Posicion de salida: " << Tortugas[i]->obtPosicion().first << " , " << Tortugas[i]->obtPosicion().second << endl;
					cout << endl;

				}

				if (Tortugas[i]->obtSalio() && Tortugas[i]->obtEstado() == Tortuga::EstadoTortuga::vagar) {
					Tortugas[i]->avanzar(tic);
					//cout << "Avanzando ..." << endl;
				}

				if (Tortugas[i]->obtPosicion() == Tortugas[i]->obtPosFinal() && Tortugas[i]->obtEstado() != Tortuga::EstadoTortuga::inactiva) {
					//cout <<" Posicion final: " << Tortugas[i]->obtPosicion().first << " , " << Tortugas[i]->obtPosicion().second << endl;
					estado = Tortugas[i]->obtEstado();
					//cout << "Estado actual : " << estado << endl;
					proba = this->comportamientoTortugas[0][estado];
					Tortugas[i]->cambiarEstado(proba);

					estado = Tortugas[i]->obtEstado();
					if (Tortugas[i]->obtAnido()) {
						++totalTortugasAnidaron;
					}

				}
			}
		}
		
		++tic;
	}
	cout << this->totalTortugasAnidaron << endl;
}

long Simulador::obtTotalTortugasArribaron()
{
	return this->totalTortugasArribaron;  
}

long Simulador::obtTotalTortugasAnidaron()
{
	return this->totalTortugasAnidaron;
}

double Simulador::obtEstimacionXtransectosSobreBerma()
{
	return this->estimacionXtransectosSobreBerma;
}

double Simulador::obtEstimacionXtransectoHorizontal()
{
	return this->estimacionXtransectoHorizontal;
}

double Simulador::obtEstimacionXcuadrantes()
{
	return this->estimacionXcuadrantes;
}

default_random_engine Simulador::generator; // inicializaci�n de la variable static
uniform_real_distribution<double> Simulador::random_uniform(0.0, 1.0); // inicializaci�n de la variable static
double Simulador::random_logistic(double location, double scale)
{
	assert(scale > 0.);
	assert(location >= 0.);
	return location - scale * log(1. / random_uniform(generator) - 1.);
}

template<typename T, class F>
vector<vector<T>> Simulador::carga_valida_datos(ifstream & archivo, F t) throw(invalid_argument, out_of_range)
{
	vector< vector< T > > valores;
	vector< T > linea_valores;
	string linea;
	while (getline(archivo, linea)) {
		linea_valores.clear();
		stringstream ss(linea);
		string numero_S;
		T numero_T;
		while (getline(ss, numero_S, ',')) {
			try {
				numero_T = t(numero_S);
			}
			catch (exception e) {
				throw e;
			}
			linea_valores.push_back(numero_T);
		}
		valores.push_back(linea_valores);
	}
	return valores;
}

template<typename T>
void Simulador::leerDatos(ifstream & arch_entrada, vector<vector<T>>& vv, string arch_name)
{
	arch_entrada.open(arch_name);
	if (!arch_entrada)
		cout << "no encuentra el archivo de datos" << endl;
	try {
		vv = carga_valida_datos< T >(arch_entrada, stod_wrapper); // usa wrapper de stod
	}
	catch (exception e) {
		cout << "valor invalido o fuera de limite" << endl;
	}
	/*
	for (auto f : vv) {
		for (auto x : f) {
			cout << x << ',' << endl;
		}
		cout << endl;
	}
	*/
	arch_entrada.close();
}

int Simulador::posYfinalTortuga(double posXinicialTortuga){

	int indice;

	if (posXinicialTortuga <= 100) {
		indice = 0;
	}
	else if (posXinicialTortuga <= 200) {
		indice = 1;
	}
	else if (posXinicialTortuga <= 300) {
		indice = 2;
	}
	else if (posXinicialTortuga <= 400) {
		indice = 3;
	}
	else if (posXinicialTortuga <= 500) {
		indice = 4;
	}
	else if (posXinicialTortuga <= 600) {
		indice = 5;
	}
	else if (posXinicialTortuga <= 700) {
		indice = 6;
	}
	else if (posXinicialTortuga <= 800) {
		indice = 7;
	}
	else if (posXinicialTortuga <= 900) {
		indice = 8;
	}
	else if (posXinicialTortuga <= 1000) {
		indice = 9;
	}
	else if (posXinicialTortuga <= 1100) {
		indice = 10;
	}
	else if (posXinicialTortuga <= 1200) {
		indice = 11;
	}
	else if (posXinicialTortuga <= 1300) {
		indice = 12;
	}
	else if (posXinicialTortuga <= 1400) {
		indice = 13;
	}
	else if (posXinicialTortuga <= 1500) {
		indice = 14;
	}
	return indice;
}
