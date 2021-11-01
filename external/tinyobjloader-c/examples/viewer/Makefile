.PHONY: clean all

# OS specific linker flags for GLFW
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
LDFLAGS=-framework OpenGL -lglfw3 -framework Cocoa -framework IOKit -framework CoreVideo
CFLAGS=-Wall -Wextra -Wshadow -Wformat=2 -pedantic -std=c99 -O2 -g
endif
ifeq ($(UNAME_S),Linux)
LDFLAGS=-lGL -lGLU -lm -pthread -ldl `pkg-config --libs glfw3`
CFLAGS=-Wall -Wextra -Wshadow -Wformat=2 -pedantic -std=c99 -O2 -g `pkg-config --cflags glfw3`
endif

all: viewer

viewer: viewer.o trackball.o glad.o
	$(CC) -o $@ $^ $(LDFLAGS)

viewer.o: viewer.c ../../tinyobj_loader_c.h trackball.h glad.h
	$(CC) -c -o $@ $(CFLAGS) viewer.c

trackball.o: trackball.c trackball.h
	$(CC) -c -o $@ $(CFLAGS) trackball.c

glad.o: glad.c glad.h khrplatform.h
	$(CC) -c -o $@ $(CFLAGS) glad.c

clean:
	@rm -rf viewer viewer.o trackball.o glad.o
