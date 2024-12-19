//scene.h
#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"

class Scene {
public:
	Scene() = default;

	virtual ~Scene() = default;
	virtual void update(double deltaTime) = 0;
	virtual void render() = 0;
	virtual void load() = 0;

protected:
	EntityManager _ents;
};