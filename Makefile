CPP_FILES = $(shell find ./src/ -path *.cpp)
INCLUDES = "/mnt/c/dev/wasm/tools/glm"

all:
	em++ $(CPP_FILES) -o build/index.html -std=c++14 -I$(INCLUDES) -s WASM=1 -s USE_GLFW=3 -s FULL_ES3=1 --shell-file ./template/minimal.html