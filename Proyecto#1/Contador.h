#pragma once

#include <utility>
using namespace std;

class Contador
{
public:

	enum EstadoContador { contar, esperar };
	typedef pair< int, int > T_posicion;

	// Construye un contador con velocidad 1.0, en posicion (0,0), en estado esperar.
	Contador();

	// Construye un contador con velocidad v, en posicion p, en estado p.
	Contador(double v, EstadoContador e, T_posicion p);

	~Contador();

	// Obtenedores:
	int obtContadorTics();
	bool obtAvanzaDerecha();
	bool estaContando();
	double obtVelocidad();
	T_posicion obtPosicion();
	EstadoContador obtEstado();

	void avanzaDerecha();
	void avanzaIzquierda();
	void aumentarTic();
	void cambiarEstado();
	void asgVelocidad(double nv);
	void asgPosicion(T_posicion np);
	void asgEstado(EstadoContador ne);

	// EFE: avanza *this seg�n su velocidad y evoluciona su estado en el tic que le toque.
	void avanzar(int tic);

private:
	int contadorTics;
	bool avanzaDerechaC;
	bool contando;
	double velocidad;
	T_posicion posicion; // posicion.first == coordenada X, posicion.second = coordenada Y
	EstadoContador estado;
};

Contador::Contador()
{
	this->velocidad = 1.0;
	this->posicion = make_pair(0, 0);
	this->estado = esperar;
	this->contando = false;
	this->contadorTics = 0;
}

Contador::Contador(double v, Contador::EstadoContador e, T_posicion p)
{
	this->velocidad = v;
	this->posicion = p;
	this->estado = e;
}

Contador::~Contador()
{
}

int Contador::obtContadorTics() {
	return this->contadorTics;
}

bool Contador::estaContando() {
	return this->contando;
}

bool Contador::obtAvanzaDerecha()
{
	return this->avanzaDerechaC;
}

double Contador::obtVelocidad()
{
	return this->velocidad;
}

Contador::T_posicion Contador::obtPosicion()
{
	return this->posicion;
}

Contador::EstadoContador Contador::obtEstado()
{
	return this->estado;
}

void Contador::asgVelocidad(double nv)
{
	this->velocidad = nv;
}

void Contador::asgPosicion(T_posicion np)
{
	this->posicion = np;
}

void Contador::asgEstado(EstadoContador ne)
{
	this->estado = ne;
}

void Contador::avanzar(int tic)
{
	this->estado = contar;
	if (this->avanzaDerechaC) {
		this->posicion.first += this->velocidad;
	}else {
		this->posicion.first -= this->velocidad;
	}
	
}

void Contador::cambiarEstado() {
	this->contadorTics = 0;
	this->contando = !contando; 
	if (contando) {
		this->estado = contar;
	}else {
		this->estado = esperar;
	}
	//cout << this->estado << endl;
}

void Contador::aumentarTic() {
	++this->contadorTics;
}

void Contador::avanzaDerecha() {
	this->avanzaDerechaC = true;
}

void Contador::avanzaIzquierda() {
	this->avanzaDerechaC = false;
}
