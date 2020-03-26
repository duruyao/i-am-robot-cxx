bin/run: obj/main.o obj/robot.o obj/action.o obj/step.o obj/window.o obj/func.o 
	g++ obj/main.o obj/robot.o obj/action.o obj/step.o \
		obj/window.o obj/func.o -o bin/run -lm -lpthread -g
	cp bin/run run

obj/main.o: src/main.cxx
	g++ -c src/main.cxx -o obj/main.o -lpthread -g

obj/robot.o: src/robot.cxx
	g++ -c src/robot.cxx -o obj/robot.o -g

obj/action.o: src/action.cxx
	g++ -c src/action.cxx -o obj/action.o -g

obj/step.o: src/step.cxx
	g++ -c src/step.cxx -o obj/step.o -g

obj/window.o: src/window.cxx
	g++ -c src/window.cxx -o obj/window.o -g

obj/func.o: src/func.c
	g++ -c src/func.c -o obj/func.o -g

clear: 
	rm ./obj/*.o

