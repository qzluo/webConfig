CC = g++
ARMCC = arm-none-linux-gnueabi-g++
SRC = *.cpp  
Target = ip_com_gateway
CFLAG = -I"../include/" -I"../test/"

#################################################
## $(Target) : $(OBJ)
##	$(CC) $(OBJ) -o $(Target)
#################################################

$(Target) : $(SRC)
	$(CC) $(SRC) $(CFLAG) -o $(Target) -lpthread

armcompile:
	$(ARMCC) $(SRC) $(CFLAG) -o $(Target) -lpthread
	
install:
	cp $(Target) ../bin/

clean:
	rm $(Target)
