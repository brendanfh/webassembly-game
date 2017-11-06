#include "input.h"

//Keyboard class functions
Keys::Keys() { }
Keys::~Keys() { }

bool Keys::keys[Keys::NUM_KEYS];
bool Keys::prevKeys[Keys::NUM_KEYS];
void Keys::Initialize() {
    for (int i = 0; i < NUM_KEYS; i++) {
        Keys::keys[i] = false;
        Keys::prevKeys[i] = false;
    }

    glfwSetKeyCallback(Gfx::window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS && Keys::keys[key] == false) {
            prevKeys[key] = true;
        }
        Keys::keys[key] = action >= GLFW_PRESS;
    });
}

bool Keys::IsDown(int code) {
    return Keys::keys[code];
}

bool Keys::IsJustDown(int code) {
    return Keys::prevKeys[code];
}

bool Keys::IsUp(int code) {
    return !Keys::keys[code];
}

void Keys::Tick() {
    for (int i = 0; i < NUM_KEYS; i++) {
        prevKeys[i] = false;
    }
}


//Mouse class functions
Mouse::Mouse() { }
Mouse::~Mouse() { }

bool Mouse::buttons[Mouse::NUM_BUTTONS];
float Mouse::x = 0.0f;
float Mouse::y = 0.0f;
float Mouse::dx = 0.0f;
float Mouse::dy = 0.0f;

void Mouse::Initialize() {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i] = false;
    }

    glfwSetCursorPosCallback(Gfx::window, [](GLFWwindow* window, double xpos, double ypos) {
        float fxpos = (float)xpos;
        float fypos = (float)ypos;

        Mouse::dx = fxpos - Mouse::x;
        Mouse::dy = fypos - Mouse::y;
        Mouse::x = fxpos;
        Mouse::y = fypos;
    });

    glfwSetMouseButtonCallback(Gfx::window, [](GLFWwindow* window, int button, int action, int mods) {
        buttons[button] = action >= GLFW_PRESS;
    });
}

bool Mouse::IsButtonDown(int code) {
    return Mouse::buttons[code];
}

void Mouse::GetRawPos(float &x, float &y) {
    x = Mouse::x;
    y = Mouse::y;
}

void Mouse::GetScreenPos(float &x, float &y) {
    x = Mouse::x * (Gfx::width / Gfx::canvasWidth);
    y = Mouse::y * (Gfx::height / Gfx::canvasHeight);
}

void Mouse::Tick() { }