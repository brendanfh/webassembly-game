#ifndef __ANIMATOR_H_
#define __ANIMATOR_H_

#define GLFW_INCLUDES_ES3
#include <GLFW/glfw3.h>
#include "../gfx.h"

/*
Animations are in a spritesheet and go left to right

/-------------\
|0|1|2|3|4|5|6|
\-------------/
*/
class Animation {
private:
    int maxFrames;
    int frame;

    float maxFrameTimer;
    float frameTimer;

    int textureWidth, textureHeight;
    int frameWidth, frameHeight;
    int frameX, frameY;

public:
    Animation();
    Animation(int x, int y, int width, int height, int t_width, int t_height, int numFrames, float delay);
    ~Animation();

    void ApplyToQuad(Gfx::Quad* quad);
    void Tick(float dt);
};

class Animator {
private:
    Animation* animations;
    int activeAnimation;
    int numAnimations;

public:
    Animator(int numAnimations);
    ~Animator();

    void SetAnimation(int id, int x, int y, int width, int height, int t_width, int t_height, int numFrames, float delay);
    void SetActive(int id);

    void ApplyToQuad(Gfx::Quad* quad);
    void Tick(float dt);
};

#endif