CPP_FILES = $(shell find ./src/ -path *.cpp)
INCLUDES = ""

PORT = 8080

all:
	em++ $(CPP_FILES) -o build/index.html \
		-std=c++14 -O2 \
		-s WASM=1 -s USE_GLFW=3 -s FULL_ES3=1 -s USE_SDL=2 -s USE_SDL_IMAGE=1 \
		--preload-file res/ --use-preload-plugins \
		--shell-file ./template/minimal.html

server:
	python -m SimpleHTTPServer $(PORT)