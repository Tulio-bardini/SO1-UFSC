CC=g++
all:main

main:main.o cpu.o main_class.o
	g++ main.o cpu.o main_class.o -o main

main.o: main.cc
	g++ -c main.cc

cpu.o: cpu.cc
	g++ -c cpu.cc

main_class.o: main_class.cc
	g++ -c main_class.cc

clean:
	rm *.o main