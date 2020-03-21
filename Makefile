./bin/run: ./obj/main.o ./obj/robot.o
	gcc ./obj/main.o ./obj/robot.o -o ./bin/run -lm
	cp ./bin/run run

./obj/main.o: ./src/main.c
	gcc -c ./src/main.c -o ./obj/main.o -g

./obj/robot.o: ./src/robot.c
	gcc -c ./src/robot.c -o ./obj/robot.o -g

clear: 
	rm ./obj/*.o

