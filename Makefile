all:
	gcc turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -DDEBUGGING_FLAG -Wall -o turtle.o
rel:
	gcc turtle.c -L./Linux -lturtle -lglfw3 -ldl -lm -lX11 -lglad -lGL -lGLU -lpthread -DOS_LINUX -O3 -o turtle.o
lib:
	cp turtle.h turtlelib.c
	gcc turtlelib.c -c -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_TEXTURES -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_LINUX -O3 -o Linux/libturtle.a
	rm turtlelib.c
win:
	gcc turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -DDEBUGGING_FLAG -Wall -o turtle.exe
winrel:
	gcc turtle.c -L./Windows -lturtle -lglfw3 -lopengl32 -lgdi32 -lglad -lole32 -luuid -lwsock32 -lWs2_32 -lMf -lMfplat -lmfreadwrite -lmfuuid -DOS_WINDOWS -O3 -o turtle.exe
winlib:
	cp turtle.h turtlelib.c
	gcc turtlelib.c -c -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_TEXTURES -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_WINDOWS -O3 -o Windows/turtle.lib
	rm turtlelib.c
html:
	emcc turtle.c --shell-file config/turtle_shell.html -sUSE_GLFW=3 -sMAX_WEBGL_VERSION=2 -sASYNCIFY -sINITIAL_MEMORY=1073741824 -sWASM=0 -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_TEXTURES -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_BROWSER -Oz -o turtle.html --embed-file images
	gcc config/combine.c -o combine.exe
	./combine.exe turtle.html
	rm combine.exe
htmlserver:
	emcc turtle.c --shell-file config/turtle_shell.html -sUSE_GLFW=3 -sMAX_WEBGL_VERSION=2 -sASYNCIFY -sINITIAL_MEMORY=1073741824 -DTURTLE_IMPLEMENTATION -DTURTLE_ENABLE_TEXTURES -DTURTLE_ENABLE_SERIAL -DTURTLE_ENABLE_SOCKETS -DTURTLE_ENABLE_CAMERA -DOS_BROWSER -O3 -o turtle.html --embed-file images
runserver:
	emrun turtle.html