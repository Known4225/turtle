all: lib
	gcc turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -DDEBUGGING_FLAG -Wall -o turtle.o
rel: lib
	gcc turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -O3 -o turtle.o
textures: lib
	gcc turtle.c -L./Linux -lturtletextures -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -DDEBUGGING_FLAG -Wall -o turtle.o
reltextures: lib
	gcc turtle.c -L./Linux -lturtletextures -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -O3 -o turtle.o
lib: singlefile
	cp turtle.h turtlelib.c
	gcc turtlelib.c -c -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_LINUX -O3 -o Linux/libturtle.a
	gcc turtlelib.c -c -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_TEXTURES -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_LINUX -O3 -o Linux/libturtletextures.a
	rm turtlelib.c
source:
	gcc turtle.c src/*.c -L./Linux -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -DDEBUGGING_FLAG -Wall -o turtle.o
sourcerel:
	gcc turtle.c src/*.c -L./Linux -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -O3 -o turtle.o
win: winlib
	gcc turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -DDEBUGGING_FLAG -Wall -o turtle.exe
winrel: winlib
	gcc turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -O3 -o turtle.exe
wintextures: winlib
	gcc turtle.c -L./Windows -lturtletextures -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -DDEBUGGING_FLAG -Wall -o turtle.exe
winreltextures: winlib
	gcc turtle.c -L./Windows -lturtletextures -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -O3 -o turtle.exe
winlib: singlefile
	cp turtle.h turtlelib.c
	gcc turtlelib.c -c -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_WINDOWS -O3 -o Windows/turtle.lib
	gcc turtlelib.c -c -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_TEXTURES -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_WINDOWS -O3 -o Windows/turtletextures.lib
	rm turtlelib.c
winsource:
	gcc turtle.c src/*.c -L./Windows -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -DDEBUGGING_FLAG -Wall -o turtle.exe
winsourcerel:
	gcc turtle.c src/*.c -L./Windows -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -O3 -o turtle.exe
singlefile:
	gcc singlefile.c -o singlefile.exe
	./singlefile.exe