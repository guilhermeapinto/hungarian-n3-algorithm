# UNIVESP - 2019
# Autor: Guilherme A. Pinto (guilherme.pinto@gmail.com)

CPPFLAGS=-std=gnu++14 -Wall -O2

all: hungarian.exe

hungarian.exe: hungarian.cpp
	g++ $(CPPFLAGS) -o hungarian.exe hungarian.cpp

touch:
	touch *.cpp

clean:
	rm -f *~ *.aux *.log *.o
