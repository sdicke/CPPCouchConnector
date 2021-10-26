CC		= g++
STANDARD	= -std=c++20
ERROPTS		= -Wall -Wextra -Werror 
ADDOPTS		= -O2 -pthread -fstack-protector-strong -fPIC
FLAGS		= $(STANDARD) $(ERROPTS) $(ADDOPTS)
LIBS		= -l cpr -l curl
CALL		= $(CC) $(FLAGS)
TARGET		= /usr/local
INCDIR		= $(TARGET)/include
INCTARGET	= $(INCDIR)/cppcouchconnector
LIBTARGET	= $(TARGET)/lib64

all: cppcouchconnector

cppcouchconnector: Document.o  Database.o Network.o Server.o DataBaseConnection.o
		$(CC) $(LIBS) -shared Document.o Database.o Network.o Server.o DataBaseConnection.o -o libcppcouchconnector.so

Database.o: Database.cpp
		$(CALL) -c Database.cpp -o Database.o

DataBaseConnection.o: DataBaseConnection.cpp
		$(CALL) -c DataBaseConnection.cpp -o DataBaseConnection.o

Document.o: Document.cpp
		$(CALL) -c Document.cpp -o Document.o

Network.o: Network.cpp
		$(CALL) -c Network.cpp -o Network.o

Server.o: Server.cpp
		$(CALL) -c Server.cpp -o Server.o

install:
		mkdir -p $(INCTARGET)
		cp -f *.hpp $(INCTARGET)
		cp -f libcppcouchconnector.so $(LIBTARGET)

clean:
		rm *.o libcppcouchconnector.so
