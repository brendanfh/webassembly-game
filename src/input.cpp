#include "input.h"

Keys::Keys() { }

Keys::~Keys() { }

bool Keys::keys[348];
void Keys::Initialize() {
    for (int i = 0; i < 348; i++) {
        Keys::keys[i] = false;
    }

    glfwSetKeyCallback(Gfx::window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Keys::keys[key] = action >= GLFW_PRESS;
    });
}

bool Keys::IsDown(int code) {
    return Keys::keys[code];
}

bool Keys::IsUp(int code) {
    return !Keys::keys[code];
}