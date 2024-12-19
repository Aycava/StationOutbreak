//Scene.h
#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
//sets up our base definition of a scene
class Scene {
public:
	Scene() = default;

	virtual ~Scene() = default;
	virtual void update(double deltaTime) = 0;
	virtual void render() = 0;
	virtual void load() = 0;

protected:
	//each scene should have its own entity manager
	EntityManager _ents;
};