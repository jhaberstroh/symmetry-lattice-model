CC = g++
CFLAGS = -Wall -g -Wno-unused-function 

main: site.o lattice.o interaction.o monteCarlo.o main.cpp
	${CC} ${CFLAGS} main.cpp site.o lattice.o interaction.o monteCarlo.o -o main.exe	


site.o: site.h site.cpp
	${CC} ${CFLAGS} -c site.cpp 

lattice.o: lattice.h lattice.cpp site.h
	${CC} ${CFLAGS} -c lattice.cpp

interaction.o: interaction.h interaction.cpp lattice.h site.h
	${CC} ${CFLAGS} -c interaction.cpp

monteCarlo.o: monteCarlo.h monteCarlo.cpp interaction.h lattice.h
	${CC} ${CFLAGS} -c monteCarlo.cpp

tester.o: tester.cpp
	${CC} ${CFLAGS} -c tester.cpp

tester: tester.o site.o lattice.o interaction.o monteCarlo.o
	${CC} ${CFLAGS} tester.o site.o lattice.o interaction.o monteCarlo.o -o tester.exe


anneal: Site.o Lattice.o anneal.cpp
	g++ -Wall -g -O3 anneal.cpp Site.o Lattice.o -o anneal.exe

clean:
	rm *~ \#*\# *.o