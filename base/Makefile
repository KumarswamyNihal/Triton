CXXFLAGS += --std=c++17
CXXFLAGS += -DASIO_STANDALONE
ASIOFLAGS += -lpthread 
GTKFLAGS += `pkg-config --cflags --libs gtk+-3.0`
default: main
all: main chat_client logger packets global

main: main.o chat_client.o logger.o packets.o global.o *.h *.hpp
	${CXX} ${CXXFLAGS} -o base main.o chat_client.o logger.o packets.o global.o ${ASIOFLAGS} ${GTKFLAGS}

main.o: main.cpp *.h *.hpp
	${CXX} ${CXXFLAGS} -c main.cpp ${ASIOFLAGS} ${GTKFLAGS}

chat_client.o: chat_client.cpp *.h *.hpp
	${CXX} ${CXXFLAGS} -c chat_client.cpp ${ASIOFLAGS} ${GTKFLAGS}

logger.o: logger.cpp *.h *.hpp
	${CXX} ${CXXFLAGS} -c logger.cpp 

packets.o: packets.cpp *.h *.hpp
	${CXX} ${CXXFLAGS} -c packets.cpp

global.o: global.cpp *.h *.hpp
	${CXX} ${CXXFLAGS} -c global.cpp ${GTKFLAGS}

clean:
	-rm -f *.gch *.o pi a.out 
