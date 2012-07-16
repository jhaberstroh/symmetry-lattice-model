CC = g++
CFLAGS = -Wall -g -Wno-unused-function 

main: site.o lattice.o interaction.o montecarlo.o filehandler.o main.cpp
	${CC} ${CFLAGS} main.cpp site.o lattice.o interaction.o montecarlo.o filehandler.o -o main.exe	


site.o: site.h site.cpp
	${CC} ${CFLAGS} -c site.cpp 

lattice.o: lattice.h lattice.cpp site.h
	${CC} ${CFLAGS} -c lattice.cpp

interaction.o: interaction.h interaction.cpp lattice.h site.h
	${CC} ${CFLAGS} -c interaction.cpp

montecarlo.o: montecarlo.h montecarlo.cpp interaction.h lattice.h filehandler.h
	${CC} ${CFLAGS} -c montecarlo.cpp

filehandler.o: montecarlo.h filehandler.h filehandler.cpp
	${CC} ${CFLAGS} -c filehandler.cpp

tester.o: tester.cpp
	${CC} ${CFLAGS} -c tester.cpp

tester: tester.o site.o lattice.o interaction.o montecarlo.o
	${CC} ${CFLAGS} tester.o site.o lattice.o interaction.o montecarlo.o -o tester.exe

version: version.h version.cpp
	${CC} ${CFLAGS} version.cpp -o versionupdate.exe

anneal: Site.o Lattice.o anneal.cpp
	g++ -Wall -g -O3 anneal.cpp Site.o Lattice.o -o anneal.exe

clean:
	rm *~ \#*\# *.o