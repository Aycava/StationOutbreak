//LevelOneScene.h
#pragma once
#include "Scene.h"
#include "Systems.h"
#include "Renderer.h"
#include "Player.h"
#include "Enemy.h"
//sets up our level one scene, further defined in LevelOneScene.cpp
class LevelOneScene : public Scene {
public:
	LevelOneScene() = default;
	void load() override;
	void update(double deltaTime) override;
	void render() override;
private:
	void spawnEnemies(int count);
	void spawnBoss();
	//to keep track of enemies left in scene
	int remainingEnemies;
	//to mark whether or not boss has been spawned
	bool bossSpawned = false;
	Entity player;
};