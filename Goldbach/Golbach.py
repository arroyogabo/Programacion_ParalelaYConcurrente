from mpi4py import MPI
from bisect import bisect_left
import numpy as np

###########Ahora si Goldbach###############
def Goldbach (i,lista_primos):
	x,y,z =0,0,0
	pos= bisect_left (lista_primos, i)
	for j in reversed(range(pos)):
		x=lista_primos [j] 
		for k in range(pos):
			y= lista_primos [k]
			if( x + y ==i):
				a = str(i) + " = " + str(x) + " + " + str(y)
				return a
			for l in range(pos):
				z = lista_primos [l]
				if ( x + y + z == i):
					a = str(i) + " = " + str(x) + " + " + str(y)+" + "+str(z)
					return a
##########################################


def main ():
	comm = MPI.COMM_WORLD # Obtiene acceso al "comunicador"
	pid = comm.rank
	global_vec=[]
	n= 1000
	lista_primos = [2,3,5]

	############Calcula Primos#############
	for i in range (6,n):
		is_primo= True
		for j in lista_primos:
			if(i%j == 0):
				is_primo = False
		if(is_primo==True):
			lista_primos.append(i)
	##########################################

	lista_primos_cero = [0]

	cantidad_hilos = comm.size
	num_por_hilo = n/cantidad_hilos
	
	
	inicio= int(num_por_hilo)*pid+5
	final= int (num_por_hilo)*pid+int(num_por_hilo)
	
	comm.Barrier()
	time= MPI.Wtime();
	vector=[]
	#################################linea a paralelizar##############################################
	for i in range (inicio,final):
	###################################################################################################
		num_text=Goldbach(i, lista_primos)
		vector.append(num_text)
	comm.Barrier()
	time = MPI.Wtime()-time;	
	
	global_vec= comm.gather(vector,0)
	comm.Barrier()
	if (pid==0):
		for i in range (len(global_vec)):
			for j in range (len(vector)):
				print(global_vec[i][j])
		print ("Tiempo: ",time)
main()