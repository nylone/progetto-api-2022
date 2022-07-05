main: main.o string-map.o string-hasher.o
	gcc -o main.out build-files/main.o build-files/string-map.o build-files/string-hasher.o

main.o: main.c libraries/string-map.h
	gcc -o build-files/main.o -c main.c

string-map.o: libraries/string-map.c libraries/string-map.h libraries/string-hasher.h
	gcc -o build-files/string-map.o -c libraries/string-map.c

string-hasher.o: libraries/string-hasher.c libraries/string-hasher.h
	gcc -o build-files/string-hasher.o -c libraries/string-hasher.c

$(shell mkdir --parents build-files)