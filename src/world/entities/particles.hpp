#ifndef __PARTICLES_H_
#define __PARTICLES_H_

#include "../../utils/general.hpp"
#include "../world.hpp"

class Particle : public Entity {
protected:
	float life;
	float z;

	float dx, dy, dz;

public:
	Particle(float x, float y) : Entity() {
		type = EntityType::Particle;
		solid = false;

		this->x = x;
		this->y = y;
		this->z = 0.0f;
		this->dx = 0.0f;
		this->dy = 0.0f;
		this->dz = 0.0f;
		this->life = 4.0f;
	}

	~Particle() {
		Entity::~Entity();
	}

	void UpdateDrawRects() override {
		quad->SetPos(x, y + z);
	}

	void Tick(float dt) override {
		this->life -= dt;
		if (this->life <= 0) {
			this->alive = false;
		}
	}

	void Render() override {
		UpdateDrawRects();
		quad->BufferData();
	}
};

class GoreParticle : public Particle {
public:
	GoreParticle(float x, float y) : Particle(x, y) {
		dz = randf(-2.4f, -3.5f);
		dx = randf(-0.5f, 0.5f);

		quad->SetColor(randf(0.7f, 1.0f), 0.0f, 0.0f, 0.9f);
		quad->SetSubTexture(31.0f, 31.0f, 1.0f, 1.0f, 32.0f, 32.0f);
		quad->SetRect(x, y, randf(0.2f, 0.4f), randf(0.2f, 0.4f));
	}

	void Tick(float dt) override {
		Particle::Tick(dt);

		z += dz * dt;
		Move(dx * dt, 0, 1);

		dz += 6.0f * dt;
		if (z > 0) {
			alive = false;
		}
	}
};

#endif