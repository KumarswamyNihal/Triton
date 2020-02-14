CXXFLAGS += --std=c++17 -lpthread -DASIO_STANDALONE
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

default: main

all: main 

debug: CXXFLAGS += -g
debug: default

main: main.o mainwin.o dialogs.o chat_client.o controller.o packets.o logger.o *.h
	${CXX} ${CXXFLAGS} -o main main.o mainwin.o dialogs.o chat_client.o controller.o logger.o packets.o $(GTKFLAGS)
main.o: main.cpp *.h
	${CXX} ${CXXFLAGS} -c main.cpp $(GTKFLAGS)
controller.o: controller.cpp *.h
	${CXX} ${CXXFLAGS} -c controller.cpp ${GTKFLAGS}
logger.o: logger.cpp *.h
	${CXX} ${CXXFLAGS} -c logger.cpp 
chat_client.o: chat_client.cpp *.h
	${CXX} ${CXXFLAGS} -c chat_client.cpp 
mainwin.o: mainwin.cpp *.h
	${CXX} ${CXXFLAGS} -c mainwin.cpp $(GTKFLAGS)
dialogs.o: dialogs.cpp *.h
	${CXX} ${CXXFLAGS} -c dialogs.cpp $(GTKFLAGS)
packets.o: packets.cpp *.h 
	${CXX} ${CXXFLAGS} -c packets.cpp 
clean:
	-rm -f *.gch *.o main 
