#!/usr/bin/python3
import enum
import numpy as np	# para generar números al azar según distribución estándar
import json			# para crear una hilera json en toJSON()

class Contador:
	"""
	Representa una Contador con id, velocidad y posicion.
	"""
	
	## VARIABLES DE CLASE
	id = 0 		## OJO variable static de clase para generar ids de Contadores
	
	## MÉTODOS DE CLASE
	""" metodo de clase que genera N Contadors """
	@classmethod
	def crea_lista_Contadores(cls,N):
		contadores = []
		for i in range(N):
			t_n = Contador()
			contadores.append(t_n)
		return contadores
	
	class EstadoContador(enum.Enum):
		contar = 0
		esperar = 1

	## MÉTODOS DE INSTANCIA
	
	## EFE: crea una Contador inicializada al azar.
	def __init__(self):
		self.id = Contador.id
		Contador.id += 1
		self.velocidad = 6
		self.posicion = np.random.randint(0, 1499), np.random.randint(0, 1499) ## OJO: así se crea un par ordenado, un tuple de dos valores
		self.estado = Contador.EstadoContador.contar
		#variables del C
		self.avanza_derecha = True
		self.contando = True
		self.contador_tics = 0
		self.tiempo_muestreo = 0
		return
	
	## EFE: retorna una hilera en formato JSON que representa a la Contador
	def toJSON(self):
		# (type(self).__name__ retorna como hilera el nombre de la clase de self
		# se le pasa un tuple con el nombre de la clase y los valores de los atributos de self
		return json.dumps((type(self).__name__, self.id, self.velocidad, self.posicion))
	
	def obt_id(self):
		return self.id
		
	def obt_velocidad(self):
		return self.velocidad
	
	def obt_posicion(self):
		return self.posicion
		
	def asg_velocidad(self, vn):
		self.velocidad = vn
		return
		
	def obt_estado(self):
		return self.estado
		
	def asg_posicion(self, pn):
		self.posicion = pn
		return
	## EFE: avanza la Contador de acuerdo con su estado
	def avanzar(self):
		self.estado = self.EstadoContador.contar
		if(self.avanza_derecha):
			pos_x = self.posicion[0]
			pos_y = self.posicion[1]
			pos_x += self.velocidad
			self.posicion = pos_x, pos_y
		else:
			pos_x = self.posicion[0]
			pos_y = self.posicion[1]
			pos_x -= self.velocidad
			self.posicion = pos_x, pos_y
		return
	
	def cambiar_estado(self):
		self.contador_tics = 0
		self.contando = not self.contando
		if (self.contando):
			self.estado = self.EstadoContador.contar
		else:
			self.estado = self.EstadoContador.esperar
		return

		
	def asg_tiempo_muestreo(self, t_m):
		self.tiempo_muestreo = t_m
		
		
	def obt_tiempo_muestreo(self):
		return self.tiempo_muestreo
		
		
	def aumentar_tic(self):
		self.contador_tics += 1
		
		
	def obt_contador_tics(self):
		return self.contador_tics
		
	def obt_avanza_derecha(self):
		return self.avanza_derecha
		
	def avanzar_derecha(self):
		self.avanza_derecha = True
		
	def avanzar_izquierda(self):
		self.avanza_derecha = False
		
