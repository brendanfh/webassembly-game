#ifndef __ANIMATOR_H_
#define __ANIMATOR_H_

#define GLFW_INCLUDES_ES3
#include <GLFW/glfw3.h>
#include "../gfx.hpp"

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
    Animation() {
        frameX = -1;
        frameY = -1;
        frameWidth = -1;
        frameHeight = -1;
        textureWidth = -1;
        textureHeight = -1;

        frame = -1;
        maxFrames = -1;
        frameTimer = -1;
        maxFrameTimer = -1;
    }

    Animation(int x, int y, int width, int height, int t_width, int t_height, int numFrames, float delay) {
        frameX = x;
        frameY = y;
        frameWidth = width;
        frameHeight = height;
        textureWidth = t_width;
        textureHeight = t_height;

        frame = 0;
        maxFrames = numFrames;

        frameTimer = delay;
        maxFrameTimer = delay;
    }

    ~Animation() { }

    void ApplyToQuad(Gfx::Quad* quad) {
        quad->SetSubTexture((frameX + frame) * frameWidth, frameY * frameHeight, frameWidth, frameHeight, textureWidth, textureHeight);
    }

    void Tick(float dt) {
        frameTimer -= dt;
        if (frameTimer <= 0) {
            frameTimer = maxFrameTimer;
            frame += 1;
            if (frame >= maxFrames) {
                frame = 0;
            }
        }
    }
};

class Animator {
private:
    Animation* animations;
    int activeAnimation;
    int numAnimations;

public:
    Animator(int numAnims) {
        animations = new Animation[numAnims];
        numAnimations = numAnims;
        activeAnimation = 0;
    }

    ~Animator() {
        delete[] animations;
    }

    void SetAnimation(int id, int x, int y, int width, int height, int t_width, int t_height, int numFrames, float delay) {
        animations[id] = Animation(x, y, width, height, t_width, t_height, numFrames, delay);
    }

    void SetActive(int id) {
        if (id < 0 || id >= numAnimations) return;
        activeAnimation = id;
    }

    void ApplyToQuad(Gfx::Quad* quad) {
        animations[activeAnimation].ApplyToQuad(quad);
    }

    void Tick(float dt) {
        animations[activeAnimation].Tick(dt);
    }
};

#endif