from mpi4py import MPI
import numpy as np
import random
from math import log2
import sys, getopt

def obt_valores_linea_comandos(argv):
	cnt_data = 0
	
	try:
		opts, args = getopt.getopt(argv,"h:d:",["cnt_datos="])
	except getopt.GetoptError:
		print('merge_mpi.py -d <valor int cantidad de datos a generar>')
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print('merge_mpi.py -d <valor int cantidad de datos a generar>')
			sys.exit()
		elif opt in ("-d", "--d"):
			cnt_data = arg
	return int(cnt_data)


def gen_data(size):
	comm = MPI.COMM_WORLD
	num_procs = comm.size
	
	local_size = size // num_procs 
	
	num_list = np.zeros(local_size, dtype=int)
	random.seed()
	for i in range(0,local_size):
		num = random.uniform(0,101)  #Numeros entre 0 y 100.
		num_list[i] = num
	return num_list

def merge(v1, v2):
	size = len(v1) + len(v2)
	list = np.zeros(size, dtype=int)
	list = np.concatenate((v1, v2))
	list.sort()
	return list
	
def main(argv):
	comm = MPI.COMM_WORLD
	pid = comm.rank
	num_procs = comm.size
	size = 0
	wtime = 0.0
	
	if pid == 0:
		size = obt_valores_linea_comandos(argv)
	
	comm.barrier()
	wtime = MPI.Wtime()
	size = comm.bcast(size,0)
	l_l = np.array([]) #Lista de listas numpy
	num_list = gen_data(size) #Lista con size elementos generados aleatoriamente.
	
	num_list.sort() #Primer ordenamiento por proceso
	
	l_l = comm.allgather(num_list) #Todos los procesos ahora tiene una lista de listas de numeros ordenados.
	l_l_size = len(l_l)

	if pid%2 != 0 and pid < l_l_size:  #Los procesos cuyo id sea impar, se encargaran de hacer las mezclas.
		if pid == 1:
			num_list = merge(l_l[pid], l_l[pid-1])
				
	l_l = comm.allgather(num_list)
	
	for i in range(0, int(log2(num_procs))-1):	
		l_l_size /= 2
		
		if pid%2 != 0 and pid < l_l_size:  #Los procesos cuyo id sea impar, se encargaran de hacer las mezclas.
			if pid == 1:
				num_list = merge(l_l[pid], l_l[pid+2])
			else:
				pos1 = pid + 2
				pos2 = pos1 + 2
				num_list = merge(l_l[pid], l_l[pid+2])
			
		l_l = comm.allgather(num_list)	 
	
	comm.barrier()
	wtime = MPI.Wtime() - wtime
	if pid == 0:
		if num_procs != 1:
			print("Arreglo resultante: ", l_l[1])
		
			print("En un tiempo de duracion de: ", wtime, " segundos.")
		else:
			print("Arreglo resultante: ", l_l)
		
			print("En un tiempo de duracion de: ", wtime, " segundos.")
	
if __name__ == "__main__":
	main(sys.argv[1:])
