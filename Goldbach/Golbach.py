from mpi4py import MPI

###########Ahora si Goldbach###############
def Goldbach (i,lista_primos, lista_primos_cero):
	for j in reversed(range(len(lista_primos))):
		for k in range(len(lista_primos)):
			for l in range(len(lista_primos_cero)):
				if ( lista_primos [j] + lista_primos [k] + lista_primos_cero [l] == i):
					if (lista_primos_cero [l] ==0):
						a = str(i) + " = " + str(lista_primos [j]) + " + " + str(lista_primos [k])
						return a
					else:
						a = str(i) + " = " + str(lista_primos [j]) + " + " + str(lista_primos [k])+" + "+str(lista_primos_cero [l])
						return a

##########################################




def main ():

	comm = MPI.COMM_WORLD # Obtiene acceso al "comunicador"
	pid = comm.rank
	
	n= 100
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
	lista_primos_cero += lista_primos # para que puedan haber sumas de 2 primos

	cantidad_hilos = comm.size
	num_por_hilo = n/cantidad_hilos
	
	for i in range (5,n):
		num_text=Goldbach(i, lista_primos, lista_primos_cero)
		print(num_text)

main()