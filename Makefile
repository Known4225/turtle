CC = gcc

all: lib libtextures
	$(CC) turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -DDEBUGGING_FLAG -Wall -o turtle.o
rel: lib libtextures
	$(CC) turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -O3 -o turtle.o
lib: singlefile
	cp turtle.h turtlelib.c
	$(CC) turtlelib.c -c -L./Linux -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DTURTLE_IMPLEMENTATION -DOS_LINUX -O3 -o Linux/libturtle.a
	rm turtlelib.c
libtextures:
	cp turtle.h turtlelib.c
	$(CC) turtlelib.c -c -L./Linux -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_TEXTURES -DOS_LINUX -O3 -o Linux/libturtletextures.a
	rm turtlelib.c
source:
	$(CC) turtle.c src/*.c -L./Linux -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -DDEBUGGING_FLAG -Wall -o turtle.o
sourcerel:
	$(CC) turtle.c src/*.c -L./Linux -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -O3 -o turtle.o
win: winlib winlibtextures
	$(CC) turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DOS_WINDOWS -DDEBUGGING_FLAG -Wall -o turtle.exe
winrel: winlib winlibtextures
	$(CC) turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DOS_WINDOWS -O3 -o turtle.exe
winlib: singlefile
	cp turtle.h turtlelib.c
	$(CC) turtlelib.c -c -L./Windows -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DTURTLE_IMPLEMENTATION -DOS_WINDOWS -O3 -o Windows/turtle.lib
	rm turtlelib.c
winlibtextures: singlefile
	cp turtle.h turtlelib.c
	$(CC) turtlelib.c -c -L./Windows -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_TEXTURES -DOS_WINDOWS -O3 -o Windows/turtletextures.lib
	rm turtlelib.c
winsource:
	$(CC) turtle.c src/*.c -L./Windows -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DOS_WINDOWS -DDEBUGGING_FLAG -Wall -o turtle.exe
winsourcerel:
	$(CC) turtle.c src/*.c -L./Windows -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DOS_WINDOWS -O3 -o turtle.exe
singlefile:
	$(CC) deploy.c -o deploy.exe
	./deploy.exe