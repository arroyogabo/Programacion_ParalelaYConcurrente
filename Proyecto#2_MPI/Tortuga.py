#!/usr/bin/python3
import enum
import numpy as np	# para generar números al azar según distribución estándar
import json			# para crear una hilera json en toJSON()
import random
from mpi4py import MPI
class Tortuga:
	"""
	Representa una tortuga con id, velocidad y posicion.
	"""
	
	## VARIABLES DE CLASE
	id = 0 		## OJO variable static de clase para generar ids de tortugas
	
	## MÉTODOS DE CLASE
	""" metodo de clase que genera N tortugas """
	@classmethod
	def crear_lista_tortugas(cls,N): #cls es la clase.
		comm = MPI.COMM_WORLD
		proc_count = comm.size
		tortugas = []
		n = N // proc_count
		for i in range(n):
			t_n = Tortuga()
			tortugas.append(t_n)
		return tortugas
	
	class EstadoTortuga(enum.Enum):
		vagar = 0
		camar = 1
		excavar = 2
		poner = 3
		tapar = 4
		camuflar = 5
		inactiva = 6
		
	## MÉTODOS DE INSTANCIA
	
	## EFE: crea una tortuga inicializada al azar.
	def __init__(self):
		self.id = Tortuga.id
		Tortuga.id += 1
		self.velocidad = np.random.normal(1.0, 0.5) ## promedio = 1.0 y desviación estándar = 0.5
		self.posicion = random.randint(0, 1499), random.randint(0, 1499) ## OJO: así se crea un par ordenado, un tuple de dos valores
		self.estado = Tortuga.EstadoTortuga.vagar
		self.pos_anidacion = 0, 0
		#variables de la version de C
		self.tic_actual = 0
		self.tic_salida = 0
		self.salio = False
		self.tic_Cambio_Estado = 0 
		self.contada = False
		self.contada_c = False 	#Si se conto en cuadrantes
		self.contada_tv = False	#Si se conto en transecto vertical
		self.contada_tpb = False
		self.tiempos_estados = np.zeros(5)
		return
	
	## EFE: retorna una hilera en formato JSON que representa a la tortuga
	def toJSON(self):
		# (type(self).__name__ retorna como hilera el nombre de la clase de self
		# se le pasa un tuple con el nombre de la clase y los valores de los atributos de self
		return json.dumps((type(self).__name__, self.id, self.velocidad, self.posicion, self.pos_anidacion))
	
	def obt_contada(self):
		return self.contada

	def obt_id(self):
		return self.id
	
	def obt_estado(self):
		return self.estado

	def obt_velocidad(self):
		return self.velocidad
	
	def obt_posicion(self):
		return self.posicion
		
	def obt_posicion_anidacion(self):
		return self.pos_anidacion
		
	def asg_velocidad(self, vn):
		self.velocidad = vn
		return
		
	def asg_posicion(self, pn):
		self.posicion = pn
		return
	
	def asg_pos_anidacion(self, pa):
		self.pos_anidacion = pa
		return
	
	def asg_tic_cambio(self, cantidad_tic):
		self.tic_Cambio_Estado = cantidad_tic
		return

	def asg_contada(self):
		self.contada = True
		return
	
	def avanzar_estado(self, es):
		if (es == self.EstadoTortuga.vagar):		
			self.estado = self.EstadoTortuga.camar
		elif (es == self.EstadoTortuga.camar):
			self.estado = self.EstadoTortuga.excavar
		elif (es == self.EstadoTortuga.excavar):
			self.estado = self.EstadoTortuga.poner
		elif (es == self.EstadoTortuga.poner):
			self.estado = self.EstadoTortuga.tapar
		elif (es == self.EstadoTortuga.tapar):
			self.estado = self.EstadoTortuga.camuflar
		elif (es == self.EstadoTortuga.camuflar):
			self.estado = self.EstadoTortuga.inactiva
		return
		
		
	## EFE: avanza la tortuga de acuerdo con su estado
	def avanzar(self):
		if( self.posicion[1] >= self.pos_anidacion[1] ):
			pos_x = self.posicion[0]
			pos_y = self.pos_anidacion[1]
			self.posicion = pos_x, pos_y
		else:
			pos_x = self.posicion[0]
			pos_y = self.posicion[1]
			pos_y += self.velocidad
			pos_y = int(pos_y)
			self.posicion = pos_x, pos_y
		return
	
	def desactivarse (self, proba):
    	
		azar = np.random.uniform(0.0,1.0)
		desactivado = False
		if (azar <= proba):
			desactivado = True
		return desactivado
	
	def cambiar_estado (self,proba):
		if(self.estado == 0):#vagando
			if(self.desactivarse(proba)== False):
				self.avanzar_estado(self.estado)
				self.tic_Cambio_Estado = self.tiempos_estados[0] #Pos estado - 1 contiene su respectivo tiempo.
			else:
				self.estado = self.EstadoTortuga.inactiva
		else:
			if(self.tic_actual == self.tic_Cambio_Estado): #tic cambio de estado #########Falta asignar
				if(self.desactivarse(proba)== False and not self.estado == Tortuga.EstadoTortuga.camuflar): #cambia de estado
					self.avanzar_estado(self.estado)
					estado_actual = self.estado_a_int(self.estado)
					self.tic_Cambio_Estado = self.tiempos_estados[estado_actual - 1]
					self.tic_actual=0 #reinicia tic
				else:
					self.estado = self.EstadoTortuga.inactiva
			else: #avanza un tic
				self.tic_actual +=1
		return
		
		
	def asg_tic_salida(self, tic):
		self.tic_salida = tic
		
	
	def obt_tic_salida(self):
		return self.tic_salida
		
	
	def _salio(self):
		self.salio = True
		
	
	def obt_salio(self):
		return self.salio
		
	def asg_tiempos_estado(self, tiempos):
		self.tiempos_estados = tiempos
		
	def obt_tiempos_estado(self):
		return self.tiempos_estados
		
	def obt_contada_en_c(self):
		return self.contada_c
		
	def contar_en_c(self):	
		self.contada_c = True
		
	def obt_contada_en_tv(self):
		return self.contada_tv
		
	def contar_en_tv(self):
		self.contada_tv = True
		
	def contar_en_tpb(self):
		self.contada_tpb = True
	
	def obt_contada_en_tpb(self):
		return self.contada_tpb
		
	def estado_a_int(self, estado):
		n = -1
		if estado == Tortuga.EstadoTortuga.vagar:
			n = 0
		elif estado == Tortuga.EstadoTortuga.camar:
			n = 1
		elif estado == Tortuga.EstadoTortuga.excavar:
			n = 2
		elif estado == Tortuga.EstadoTortuga.poner:
			n = 3	
		elif estado == Tortuga.EstadoTortuga.tapar:
			n = 4
		elif estado == Tortuga.EstadoTortuga.camuflar:
			n = 5
		elif estado == Tortuga.EstadoTortuga.inactiva:
			n = 6
		return n
