from mpi4py import MPI
import random
import numpy as np
import sys, getopt 

def obt_valores_linea_comandos(argv):
	cnt_terminos = 0
	
	try:
		opts, args = getopt.getopt(argv,"h:t:",["cnt_terminos="])
	except getopt.GetoptError:
		print('pi_mpi.py -t <valor int cantidad de terminos>')
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print('pi_mpi.py -t <valor int cantidad de terminos>')
			sys.exit()
		elif opt in ("-t", "--t"):
			cnt_terminos = arg
	return int(cnt_terminos)

def main(argv):
	comm = MPI.COMM_WORLD
	pid = comm.rank
	num_p = comm.size
	cnt_terminos = 0
	global_number_in_circle = 0
	number_in_circle = 0
	pi = 0
	
	if pid == 0:
		cnt_terminos = obt_valores_linea_comandos(argv)
	comm.barrier()
	wtime = MPI.Wtime()
	cnt_terminos = comm.bcast(cnt_terminos, 0)
	i = int(cnt_terminos/num_p)
	for i in range(0, i):
		x = random.uniform(-1.0, 1.0)
		y = random.uniform(-1.0, 1.0)
		distance_squared = x*x + y*y
		if (distance_squared <= 1):
			number_in_circle += 1
			
	global_number_in_circle = comm.reduce(number_in_circle, op=MPI.SUM)
	wtime = MPI.Wtime() - wtime
	if pid == 0:
		if cnt_terminos != 0:
			pi = 4*global_number_in_circle/cnt_terminos
			print ("El valor de pi es: ", pi, "En un tiempo de: ", wtime, "segundos")
		else:
			print("Error: La cantidad de terminos es 0.")
		
	
	return

if __name__ == "__main__":
	main(sys.argv[1:])
	
	
