#pragma once

struct TransformComponent {
	sf::Vector2f position;
	sf::Vector2f size;
};

struct VelocityComponent {
	sf::Vector2f velocity;
};

struct HealthComponent {
	int health;
};

struct GraphicsComponent {
	sf::RectangleShape shape;
};

	
	