SRC=platcomm.cpp tcplistener.cpp webSrvCmdParser.cpp main.cpp
TARGET=webResSrv

start:
	g++ $(SRC) -o $(TARGET) -lpthread

arm-compile:
	arm-linux-g++ $(SRC) -o $(TARGET) -lpthread

clean:
	rm -fr $(TARGET)
