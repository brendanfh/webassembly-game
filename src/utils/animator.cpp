#include "animator.h"

Animation::Animation() {
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

Animation::Animation(int x, int y, int width, int height, int t_width, int t_height, int numFrames, float delay) {
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

Animation::~Animation() {

}

void Animation::ApplyToQuad(Gfx::Quad* quad) {
    quad->SetSubTexture((frameX + frame) * frameWidth, frameY * frameHeight, frameWidth, frameHeight, textureWidth, textureHeight);
}

void Animation::Tick(float dt) {
    frameTimer -= dt;
    if (frameTimer <= 0) {
        frameTimer = maxFrameTimer;
        frame += 1;
        if (frame >= maxFrames) {
            frame = 0;
        }
    }
}

Animator::Animator(int numAnims) {
    animations = new Animation[numAnims];
    numAnimations = numAnims;
    activeAnimation = 0;
}

Animator::~Animator() {
    delete[] animations;
}

void Animator::SetAnimation(int id , int x, int y, int width, int height, int t_width, int t_height, int numFrames, float delay) {
    animations[id] = Animation(x, y, width, height, t_width, t_height, numFrames, delay);
}

void Animator::SetActive(int id) {
    if (id < 0 || id >= numAnimations) return;
    activeAnimation = id;
}

void Animator::ApplyToQuad(Gfx::Quad* quad) {
    animations[activeAnimation].ApplyToQuad(quad);
}

void Animator::Tick(float dt) {
    animations[activeAnimation].Tick(dt);
}