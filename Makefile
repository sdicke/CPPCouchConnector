CC		= g++
STANDARD	= -std=c++20
ERROPTS		= -Wall -Wextra -Werror 
ADDOPTS		= -O2 -pthread -fstack-protector-strong -fPIC
INC		= -I external/jsoncons/include/ 
INC2		= -I external/stduuid/include/
FLAGS		= $(STANDARD) $(ERROPTS) $(ADDOPTS) $(INC)
LIBS		= -l cpr -l curl
CALL		= $(CC) $(FLAGS)
TARGET		= /usr/local
INCDIR		= $(TARGET)/include
INCTARGET	= $(INCDIR)/cppcouchconnector
LIBTARGET	= $(TARGET)/lib64
BINARY		= libcppcouchconnector.so

all: cppcouchconnector

cppcouchconnector: Document.o Database.o Network.o Server.o DataBaseConnection.o
		$(CC) $(LIBS) -shared *.o -o $(BINARY)

Database.o: Database.hpp Database.cpp
		$(CALL) $(INC2) -c Database.cpp -o Database.o

DataBaseConnection.o: DataBaseConnection.hpp DataBaseConnection.cpp
		$(CALL) -c DataBaseConnection.cpp -o DataBaseConnection.o

Document.o: Document.hpp Document.cpp
		$(CALL) -c Document.cpp -o Document.o

Network.o: Network.hpp Network.cpp
		$(CALL) -c Network.cpp -o Network.o

Server.o: Server.hpp Server.cpp
		$(CALL) -c Server.cpp -o Server.o

install:
		mkdir -p $(INCTARGET)
		cp -f *.hpp $(INCTARGET)
		cp -f $(BINARY) $(LIBTARGET)

clean:
		rm *.o $(BINARY)
