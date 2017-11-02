CPP_FILES = $(shell find ./src/ -path *.cpp)

all:
	em++ $(CPP_FILES) -o build/index.html -s WASM=1 -s USE_GLFW=3 -s FULL_ES3=1 --shell-file ./template/minimal.html