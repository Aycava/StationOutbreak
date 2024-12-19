#include <SFML/Graphics.hpp>
#include "LevelOneScene.h"
#include "ComponentManager.h"
//instantiating our component managers and systems 
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
//creates player and enemies
void LevelOneScene::load() {
	player = makePlayer(_ents, transformManager, graphicsManager, velocityManager, healthManager);
	spawnEnemies(12);
}
//spawns enemies in groups of 3, using random numbers to ensure they do not spawn stacked on top of each other
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
//spawns boss and sets our bossSpawned bool to true
void LevelOneScene::spawnBoss() {
	makeBossEnemy(_ents, transformManager, graphicsManager, healthManager, shootingManager);
	bossSpawned = true;
}

void LevelOneScene::update(double deltaTime) {
	//handles player input
	controlsSystem.handleInput(deltaTime, Renderer::getWindow(), player, velocityManager, transformManager,
		graphicsManager, bulletManager, _ents);
	//updates various systems to do with entity activity
	movementSystem.update(deltaTime, transformManager, velocityManager);
	movementSystem.updateSeek(deltaTime, transformManager, seekManager, transformManager.getComponent(player)->position);
	movementSystem.updateFlee(deltaTime, transformManager, fleeManager, transformManager.getComponent(player)->position);
	bulletSystem.update(deltaTime, transformManager, velocityManager, healthManager, graphicsManager,
		shootingManager, fleeManager, bulletManager, seekManager, stateMachineManager,
		meleeManager, _ents, transformManager.getComponent(player)->position);
	//check if player is dead, if so closes window
	auto playerHealth = healthManager.getComponent(player);
	if (playerHealth && playerHealth->health <= 0) {
		Renderer::getWindow().close();
		return;
	}
	//update enemy count
	remainingEnemies = _ents.size() - 1;
	//checks if enemies are dead and if boss has been spawned, if not spawned then spawns boss, if spawned then closes window
	if (remainingEnemies <= 0 && !bossSpawned) {
		spawnBoss();
	} else if (remainingEnemies <= 0 && bossSpawned) {
		Renderer::getWindow().close();
	}
}
//renders entity related stuff
void LevelOneScene::render() {
	renderSystem.render(Renderer::getWindow(), transformManager, graphicsManager);
}