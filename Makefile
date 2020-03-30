bin/run: obj/main.o obj/robot.o obj/action.o obj/step.o obj/window.o obj/func.o 
	g++ obj/main.o obj/robot.o obj/action.o obj/step.o \
		obj/window.o obj/func.o -o bin/run -lm -lpthread
	cp bin/run run

obj/main.o: src/main.cxx
	g++ -c src/main.cxx -o obj/main.o -lpthread

obj/robot.o: src/robot.cxx
	g++ -c src/robot.cxx -o obj/robot.o

obj/action.o: src/action.cxx
	g++ -c src/action.cxx -o obj/action.o

obj/step.o: src/step.cxx
	g++ -c src/step.cxx -o obj/step.o

obj/window.o: src/window.cxx
	g++ -c src/window.cxx -o obj/window.o

obj/func.o: src/func.c
	g++ -c src/func.c -o obj/func.o

clear: 
	rm ./obj/*.o

