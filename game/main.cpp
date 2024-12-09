//main.cpp
#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Renderer.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"
#include "Systems.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp> 

int main() {


	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Station Outbreak");

	Renderer::initialise(window);
	
	EntityManager entityManager;
	ComponentManager<TransformComponent> transformManager;
	ComponentManager<GraphicsComponent> graphicsManager;
	ComponentManager<VelocityComponent> velocityManager;
	ComponentManager<HealthComponent> healthManager;

	ControlsSystem controlsSystem;
	RenderSystem renderSystem;
	MovementSystem movementSystem;
	BulletSystem bulletSystem;
	
	Entity player = makePlayer(entityManager, transformManager, graphicsManager, velocityManager);
	Entity enemy = makeEnemy(entityManager, transformManager, graphicsManager, healthManager);

	sf::Clock clock;
	while (Renderer::getWindow().isOpen()) {
		sf::Event event;
		while (Renderer::getWindow().pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				Renderer::getWindow().close();
		}

		float deltaTime = clock.restart().asSeconds();

		controlsSystem.handleInput(deltaTime, window, player, velocityManager, transformManager, graphicsManager, entityManager); 
		
		movementSystem.update(deltaTime, transformManager, velocityManager);

		bulletSystem.update(deltaTime, transformManager, velocityManager, healthManager, entityManager);

		Renderer::getWindow().clear();

		renderSystem.render(Renderer::getWindow(), transformManager, graphicsManager);

		Renderer::getWindow().display();
	}

	Renderer::close();

	return 0;
}