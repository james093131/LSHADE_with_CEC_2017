all:
	g++ -Wall -O3 -o SHADE main.cc

clean:
	rm -f main *.o
