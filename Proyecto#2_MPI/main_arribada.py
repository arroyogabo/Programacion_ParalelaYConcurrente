#!/usr/bin/python3

import sys
import csv
from enum import Enum

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
	Simulador.inicializar_arribada(dts_ct, 10) #dts_exp[0][2]
	
	
	Simulador.simular(372)
	print(Tortuga.desactivarse(Tortuga, 0.5))
	## OJO: el archivo esta en una carpeta "archivos"
	
main()

