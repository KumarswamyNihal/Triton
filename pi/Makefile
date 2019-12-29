CXXFLAGS += --std=c++17	
CXXFLAGS += -DASIO_STANDALONE
ASIOFLAGS += -lpthread 

default: main
all: main chat_client

main: main.o chat_client.o *.h
	${CXX} ${CXXFLAGS} -o main main.o chat_client.o ${ASIOFLAGS}

main.o: main.cpp *.h
	${CXX} ${CXXFLAGS} -c main.cpp ${ASIOFLAGS}

chat_client.o: chat_client.cpp *.h
	${CXX} ${CXXFLAGS} -c chat_client.cpp ${ASIOFLAGS}

clean:
	-rm -f *.gch *.o main 