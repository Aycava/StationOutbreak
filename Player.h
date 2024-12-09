#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Player {
public:
	Player() {
		shape.setSize({ 50.0f, 50.0f });
		shape.setFillColor(sf::Color::Cyan);
		shape.setPosition(960.0f, 540.0f);
	}

	void input(float deltaTime, const sf::RenderWindow& window) {
		const float speed = 150.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			shape.move(0, -speed * deltaTime);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			shape.move(0, speed * deltaTime);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			shape.move(-speed * deltaTime, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			shape.move(speed * deltaTime, 0);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootCooldown <= 0.0f) {
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			shoot(window.mapPixelToCoords(mousePosition));
			shootCooldown = 0.5f;
		}
	}
	void update(float deltaTime) {
		if (shootCooldown > 0.0f) {
			shootCooldown -= deltaTime;
		}

		for (size_t i = 0; i < bullets.size(); ++i) {
			bullets[i].move(bulletVelocities[i] * deltaTime);
		}

		for (size_t i = 0; i < bullets.size();) {
			sf::Vector2f pos = bullets[i].getPosition();
			if (pos.x < 0 || pos.x > 1920 || pos.y < 0 || pos.y > 1080) {
				bullets.erase(bullets.begin() + i);
				bulletVelocities.erase(bulletVelocities.begin() + i);
			}
			else {
				++i;
			}
		}
	}

	void render() {
		Renderer::addDrawable(&shape);

		for (const auto& bullet : bullets) {
			Renderer::addDrawable(&bullet);
		}
	}

private:

	void shoot(const sf::Vector2f& target) {
		sf::RectangleShape bullet({ 10.0f, 10.0f });
		bullet.setFillColor(sf::Color::Red);

		sf::Vector2f playerCenter = shape.getPosition() + (shape.getSize() / 2.0f);
		bullet.setPosition(playerCenter);

		sf::Vector2f direction = target - playerCenter;

		float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (magnitude != 0) {
			direction /= magnitude;
		}

		bullets.push_back(bullet);
		bulletVelocities.push_back(direction * bulletSpeed);
	}
	sf::RectangleShape shape;
	std::vector<sf::RectangleShape> bullets;
	std::vector<sf::Vector2f> bulletVelocities;
	float shootCooldown = 0.0f;
	const float bulletSpeed = 600.0f;
};