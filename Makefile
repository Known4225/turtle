all:
	gcc turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -DDEBUGGING_FLAG -Wall -o turtle.o
rel:
	gcc turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -O3 -o turtle.o
textures:
	gcc turtle.c -L./Linux -lturtletextures -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -DDEBUGGING_FLAG -Wall -o turtle.o
reltextures:
	gcc turtle.c -L./Linux -lturtletextures -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -O3 -o turtle.o
lib:
	cp turtle.h turtlelib.c
	gcc turtlelib.c -c -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_LINUX -O3 -o Linux/libturtle.a
	gcc turtlelib.c -c -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_TEXTURES -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_LINUX -O3 -o Linux/libturtletextures.a
	rm turtlelib.c
win:
	gcc turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -DDEBUGGING_FLAG -Wall -o turtle.exe
winrel:
	gcc turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -O3 -o turtle.exe
wintextures:
	gcc turtle.c -L./Windows -lturtletextures -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -DDEBUGGING_FLAG -Wall -o turtle.exe
winreltextures:
	gcc turtle.c -L./Windows -lturtletextures -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -O3 -o turtle.exe
winlib:
	cp turtle.h turtlelib.c
	gcc turtlelib.c -c -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_WINDOWS -O3 -o Windows/turtle.lib
	gcc turtlelib.c -c -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_TEXTURES -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_WINDOWS -O3 -o Windows/turtletextures.lib
	rm turtlelib.c