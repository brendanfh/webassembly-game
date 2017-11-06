#ifndef __INPUT_H_
#define __INPUT_H_

#include <GLFW/glfw3.h>
#include <iostream>

#include "gfx.h"

using namespace std;

class Keys {
private:
    static const int NUM_KEYS = 348;

    static bool keys[];
    static bool prevKeys[];

    Keys();
    ~Keys();
    
public:
    static void Initialize();
    static bool IsDown(int code);
    static bool IsJustDown(int code);
    static bool IsUp(int code);

    static void Tick();

};

class Mouse {
private:
    static const int NUM_BUTTONS = 8;

    static float x;
    static float y;
    static float dx;
    static float dy;

    static bool buttons[];

    Mouse();
    ~Mouse();

public:
    static void Initialize();
    static bool IsButtonDown(int button);
    static void GetRawPos(float &x, float &y);
    static void GetScreenPos(float &x, float &y);

    static void Tick();
};

#endif