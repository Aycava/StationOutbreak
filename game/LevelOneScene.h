#pragma once
#include "Scene.h"
#include "Systems.h"
#include "Renderer.h"
#include "Player.h"
#include "Enemy.h"

class LevelOneScene : public Scene {
public:
	LevelOneScene() = default;
	void load() override;
	void update(double deltaTime) override;
	void render() override;
private:
	void spawnEnemies(int count);
	void spawnBoss();
	sf::Text enemiesRemaining;
	int remainingEnemies;
	bool bossSpawned = false;
	sf::Font font;
	Entity player;
};