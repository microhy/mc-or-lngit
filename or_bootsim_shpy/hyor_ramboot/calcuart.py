
INCLK = 50*10**6
RATE  = 115200
fdiv = INCLK/(16*RATE) + 0.5
div  = int(fdiv)
