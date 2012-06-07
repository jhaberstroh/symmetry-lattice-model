Site.o: Site.h Site.cpp
	g++ -Wall -g -c Site.cpp -o Site.o

Lattice.o: Lattice.h Lattice.cpp Site.o
	g++ -Wall -g -c Lattice.cpp -o Lattice.o

test: Site.o testSite.cpp
	g++ -Wall -g testSite.cpp Site.o -o test.exe

latTest: Site.o Lattice.o latTest.cpp
	g++ -Wall -g latTest.cpp Site.o Lattice.o -o latTest.exe

clean:
	rm *~ \#*\# *.o