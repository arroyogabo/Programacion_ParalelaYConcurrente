#!/usr/bin/python3

import sys
import csv
from enum import Enum
from mpi4py import MPI

from Tortuga import Tortuga
from Contador import Contador
from Simulador import Simulador
## Para lectura de archivos de texto en python ver:
## https://www.pythonforbeginners.com/files/reading-and-writing-files-in-python

## Para leer archivos csv con numeros con python ver:
## https://pythonprogramming.net/reading-csv-files-python-3/

## Para manejo de excepciones en python ver:
## https://docs.python.org/3/library/exceptions.html

## Tipo enumerado para los dos tipos de numeros que se pueden leer
class Tipos_numeros(Enum):
	int = 0
	float = 1
	
## EFE: lee un archivo csv con numeros separados por coma y retorna una lista 
## de listas de numeros de tipo tn.
def lee_numeros_csv(archivo,tn):
	lista = []
	read_csv = csv.reader(archivo, delimiter=',')
	for row in read_csv:
		sublista = []
		for n in row:
			try:
				if (tn == Tipos_numeros.int):
					sublista.append(int(n))
				else: 
					sublista.append(float(n))
			except ValueError as error_de_valor:
				print("Error de tipo de valor: {0}".format(error_de_valor)) #{0} es obligatorio ponerlo.
		lista.append(sublista)
	return lista

def read_data(file_path, tn):	
	data = []
	try:
		with open(file_path) as file: #with se utiliza normalmente cuando se quiere abrir un archivo, generalmente de texto.
			data = lee_numeros_csv(file, tn)
	except OSError as oserror:
		print("Error de entrada-salida de archivos: {0}".format(oserror))
	return data
	
	
def main():
	
	comm = MPI.COMM_WORLD
	pid = comm.rank
	proc_count = comm.size
	
	global_conteo_tv = 0
	global_conteo_c = 0
	
	dts_c = read_data("archivos\cuadrantes.csv", Tipos_numeros.int)		# datos de los cuadrantes.
	dts_ct = read_data("archivos\comportamiento_tortugas.csv", Tipos_numeros.float)		# datos del comportamiento de tortugas.
	dts_tv = read_data(r"archivos\transectos_verticales.csv", Tipos_numeros.int)
	dts_tpb = read_data(r"archivos\transecto_paralelo_berma.csv", Tipos_numeros.int)
	dts_exp = read_data("archivos\experimentos.csv", Tipos_numeros.int) 	# datos de los experimentos.
	dts_marea = read_data("archivos\marea.csv", Tipos_numeros.float)
	dts_terreno = read_data(r"archivos\terreno.csv", Tipos_numeros.float)
	
	# Traspaso de datos a la clase simulador.
	
	Simulador.inicializar_playa(dts_terreno)
	Simulador.inicializar_marea(dts_marea, 0)
	Simulador.inicializar_transecto_berma(dts_tpb)
	Simulador.inicializar_transectos_verticales(dts_tv)
	Simulador.inicializar_cuadrantes(dts_c)
	Simulador.inicializar_arribada(dts_ct, 7000) #dts_exp[0][2]
	
	
	conteo_tv, conteo_c = Simulador.simular(372)
	global_conteo_tv = comm.reduce(conteo_tv, op=MPI.SUM)
	global_conteo_c = comm.reduce(conteo_c, op=MPI.SUM)
	
	if pid == 0:
	
		##ESTIMACION TV
		estimacion_tv = 0.0
		area_observacion = 0.0
		long_tv = 0.0
		total_tics = 372.0
		w = 2.0
		m = total_tics // (dts_tv[0][1]*2.0) #El denominador es el tiempo necesario para un muestreo. 15*2(conteo 15 y esperando 15) 
		pt = 64.8	#tiempo que duran las tortugas en el transecto.
		for i in range(len(dts_terreno)):
			area_observacion = area_observacion + (dts_terreno[i][3]*100)
			long_tv = long_tv + ((dts_terreno[i][3]))
			
		estimacion_tv = (area_observacion*total_tics)
		estimacion_tv = estimacion_tv / (2.0*w*m*long_tv)
		estimacion_tv = estimacion_tv * (global_conteo_tv/pt)
		
		##ESTIMACION CUADRANTES
		area_observacion = 0.0
		for i in range(len(dts_terreno)):
			area_observacion = area_observacion + (dts_terreno[i][3])
		estimacion_c = 0.0
		area_cuadrantes = 10.0 * 10.0
		m = total_tics // (dts_ct[0][1]*2.0)
		estimacion_c = global_conteo_c * 1.25 
		estimacion_c = estimacion_c * (area_cuadrantes/area_observacion) 
		estimacion_c = estimacion_c * total_tics
		estimacion_c = estimacion_c / (1.08 * m)
		print(estimacion_c)
		print(estimacion_tv)
	## OJO: el archivo esta en una carpeta "archivos"
	
main()
