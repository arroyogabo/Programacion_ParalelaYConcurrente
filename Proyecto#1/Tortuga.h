#pragma once

#include <utility>
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

	// Obtenedores:
	double obtVelocidad();
	T_posicion obtPosicion();
	EstadoTortuga obtEstado();

	// Asignadores:
	void asgVelocidad(double nv);
	void asgPosicion(T_posicion np);
	void asgEstado(EstadoTortuga ne);

	// EFE: avanza *this seg�n su velocidad y evoluciona su estado en el tic que le toque.
	void avanzar(int tic);

private:
	double velocidad;
	T_posicion posicion; // posicion.first == coordenada X, posicion.second = coordenada Y
	EstadoTortuga estado;
};

Tortuga::Tortuga()
{
	this->velocidad = 1.0;
	this->estado = inactiva;
	this->posicion = make_pair(0, 0);
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

Tortuga::T_posicion Tortuga::obtPosicion()
{
	return this->posicion;
}

Tortuga::EstadoTortuga Tortuga::obtEstado()
{
	return this->estado;
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

void Tortuga::avanzar(int tic)
{
	this->estado = vagar;
	this->posicion.second += this->velocidad;

}
