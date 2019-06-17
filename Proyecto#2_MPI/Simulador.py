#!/usr/bin/python3
import enum
import numpy as np	# para generar números al azar según distribución estándar
from Tortuga import Tortuga
class Simulador:
	"""
	Representa al simulador de la arribada.
	"""
	
	## VARIABLES DE CLASE
	sectores_playa = []
	marea = []
	comportamiento_tortugas = []
	tortugas = []
	transecto_berma = []
	transectos_verticales = []
	cuadrantes = []
	tics = 0			## cantidad total de tics a simular
	tic = 0				## tic actual
	conteo_tpb = 0 		## variable para el conteo basado en transecto paralelo
	conteo_tsv = 0		## variable para el conteo basado en transectos verticales
	conteo_cs = 0		## variable para el conteo basado en cuadrantes
	
	
	## MÉTODOS DE CLASE
	
	## EFE:Inicializa los sectores de playa con sp. 
	@classmethod
	def inicializar_playa(cls, sp):
		cls.sectores_playa = sp
		return
	
	## EFE: Inicializa los datos de la marea con la posición i de la lista mareas.
	@classmethod
	def inicializar_marea(cls, mareas, i): #solo vamos a usar la primera. i=0
		cls.marea = mareas[i]
		return
		
	## EFE: Inicializa la arribada con el comportamiento de las tortugas y la cantidad 
	## indicada por nt de tortugas a simular.
	@classmethod
	def inicializar_arribada(cls, comportamiento, nt):
	
		cls.comportamiento_tortugas = comportamiento
		marea = cls.marea[0]
		pendiente = (cls.marea[1] - cls.marea[0]) / cls.marea[2] 
		velocidad_promedio = cls.comportamiento_tortugas[0][6]
		desv_velocidad = cls.comportamiento_tortugas[0][7]
		tortugas = Tortuga.crear_lista_tortugas(nt) # falta el comportamiento
		
		for tortuga in tortugas:
			# CALCULO PARA LA POSICION INICIAL DE LA TORTUGA, SALIDA.
			tic_salida = np.random.logistic(0, cls.comportamiento_tortugas[0][8]) 
			tic_salida += 372 / 2   #Para ajustar la distribucion logistica.
		
			pos_x_tortuga = np.random.uniform(0, cls.transecto_berma[1][1])
			pos_y_salida = marea + (pendiente * tic_salida)
			pos_salida = pos_x_tortuga, pos_y_salida
		
			velocidad = np.random.normal(velocidad_promedio, desv_velocidad)
			
			# CALCULO DE LA POSICION Y FINAL DE LA TORTUGA (ANIDAMIENTO)
			proba = np.random.uniform(0,1) #Probabilidad de anidamiento en los diferentes sectores de la playa
			if(proba <= cls.comportamiento_tortugas[2][0]):
				i = Simulador.pos_playa(pos_x_tortuga)
				pos_y_anidacion = np.random.uniform(cls.sectores_playa[i][1]-10, cls.sectores_playa[i][1])
				
			elif (proba <= cls.comportamiento_tortugas[2][1]+cls.comportamiento_tortugas[2][0]):
				i = Simulador.pos_playa(pos_x_tortuga)
				pos_y_anidacion = np.random.uniform(cls.sectores_playa[i][1], cls.sectores_playa[i][1]+10)
				
			elif (proba <= cls.comportamiento_tortugas[2][2]+cls.comportamiento_tortugas[2][1]+cls.comportamiento_tortugas[2][0]):
				i = Simulador.pos_playa(pos_x_tortuga)
				pos_y_anidacion = np.random.uniform(cls.sectores_playa[i][1]+11, cls.sectores_playa[i][1]+20)
				
			else:
				i = Simulador.pos_playa(pos_x_tortuga)
				pos_y_anidacion = np.random.uniform(cls.sectores_playa[i][1]+21, cls.sectores_playa[i][1]+30)
				
			pos_anidacion = pos_x_tortuga, pos_y_anidacion
			
			#Asignaciones a cada tortuga.
			tortuga.asg_velocidad(velocidad)
			tortuga.asg_posicion(pos_salida)
			tortuga.asg_pos_anidacion(pos_anidacion)
			
			print(tortuga.toJSON())
			
		return

	## EFE: Inicializa el transecto paralelo a la berma.
	@classmethod
	def inicializar_transecto_berma(cls, tb):
		cls.transecto_berma = tb
		return
	
	## EFE: Inicializa los transectos verticales.
	@classmethod
	def inicializar_transectos_verticales(cls, tsv):
		cls.transectos_verticales = tsv
		return
	
	## EFE: Inicializa los cuadrantes.
	@classmethod
	def inicializar_cuadrantes(cls, cs):
		cls.cuadrantes = cs
		return	
	
	@classmethod
	def simular(cls, total_tics):
		cls.tics = total_tics
		marea_media =  (cls.marea[1] + cls.marea[0]) / 2.0
		pendiente = (cls.marea[1] - cls.marea[0]) / cls.marea[2] 
		marea_actual = cls.marea[0]
		
		while(cls.tic < cls.tics): #Tic actual menor al total
			marea_actual += pendiente	#Aumento de la marea por tic
			
			cls.tic += 1
		
		return
	@classmethod
	def pos_playa(self, pos_x_tortuga):
		indice = -1
		if(pos_x_tortuga <= 100):
			indice = 0
		elif(pos_x_tortuga <= 200):
			indice = 1
		elif(pos_x_tortuga <= 300):
			indice = 2
		elif(pos_x_tortuga <= 400):
			indice = 3
		elif(pos_x_tortuga <= 500):
			indice = 4
		elif(pos_x_tortuga <= 600):
			indice = 5
		elif(pos_x_tortuga <= 700):
			indice = 6
		elif(pos_x_tortuga <= 800):
			indice = 7
		elif(pos_x_tortuga <= 900):
			indice = 8
		elif(pos_x_tortuga <= 1000):
			indice = 9
		elif(pos_x_tortuga <= 1100):
			indice = 10
		elif(pos_x_tortuga <= 1200):
			indice = 11
		elif(pos_x_tortuga <= 1300):
			indice = 12
		elif(pos_x_tortuga <= 1400):
			indice = 13
		elif(pos_x_tortuga <= 1500):
			indice = 14
			
		return indice
	
	## DE ESTA CLASE SIMULADOR SÓLO EXISTIRÍA UNA INSTANCIA (SINGLETON).
	## POR LO QUE NO SE INCLUYEN MÉTODOS DE INSTANCIA, SÓLO MÉTODOS DE CLASE.

		
	
