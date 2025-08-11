all:
	gcc turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -DDEBUGGING_FLAG -Wall -o turtle.o
rel:
	gcc turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -O3 -o turtle.o
lib:
	gcc turtlelib.c -c -L./Linux -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DTURTLE_IMPLEMENTATION -DOS_LINUX -O3 -o Linux/libturtle.a
win:
	gcc turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DOS_WINDOWS -DDEBUGGING_FLAG -Wall -o turtle.exe
winrel:
	gcc turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DOS_WINDOWS -O3 -o turtle.exe
winlib:
	gcc turtlelib.c -c -L./Windows -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DTURTLE_IMPLEMENTATION -DOS_WINDOWS -O3 -o Windows/turtle.lib
preprocess:
	gcc include/turtle.h -E -o turtlepreprocess.h