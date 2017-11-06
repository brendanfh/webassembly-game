#ifndef __INPUT_H_
#define __INPUT_H_

#include <GLFW/glfw3.h>
#include <iostream>

#include "gfx.h"

using namespace std;

class Keys {
private:
    static bool keys[];

    Keys();
    ~Keys();
    
public:
    static void Initialize();
    static bool IsDown(int code);
    static bool IsUp(int code);
};

#endif