Bead.o: Bead.cpp Bead.h
	g++ -Wall -g -c Bead.cpp

StringOhBeads.o: StringOhBeads.cpp StringOhBeads.h Bead.cpp Bead.h
	g++ -Wall -g -c StringOhBeads.cpp

string: StringOhBeads.o Bead.o stringFunc.cpp
	g++ -g stringFunc.cpp Bead.o StringOhBeads.o -o string.exe

bead: Bead.o beadWalk.cpp
	g++ -g beadWalk.cpp Bead.o -o bead.exe

clean:
	rm *~ \#*\# *.o