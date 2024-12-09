#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"

#include <SFML/Graphics.hpp>

Entity makeEnemy(EntityManager& entityManager, ComponentManager<TransformComponent>& transforms, ComponentManager<GraphicsComponent>& graphics, ComponentManager<HealthComponent>& healths) {
	Entity enemy = entityManager.createEntity();
	
	transforms.addComponent(enemy, { {500, 300}, {40,40} });

	sf::RectangleShape shape({ 40, 40 });
	shape.setFillColor(sf::Color::Red);
	graphics.addComponent(enemy, { shape });

	healths.addComponent(enemy, { 1 });

	return enemy;
}