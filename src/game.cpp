#include "game.h"
#include "gfx.h"

#include <cmath>
#include <string>

Game::Game() {
	Gfx::Initialize();
	lastTime = glfwGetTime();

	t = 0.0;
	quadData = new GLfloat[7 * 4];
	Triangle(.25f, 0, 0);
}

Game::~Game() {
	cout << "Destuctor was called" << endl;
	delete[] quadData;
}

void Game::Triangle(float size, float x, float y) {
	quadData[7 * 0 + 0] = -1.0f * size + x;
	quadData[7 * 0 + 1] = -1.0f * size + y;
	quadData[7 * 0 + 2] = 1.0f;
	quadData[7 * 0 + 3] = 1.0f;
	quadData[7 * 0 + 4] = 1.0f;
	quadData[7 * 0 + 5] = 1.0f;
	quadData[7 * 0 + 6] = 1.0f;
	quadData[7 * 1 + 0] = 1.0f * size + x;
	quadData[7 * 1 + 1] = -1.0f * size + y;
	quadData[7 * 1 + 2] = 1.0f;
	quadData[7 * 1 + 3] = 1.0f;
	quadData[7 * 1 + 4] = 1.0f;
	quadData[7 * 1 + 5] = 1.0f;
	quadData[7 * 1 + 6] = 1.0f;
	quadData[7 * 2 + 0] = 0.0f * size + x;
	quadData[7 * 2 + 1] = 1.0f * size + y;
	quadData[7 * 2 + 2] = 1.0f;
	quadData[7 * 2 + 3] = 1.0f;
	quadData[7 * 2 + 4] = 0.0f;
	quadData[7 * 2 + 5] = 0.0f;
	quadData[7 * 2 + 6] = 1.0f;
	
	Gfx::BufferData(sizeof(GLfloat) * 28, quadData);
}

#define TAU (3.1415926595 * 2)
void Game::tick() {
	double currTime = glfwGetTime();
	double dt = currTime - lastTime;
	lastTime = currTime;

	t += dt;
	if (t > TAU) t -= TAU;
	Triangle(3.0f, 3.0f * cos(t) + 5.0f, 3.0f * sin(t) + 5.0f);
}

void Game::render() {
    int width, height;
    glfwGetFramebufferSize(Gfx::window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gfx::SetupDraw();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	Gfx::CleanupDraw();
}
