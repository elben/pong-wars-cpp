CXX = g++
FLAGS = -ggdb -lSDLmain -lSDL_ttf -lSDL_image -lSDL_mixer -lSDL

main: pongwars.cc
	${CXX} ${FLAGS} -o pongwars pongwars.cc

clean:
	rm -f pongwars
