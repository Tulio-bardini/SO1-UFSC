CC=g++
all:main

DEPS = cpu.h debug.h main_class.h system.h thread.h traits.h


# main: cpu.cc debug.cc main_class.cc main.cc system.cc thread.cc
# 	g++ cpu.cc debug.cc main_class.cc main.cc system.cc thread.cc -o main

main: cpu.o debug.o system.o thread.o main.o main_class.o traits.h
	g++ cpu.o debug.o system.o thread.o main.o main_class.o -o main

cpu.o: cpu.cc $(DEPS)
	g++ -c cpu.cc

debug.o: debug.cc $(DEPS)
	g++ -c debug.cc

system.o: system.cc $(DEPS)
	g++ -c system.cc

thread.o: thread.cc $(DEPS)
	g++ -c thread.cc

main.o: main.cc system.cc $(DEPS)
	g++ -c main.cc

main_class.o: main_class.cc $(DEPS)
	g++ -c main_class.cc

clean:
	rm *.o main