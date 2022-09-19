CC=g++
all:main

# main: cpu.cc debug.cc main_class.cc main.cc system.cc thread.cc
# 	g++ cpu.cc debug.cc main_class.cc main.cc system.cc thread.cc -o main

main: cpu.o debug.o system.o thread.o main.o main_class.o
	g++ cpu.o debug.o system.o thread.o main.o main_class.o -o main

cpu.o: cpu.cc
	g++ -c cpu.cc

debug.o: debug.cc
	g++ -c debug.cc

system.o: system.cc
	g++ -c system.cc

thread.o: thread.cc
	g++ -c thread.cc

main.o: main.cc system.cc
	g++ -c main.cc

main_class.o: main_class.cc
	g++ -c main_class.cc

clean:
	rm *.o main