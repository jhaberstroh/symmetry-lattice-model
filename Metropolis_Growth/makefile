Site.o: Site.h Site.cpp
	g++ -Wall -g -c -O3 Site.cpp -o Site.o

Lattice.o: Lattice.h Lattice.cpp Site.o
	g++ -Wall -g -c -O3 Lattice.cpp -o Lattice.o

test: Site.o testSite.cpp
	g++ -Wall -g testSite.cpp Site.o -o test.exe

latTest: Site.o Lattice.o latTest.cpp
	g++ -Wall -g -O3 latTest.cpp Site.o Lattice.o -o latTest.exe 

anneal: Site.o Lattice.o anneal.cpp
	g++ -Wall -g -O3 anneal.cpp Site.o Lattice.o -o anneal.exe

clean:
	rm *~ \#*\# *.o