#ifndef __INPUT_H_
#define __INPUT_H_

#include <GLFW/glfw3.h>
#include <iostream>

#include "gfx.hpp"

using namespace std;

class Keys {
private:
    static const int NUM_KEYS = 348;

    static bool keys[];
    static bool prevKeys[];

    Keys() { }
    ~Keys() { }
    
public:
    static void Initialize() {
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
    
    static bool IsDown(int code) {
        return Keys::keys[code];
    }
    
    static bool IsJustDown(int code) {
        return Keys::prevKeys[code];
    }
    
    static bool IsUp(int code) { 
        return !Keys::keys[code];
    }
    
    static void Tick() {
        for (int i = 0; i < NUM_KEYS; i++) {
            prevKeys[i] = false;
        }
    }        
};

class Mouse {
private:
    static const int NUM_BUTTONS = 8;

    static float x;
    static float y;
    static float dx;
    static float dy;

    static bool buttons[];

    Mouse() { }
    ~Mouse() { }

public:
    static void Initialize() {
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

    static bool IsButtonDown(int code) {
        return Mouse::buttons[code];
    }
    
    static void GetRawPos(float &x, float &y) {
        x = Mouse::x;
        y = Mouse::y;
    }
    
    static void GetScreenPos(float &x, float &y) {
        x = Mouse::x * (Gfx::width / Gfx::canvasWidth);
        y = Mouse::y * (Gfx::height / Gfx::canvasHeight);
    }
    
    static void Tick() { }
};

//Keyboard class functions
bool Keys::keys[Keys::NUM_KEYS];
bool Keys::prevKeys[Keys::NUM_KEYS];

//Mouse class functions
bool Mouse::buttons[Mouse::NUM_BUTTONS];
float Mouse::x = 0.0f;
float Mouse::y = 0.0f;
float Mouse::dx = 0.0f;
float Mouse::dy = 0.0f;

#endif