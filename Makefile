all: lib
	gcc turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -DDEBUGGING_FLAG -Wall -o turtle.o
rel: lib
	gcc turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -O3 -o turtle.o
lib: singlefile
	cp singlefile_output/turtle.h singlefile_output/turtlelib.c
	gcc singlefile_output/turtlelib.c -c -L./Linux -I. -I./include -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DTURTLE_IMPLEMENTATION -DOS_LINUX -O3 -o Linux/libturtle.a
	rm singlefile_output/turtlelib.c
win: winlib
	gcc turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DOS_WINDOWS -DDEBUGGING_FLAG -Wall -o turtle.exe
winrel: winlib
	gcc turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DOS_WINDOWS -O3 -o turtle.exe
winlib: singlefile
	cp singlefile_output/turtle.h singlefile_output/turtlelib.c
	gcc singlefile_output/turtlelib.c -c -L./Windows -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DTURTLE_IMPLEMENTATION -DOS_WINDOWS -O3 -o Windows/turtle.lib
	rm singlefile_output/turtlelib.c
singlefile:
	gcc deploy.c -o deploy.exe
	./deploy.exe