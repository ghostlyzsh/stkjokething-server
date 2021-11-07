compile: all
	@echo "Done"

box2d:
	@cd dependencies/box2d; /bin/sh build.sh
	@rm -rf lib
	@mkdir lib
	@cp dependencies/box2d/build/bin/libbox2d.a lib/libbox2d.a
	@cp dependencies/box2d/build/bin/libglad.a lib/libglad.a
	@cp dependencies/box2d/build/bin/libglfw.a lib/libglfw.a
	@cp dependencies/box2d/build/bin/libimgui.a lib/libimgui.a
	@cp dependencies/box2d/build/bin/libsajson.a lib/libsajson.a

all: box2d
	@rm -rf bin
	@mkdir bin
	@clang++ -Wall -o bin/server -L opt_include -I opt_include -I include src/main.cpp lib/libbox2d.a lib/libglad.a lib/libglfw.a lib/libimgui.a lib/libsajson.a -lpthread -std=c++17 -Wc++17-extensions

clean:
	@cd dependencies/box2d/build; make clean
	@rm -rf lib
	@rm -rf bin

server:
	@rm -rf bin
	@mkdir bin
	@clang++ -Wall -o bin/server -L opt_include -I opt_include -I include src/main.cpp lib/libbox2d.a lib/libglad.a lib/libglfw.a lib/libimgui.a lib/libsajson.a -lpthread -std=c++17 -Wc++17-extensions

run: all
	@./bin/server
