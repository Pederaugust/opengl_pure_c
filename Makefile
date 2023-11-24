CFLAGS = -g -Wall -Wextra -pedantic -std=c99 -lGLEW -lGLFW -framework OpenGL -lCGLM
CC = clang
OBJECTS=renderer.o vertex_buffer.o utils.o main.o index_buffer.o stb_image.o camera.o
SHADERS=src/shaders/vshader.glsl src/shaders/fshader.glsl
OBJECTOUT=$(addprefix dist/, $(OBJECTS))


# Build and run
default: build run

build: $(OBJECTS)
	echo "Linking objects"
	$(CC) $(CFLAGS) -o dist/main $(OBJECTOUT)

%.o: src/%.c
	echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o dist/$@

clean:
	rm -rf dist/*

run:
	./dist/main $(SHADERS)

debug: build
	sudo gdb --args ./dist/main $(SHADERS)

	
