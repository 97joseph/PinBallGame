# Check for OS Type
UNAME_S := $(shell uname -s)

# Linux
ifeq ($(UNAME_S), Linux)
	CC=g++
	LIBS=-lSDL2 -lGLEW -lGL -libassimp -lMagick++ -libBulletDynamics -libBulletCollision -libLinearMath
# Mac
else
	CC=clang++
	LIBS=-lSDL2 -framework OpenGL -framework Cocoa -lGLEW -stdlib=libc++ -libassimp -lMagick++ -libBulletDynamics -libBulletCollision -libLinearMath
endif

# Paths, use this to set Include directories, or library directories; uncomment in all: if used
#PATHI=-I/usr/local/include/
#PATHL=-L/usr/local/lib/

#Compiler Flags
CXXFLAGS=-g -Wall -std=c++0x

# .o Compilation
O_FILES=main.o Camera.o engine.o graphics.o object.o shader.o window.o loader.o obj.o event.o world.o physObject.o light.o imgui.o imgui_demo.o imgui_draw.o gui.o paddle.o

# Point to includes of local directories
INDLUDES=-I../include


all: $(O_FILES)
	$(CC) $(CXXFLAGS) -o Tutorial $(O_FILES) $(LIBS) $(PATHI) $(PATHL)

main.o: ../src/main.cpp
	$(CC) $(CXXFLAGS) -c ../src/main.cpp -o main.o $(INDLUDES)

imgui.o: ../src/imgui.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui.cpp -o imgui.o $(INDLUDES)

imgui_demo.o: ../src/imgui_demo.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_demo.cpp -o imgui_demo.o $(INDLUDES)

imgui_draw.o: ../src/imgui_draw.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_draw.cpp -o imgui_draw.o $(INDLUDES)

Camera.o: ../src/camera.cpp
	$(CC) $(CXXFLAGS) -c ../src/Camera.cpp -o Camera.o $(INDLUDES)

engine.o: ../src/engine.cpp
	$(CC) $(CXXFLAGS) -c ../src/engine.cpp -o engine.o $(INDLUDES)

graphics.o: ../src/graphics.cpp
	$(CC) $(CXXFLAGS) -c ../src/graphics.cpp -o graphics.o $(INDLUDES)

object.o: ../src/object.cpp
	$(CC) $(CXXFLAGS) -c ../src/object.cpp -o object.o $(INDLUDES)

physObject.o: ../src/physObject.cpp
	$(CC) $(CXXFLAGS) -c ../src/physObject.cpp -o physObject.o $(INDLUDES)

shader.o: ../src/shader.cpp
	$(CC) $(CXXFLAGS) -c ../src/shader.cpp -o shader.o $(INDLUDES)

window.o: ../src/window.cpp
	$(CC) $(CXXFLAGS) -c ../src/window.cpp -o window.o $(INDLUDES)
	
loader.o: ../src/loader.cpp
	$(CC) $(CXXFLAGS) -c ../src/loader.cpp -o loader.o $(INDLUDES)
	
obj.o: ../src/obj.cpp
	$(CC) $(CXXFLAGS) -c ../src/obj.cpp -o obj.o $(INDLUDES)

event.o: ../src/event.cpp
	$(CC) $(CXXFLAGS) -c ../src/event.cpp -o event.o $(INDLUDES)

world.o: ../src/world.cpp
	$(CC) $(CXXFLAGS) -c ../src/world.cpp -o world.o $(INDLUDES)

light.o: ../src/Light.cpp
	$(CC) $(CXXFLAGS) -c ../src/Light.cpp -o light.o $(INDLUDES)

gui.o: ../src/gui.cpp
	$(CC) $(CXXFLAGS) -c ../src/gui.cpp -o gui.o $(INDLUDES)

gui.o: ../src/paddle.cpp
	$(CC) $(CXXFLAGS) -c ../src/paddle.cpp -o paddle.o $(INDLUDES)

imgui_impl_sdl_gl3.o: ../src/imgui_impl_sdl_gl3.cpp
	$(CC) $(CXXFLAGS) -c ../src/imgui_impl_sdl_gl3.cpp -o imgui_impl_sdl_gl3.o $(INDLUDES)

clean:
	-@if rm *.o Tutorial 2>/dev/null || true; then echo "Main Removed"; else echo "No Main"; fi
