CXXFLAGS += --std=c++17	
CXXFLAGS += -DASIO_STANDALONE
ASIOFLAGS += -lpthread 

default: main
all: main chat_client logger

main: main.o chat_client.o logger.o *.h
	${CXX} ${CXXFLAGS} -o main main.o chat_client.o logger.o ${ASIOFLAGS}

main.o: main.cpp *.h
	${CXX} ${CXXFLAGS} -c main.cpp ${ASIOFLAGS}

chat_client.o: chat_client.cpp *.h
	${CXX} ${CXXFLAGS} -c chat_client.cpp ${ASIOFLAGS}

logger.o: logger.cpp *.h
	${CXX} ${CXXFLAGS} -c logger.cpp 

clean:
	-rm -f *.gch *.o main 