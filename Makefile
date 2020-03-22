./bin/run: ./obj/main.o ./obj/robot.o
	gcc ./obj/main.o ./obj/robot.o -o ./bin/run -lm -lpthread
	cp ./bin/run run

./obj/main.o: ./src/main.c
	gcc -c ./src/main.c -o ./obj/main.o -lpthread

./obj/robot.o: ./src/robot.c
	gcc -c ./src/robot.c -o ./obj/robot.o -lpthread

clear: 
	rm ./obj/*.o

