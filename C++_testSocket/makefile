CC = g++
CFLAGS = -std=c++17 -Wall -g
EXEC_NAME = TestServer
INCLUDES = 
LIBS = -pthread
OBJ_FILES = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
INSTALL_DIR = /usr/bin

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

install :
	cp $(EXEC_NAME) $(INSTALL_DIR)gm