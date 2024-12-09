

class MovementSystem {
public: 
	void update (float deltaTime, ComponentManager<TransformComponent>& transforms, ComponentManager<VelocityComponent>& velocities) {
		for (const auto& pair : velocities.getComponents()) {
			Entity entity = pair.first;
			auto velocity = pair.second;

			auto transform = transforms.getComponent(entity);
			if (transform) {
				transform->position += velocity->velocity * deltaTime;
			}
		}
	}
};

class RenderSystem { 
public:
	void render(sf::RenderWindow& window, ComponentManager<TransformComponent>& transforms, ComponentManager<GraphicsComponent>& graphics) {
		for (const auto& pair : graphics.getComponents()) {
			Entity entity = pair.first;
			auto graphic = pair.second;

			auto transform = transforms.getComponent(entity);
			if (transform) {
				graphic->shape.setPosition(transform->position);
				window.draw(graphic->shape);
			}
		}
	}
};
	
class BulletSystem {
public: 
	void update(float deltaTime, ComponentManager<TransformComponent>& transforms, ComponentManager<VelocityComponent>& velocities, ComponentManager<HealthComponent>& healths, EntityManager& entityManager) {
		for (const auto& pair : velocities.getComponents()) {
			Entity bullet = pair.first;
			auto velocity = pair.second;

			auto transform = transforms.getComponent(bullet);
			if (transform) {
				transform->position += velocity->velocity * deltaTime;
				
				if (transform->position.x < 0 || transform->position.x > 1920 || transform->position.y < 0 || transform->position.y > 1080) {
					bulletsToRemove.push_back(bullet);
				}
				else {
					for (const auto& pair : transforms.getComponents()) {
						Entity enemy = pair.first;
						auto enemyTransform = pair.second;
						if (enemy != bullet && healths.getComponent(enemy)) {
							if (enemyTransform->position.x < transform->position.x + transform->size.x &&
								enemyTransform->position.x + enemyTransform->size.x > transform->position.x &&
								enemyTransform->position.y < transform->position.y + transform->size.y &&
								enemyTransform->position.y + enemyTransform->size.y > transform->position.y) {

								auto health = healths.getComponent(enemy);
								if (health) {
									health->health -= 1;
									if (health->health <= 0) {
										enemiesToRemove.push_back(enemy);
									}
								}

								bulletsToRemove.push_back(bullet);
								break;
							}
						}
					}
				}
			}	
		}
		for (Entity bullet : bulletsToRemove) {
			entityManager.destroyEntity(bullet);
			transforms.removeComponent(bullet);
			velocities.removeComponent(bullet);
		}
		bulletsToRemove.clear();

		for (Entity enemy : enemiesToRemove) {
			entityManager.destroyEntity(enemy);
			transforms.removeComponent(enemy);
			healths.removeComponent(enemy);
		}
		enemiesToRemove.clear();

	}

	const std::vector<Entity>& getEntitiesToRemove() const {
		return bulletsToRemove;
	}

	void clear() {
		bulletsToRemove.clear();
	}

private:
		std::vector<Entity> bulletsToRemove;
		std::vector<Entity> enemiesToRemove;
};		
		
class ControlsSystem {
public:
	void handleInput(float deltaTime, const sf::RenderWindow& window, Entity player, ComponentManager<VelocityComponent>& velocities, ComponentManager<TransformComponent>& transforms, ComponentManager<GraphicsComponent>& graphics, EntityManager& entityManager) {
		const float speed = 150.0f;
		
		auto velocity = velocities.getComponent(player);
		if (!velocity) return;
		
		velocity->velocity = {0,0};
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			velocity->velocity.y = -speed;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			velocity->velocity.y = speed;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			velocity->velocity.x = -speed;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			velocity->velocity.x = speed;
		}
		
		static float shootCooldown = 0.0f;
		if (shootCooldown > 0.0f) {
			shootCooldown -= deltaTime;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootCooldown <= 0.0f) {
			auto transform = transforms.getComponent(player);
			if(!transform) return;
			
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			sf::Vector2f target = window.mapPixelToCoords(mousePosition);
			
			Entity bullet = entityManager.createEntity();
			
			sf::Vector2f direction = target - transform->position;
			float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
			if (magnitude != 0) {
				direction /= magnitude;
			}

			transforms.addComponent(bullet, {transform->position, {10.0f, 10.0f}});
			graphics.addComponent(bullet, { sf::RectangleShape({10.0f, 10.0f}) });
			velocities.addComponent(bullet, {direction * 600.0f});
			
			shootCooldown = 0.5f;
		}
	}	
};	