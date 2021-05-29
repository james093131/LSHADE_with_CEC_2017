all:
	g++ -Wall -O3 -o LSHADE main.cc

clean:
	rm -f main *.o
