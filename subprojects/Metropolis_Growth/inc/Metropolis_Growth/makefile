CC = g++
CFLAGS = -Wall -g -Wno-unused-function
<<<<<<< HEAD:subprojects/Metropolis_Growth/inc/Metropolis_Growth/makefile
PNGWRITE = -L/usr/local/lib -I/usr/local/include -lpng -lpngwriter -lz -lfreetype -DNO_FREETYPE
=======
PNGWRITE = -I/usr/local/lib -L/usr/local/include -lpng -lpngwriter -lz -DNO_FREETYPE
>>>>>>> parent of ada7b97... Finally fixed linking... and now I just need to build montecarlo.h into a library in order to allow for separate compilation of the front end and back end. Look into cmake.:Metropolis_Growth/makefile

main: site.o lattice.o interaction.o montecarlo.o filehandler.o main.cpp
	${CC} ${CFLAGS} main.cpp site.o lattice.o interaction.o montecarlo.o filehandler.o -o main.exe


site.o: site.h site.cpp
	${CC} ${CFLAGS} -c site.cpp 

lattice.o: filehandler.h site.h lattice.h lattice.cpp
	${CC} ${CFLAGS} -c lattice.cpp${PNGWRITE}

interaction.o: lattice.h site.h interaction.h interaction.cpp 
	${CC} ${CFLAGS} -c interaction.cpp ${PNGWRITE}

montecarlo.o: interaction.h lattice.h filehandler.h montecarlo.h montecarlo.cpp
	${CC} ${CFLAGS} -c montecarlo.cpp ${PNGWRITE}

filehandler.o: montecarlo.h lattice.h filehandler.h filehandler.cpp
	${CC} ${CFLAGS} -c filehandler.cpp ${PNGWRITE}

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