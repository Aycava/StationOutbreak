#include <SFML/Graphics.hpp>
#include "LevelOneScene.h"
#include "ComponentManager.h"

ComponentManager<TransformComponent> transformManager;
ComponentManager<GraphicsComponent> graphicsManager;
ComponentManager<VelocityComponent> velocityManager;
ComponentManager<HealthComponent> healthManager;
ComponentManager<FleeComponent> fleeManager;
ComponentManager<ShootingComponent> shootingManager;
ComponentManager<SeekComponent> seekManager;
ComponentManager<BulletComponent> bulletManager;
ComponentManager<MeleeComponent> meleeManager;
ComponentManager<StateMachineComponent> stateMachineManager;

ControlsSystem controlsSystem;
RenderSystem renderSystem;
MovementSystem movementSystem;
BulletSystem bulletSystem;
MeleeSystem meleeSystem;
StateMachineSystem stateMachineSystem;

void LevelOneScene::load() {
	enemiesRemaining.setString("12/12");
	enemiesRemaining.setCharacterSize(20);
	enemiesRemaining.setPosition(10, 10);
	enemiesRemaining.setFillColor(sf::Color::White);
	player = makePlayer(_ents, transformManager, graphicsManager, velocityManager, healthManager);
	spawnEnemies(12);
}

void LevelOneScene::spawnEnemies(int count) {
	for (int i = 0; i < count/3; i++) {
		float x = 100 + (rand() % 800);
		float y = 100 + (rand() % 800);
		float x2 = 100 + (rand() % 1200);
		float y2 = 100 + (rand() % 900);
		float x3 = 100 + (rand() % 1600);
		float y3 = 100 + (rand() % 1000);
		auto enemy = makeAggressiveEnemy(_ents, transformManager, graphicsManager, healthManager, seekManager, meleeManager);
		auto seekEnemy = makeIdleSeekEnemy(_ents, transformManager, graphicsManager, healthManager, stateMachineManager, meleeManager);
		auto rangedEnemy = makeRangedEnemy(_ents, transformManager, graphicsManager, healthManager, fleeManager, shootingManager);

		auto transform = transformManager.getComponent(enemy);
		auto transform2 = transformManager.getComponent(seekEnemy);
		auto transform3 = transformManager.getComponent(rangedEnemy);

		if (transform) {
			transform->position = { x2, y2 };
			transform2->position = { x, y };
			transform3->position = { x3, y3 };
		}
	}
}

void LevelOneScene::spawnBoss() {
	makeBossEnemy(_ents, transformManager, graphicsManager, healthManager, shootingManager);
	bossSpawned = true;
}

void LevelOneScene::update(double deltaTime) {
	controlsSystem.handleInput(deltaTime, Renderer::getWindow(), player, velocityManager, transformManager,
		graphicsManager, bulletManager, _ents);

	movementSystem.update(deltaTime, transformManager, velocityManager);
	movementSystem.updateSeek(deltaTime, transformManager, seekManager, transformManager.getComponent(player)->position);
	movementSystem.updateFlee(deltaTime, transformManager, fleeManager, transformManager.getComponent(player)->position);
	bulletSystem.update(deltaTime, transformManager, velocityManager, healthManager, graphicsManager,
		shootingManager, fleeManager, bulletManager, seekManager, stateMachineManager,
		meleeManager, _ents, transformManager.getComponent(player)->position);

	remainingEnemies = _ents.size() - 1;
	if (remainingEnemies <= 0 && !bossSpawned) {
		spawnBoss();
	}

	enemiesRemaining.setString(std::to_string(remainingEnemies) + "/12");
}

void LevelOneScene::render() {
	Renderer::addDrawable(&enemiesRemaining);
	renderSystem.render(Renderer::getWindow(), transformManager, graphicsManager);
}