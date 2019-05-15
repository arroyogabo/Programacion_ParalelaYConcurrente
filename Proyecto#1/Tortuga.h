#pragma once

#include <utility>
#include "Aleatorizador.h"
using namespace std;

class Tortuga
{
public:

	enum EstadoTortuga { vagar, camar, excavar, poner, tapar, camuflar, inactiva };
	typedef pair< int, int > T_posicion;

	// Construye una tortuga con velocidad 1, en estado "inactiva", en posicion (0,0).
	Tortuga();

	// Construye una tortuga con velocidad v, en estado e, en posicion p.
	Tortuga(double v, EstadoTortuga e, T_posicion p);

	~Tortuga();

	void cambiarEstado(double proba);

	// Obtenedores:
	int obtTicSalida();
	int obtTicCambioEstado();
	bool obtSalio();
	bool obtAnido();
	double obtVelocidad();
	T_posicion obtPosicion();
	T_posicion obtPosFinal();
	EstadoTortuga obtEstado();
	EstadoTortuga obtNumEstado(int nE); //numEstado

	// Asignadores:
	void asgTicSalida(int ticSalida);
	void asgVelocidad(double nv);
	void asgSalio();
	void asgPosFinal(T_posicion posFinal);
	void asgPosicion(T_posicion np);
	void asgEstado(EstadoTortuga ne);
	void asgTicCambioEstado(int ticCambio);
	

	// EFE: avanza *this seg�n su velocidad y evoluciona su estado en el tic que le toque.
	void avanzar(int tic);

private:
	bool desactivarse(double proba);

	int ticSalida;
	int ticCambioEstado;
	int contadorTicEstado; //Se usa localmente para el cambio de estado.
	bool salio;
	bool anido;
	double velocidad;
	T_posicion posFinal;
	T_posicion posicion; // posicion.first == coordenada X, posicion.second = coordenada Y
	EstadoTortuga estado;


};

Tortuga::Tortuga()
{
	this->velocidad = 1.0;
	this->estado = inactiva;
	this->posicion = make_pair(0, 0);
	this->salio = false;
	this->anido = false;
	this->contadorTicEstado = 0;
}

Tortuga::Tortuga(double v, EstadoTortuga e, T_posicion p)
{
	this->velocidad = v;
	this->estado = e;
	this->posicion = p;
}

Tortuga::~Tortuga()
{

}

double Tortuga::obtVelocidad()
{
	return this->velocidad;
}

bool Tortuga::obtSalio()
{
	return this->salio;
}

bool Tortuga::obtAnido() 
{
	return this->anido;
}

Tortuga::T_posicion Tortuga::obtPosicion()
{
	return this->posicion;
}

Tortuga::EstadoTortuga Tortuga::obtEstado()
{
	return this->estado;
}

Tortuga::T_posicion Tortuga::obtPosFinal() 
{
	return this->posFinal;
}

Tortuga::EstadoTortuga Tortuga::obtNumEstado(int nE) {
	Tortuga::EstadoTortuga estado;
	switch (nE)
	{
		case 0:
			estado = Tortuga::EstadoTortuga::vagar;
			break;
		case 1:
			estado = Tortuga::EstadoTortuga::camar;
			break;
		case 2:
			estado = Tortuga::EstadoTortuga::excavar;
			break;
		case 3:
			estado = Tortuga::EstadoTortuga::poner;
			break;
		case 4:
			estado = Tortuga::EstadoTortuga::tapar;
			break;
		case 5:
			estado = Tortuga::EstadoTortuga::camuflar;
			break;
		case 6:
			estado = Tortuga::EstadoTortuga::inactiva;
			break;

	}
	return estado;
}

int Tortuga::obtTicSalida()
{
	return this->ticSalida;
}

int Tortuga::obtTicCambioEstado() {
	return this->ticCambioEstado;
}

void Tortuga::asgVelocidad(double nv)
{
	this->velocidad = nv;
}

void Tortuga::asgPosicion(T_posicion np)
{
	this->posicion = np;
}

void Tortuga::asgEstado(EstadoTortuga ne)
{
	this->estado = ne;
}

void Tortuga::asgPosFinal(T_posicion posFinal)
{
	this->posFinal = posFinal;
}

void Tortuga::asgTicSalida(int ticSalida)
{
	this->ticSalida = ticSalida;
}

void Tortuga::asgSalio() {
	
	this->salio = true; 

}

void Tortuga::asgTicCambioEstado(int ticCambio) {
	this->ticCambioEstado = ticCambio;
}

void Tortuga::avanzar(int tic)
{
	this->estado = vagar;

	if (this->posicion.second >= this->posFinal.second) {
		this->posicion.second = this->posFinal.second;
	}
	else {
		this->posicion.second += this->velocidad;
	}

}

void Tortuga::cambiarEstado(double proba) {
	int estado = this->obtEstado();
	if (estado == 0) {

		if (!this->desactivarse(proba)) {
			this->estado = camar;
			//cout << "Despues del cambio de estado : " << this->estado << endl;
		}
		else {
			this->estado = inactiva;
		}
		
	}else {
		if (this->contadorTicEstado == this->ticCambioEstado) {
			if (!this->desactivarse(proba)) {
				++estado;
				this->estado = this->obtNumEstado(estado);
				if (this->estado == inactiva) { 
					this->anido = true; //Si llego hasta "camuflar" quiere decir que anido con exito
				}
				this->contadorTicEstado = 0;
				//cout << "Despues del cambio de estado : " << this->estado << endl;
			}
			else {
				this->estado = inactiva;
			}
		}
		else {
			++this->contadorTicEstado;
		}
	}	
}

bool Tortuga::desactivarse(double proba) {
	bool desactivar = false;
	double azar;
	uniform_real_distribution<double> random_uniform_real(0.0, 1.0); //Pos X inicial tortuga;
	//azar = random_uniform_real(Aleatorizador::generador);
	//cout << "Proba de desactivarse " << azar << endl;
	azar = 0.8;
	if (azar <= proba) {
		desactivar = true;
	}
	return desactivar;
}
