Libs     := -lsnap7 
Wrapper  :=snap7.cpp

CXX      := g++
CC       := gcc
CXXFLAGS :=-O3
CFLAGS   :=

.PHONY: all clean

all: 
	$(CXX) $(CXXFLAGS) -o client client.cpp $(Wrapper) $(Libs) 
clean:
	$(RM) client
