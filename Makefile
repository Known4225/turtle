all:
	gcc turtle.c -L./Linux -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -DDEBUGGING_FLAG -Wall -o turtle.o
rel:
	gcc turtle.c -L./Linux -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -O3 -o turtle.o
lib:
	gcc -c include/turtleTools.h -DOS_LINUX -O3 -o turtleTools.o
	gcc -c include/osTools.h -DOS_LINUX -O3 -o osTools.o
	ar rcs turtle.dll turtleTools.o osTools.o
win:
	gcc turtle.c -L./Windows -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lturtle -DOS_WINDOWS -DDEBUGGING_FLAG -Wall -o turtle.exe
winrel:
	gcc turtle.c -L./Windows -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -DOS_WINDOWS -O3 -o turtle.exe
winlib:
	gcc -c include/turtleTools.h -DOS_WINDOWS -O3 -o turtleTools.o
	gcc -c include/osTools.h -DOS_WINDOWS -O3 -o osTools.o
	ar rcs turtle.dll turtleTools.o osTools.o
