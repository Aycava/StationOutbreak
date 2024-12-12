#pragma once
#include "ComponentManager.h"
#include "EntityManager.h"
#include "Components.h"
#include "Renderer.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>


class MovementSystem {
public:
    void update(float deltaTime, ComponentManager<TransformComponent>& transforms,
                ComponentManager<VelocityComponent>& velocities) {
        for (const auto& pair : velocities.getComponents()) {
            Entity entity = pair.first;
            auto velocity = pair.second;

            auto transform = transforms.getComponent(entity);
            if (transform) {
                transform->position += velocity->velocity * deltaTime;
            }
        }
    }

    void updateFlee(float deltaTime, ComponentManager<TransformComponent>& transforms,
                    ComponentManager<FleeComponent>& flees, const sf::Vector2f& playerPosition) {
        for (const auto& pair : flees.getComponents()) {
            Entity entity = pair.first;
            auto flee = pair.second;
            auto transform = transforms.getComponent(entity);

            if (transform) {
                float distance = std::sqrt(std::pow(playerPosition.x - transform->position.x, 2) +
                                           std::pow(playerPosition.y - transform->position.y, 2));
                if (distance < flee->fleeRadius) {
                    sf::Vector2f direction = transform->position - playerPosition;
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    if (length != 0) {
                        direction /= length;
                    }
                    transform->position += direction * static_cast<float>(flee->speed * deltaTime);
                }
            }
        }
    }

    void updateSeek(float deltaTime, ComponentManager<TransformComponent>& transforms,
                    ComponentManager<SeekComponent>& seekers, const sf::Vector2f& playerPosition) {
        for (const auto& pair : seekers.getComponents()) {
            Entity entity = pair.first;
            auto seeker = pair.second;
            auto transform = transforms.getComponent(entity);

            if (transform) {
                sf::Vector2f direction = playerPosition - transform->position;
                float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (magnitude != 0) {
                    direction /= magnitude; 
                }
                transform->position += direction * seeker->speed * deltaTime;
            }
        }
    }
};

class BulletSystem {
public:
    void update(float deltaTime, ComponentManager<TransformComponent>& transforms,
                ComponentManager<VelocityComponent>& velocities,
                ComponentManager<HealthComponent>& healths,
                ComponentManager<GraphicsComponent>& graphics,
                ComponentManager<ShootingComponent>& shootings,
                ComponentManager<FleeComponent>& flees,
                ComponentManager<BulletComponent>& bullets,
                ComponentManager<SeekComponent>& seekers,
				ComponentManager<StateMachineComponent>& stateMachines,
				ComponentManager<MeleeComponent>& melees,
                EntityManager& entityManager,
                const sf::Vector2f& playerPosition) {
        // Update existing bullets
        for (const auto& pair : velocities.getComponents()) {
            Entity bullet = pair.first;
            auto velocity = pair.second;
            auto transform = transforms.getComponent(bullet);
            auto bulletComp = bullets.getComponent(bullet);

            if (!transform || !bulletComp) continue;

            // Move the bullet
            transform->position += velocity->velocity * deltaTime;

            // Remove bullet if it goes off-screen
            if (transform->position.x < 0 || transform->position.x > 1920 ||
                transform->position.y < 0 || transform->position.y > 1080) {
                bulletsToRemove.push_back(bullet);
                continue;
            }

            // Check for collisions with entities
            for (const auto& entityPair : healths.getComponents()) {
                Entity target = entityPair.first;
                if (target == bullet) continue;

                auto targetTransform = transforms.getComponent(target);
                if (!targetTransform) continue;

                if (isColliding(*transform, *targetTransform)) {
                    bool shouldDamage = false;

                    if (bulletComp->isPlayerBullet &&
                        (shootings.getComponent(target) != nullptr || seekers.getComponent(target) != nullptr) || stateMachines.getComponent(target) !=nullptr) {
                        shouldDamage = true;
                    } else if (!bulletComp->isPlayerBullet && shootings.getComponent(target) == nullptr) {
                        shouldDamage = true;
                    }

                    if (shouldDamage) {
    auto health = healths.getComponent(target);
    if (health) {
        health->health -= 1;
        if (health->health <= 0) {
            if (shootings.getComponent(target) == nullptr) {  
                
                graphics.removeComponent(target);
            } else {  
                enemiesToRemove.push_back(target);
            }
        }
    }
    bulletsToRemove.push_back(bullet);
    break;
}
                }
            }
        }

        // Handle enemy shooting
        for (const auto& pair : shootings.getComponents()) {
            Entity entity = pair.first;
            auto shooting = pair.second;
            auto transform = transforms.getComponent(entity);

            if (transform) {
                shooting->shootTimer += deltaTime;
                if (shooting->shootTimer >= shooting->shootInterval) {
                    shooting->shootTimer = 0;

                    float distance = std::sqrt(
                        std::pow(playerPosition.x - transform->position.x, 2) +
                        std::pow(playerPosition.y - transform->position.y, 2));

                    if (distance < 600.0f) {
                        sf::Vector2f direction = playerPosition - transform->position;
                        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                        if (magnitude != 0) {
                            direction /= magnitude;
                        }

                        Entity bullet = entityManager.createEntity();
                        transforms.addComponent(bullet, {transform->position, {10.0f, 10.0f}});
                        
                        sf::RectangleShape bulletShape({10.0f, 10.0f});
                        bulletShape.setFillColor(sf::Color::Yellow);
                        graphics.addComponent(bullet, {bulletShape});
                        
                        velocities.addComponent(bullet, {direction * 400.0f});
                        bullets.addComponent(bullet, BulletComponent(false)); // Mark as enemy bullet
                    }
                }
            }
        }

        // Remove bullets and enemies
        for (Entity bullet : bulletsToRemove) {
            entityManager.destroyEntity(bullet);
            transforms.removeComponent(bullet);
            velocities.removeComponent(bullet);
            graphics.removeComponent(bullet);
            bullets.removeComponent(bullet);
        }
        bulletsToRemove.clear();

        for (Entity enemy : enemiesToRemove) {
    entityManager.destroyEntity(enemy);
    transforms.removeComponent(enemy);
    healths.removeComponent(enemy);
    graphics.removeComponent(enemy);
    shootings.removeComponent(enemy);
    flees.removeComponent(enemy);
    seekers.removeComponent(enemy);     
    melees.removeComponent(enemy);      
    stateMachines.removeComponent(enemy); 
}
        enemiesToRemove.clear();
    }

private:
    std::vector<Entity> bulletsToRemove;
    std::vector<Entity> enemiesToRemove;

    bool isColliding(const TransformComponent& a, const TransformComponent& b) {
        return (a.position.x < b.position.x + b.size.x &&
                a.position.x + a.size.x > b.position.x &&
                a.position.y < b.position.y + b.size.y &&
                a.position.y + a.size.y > b.position.y);
    }
};

class ControlsSystem {
public:
    void handleInput(float deltaTime, const sf::RenderWindow& window, Entity player,
                     ComponentManager<VelocityComponent>& velocities,
                     ComponentManager<TransformComponent>& transforms,
                     ComponentManager<GraphicsComponent>& graphics,
                     ComponentManager<BulletComponent>& bullets,
                     EntityManager& entityManager) {
        const float speed = 150.0f;

        auto velocity = velocities.getComponent(player);
        if (!velocity) return;

        velocity->velocity = {0, 0};

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
            if (!transform) return;

            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f target = window.mapPixelToCoords(mousePosition);

            Entity bullet = entityManager.createEntity();

            sf::Vector2f direction = target - transform->position;
            float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (magnitude != 0) {
                direction /= magnitude;
            }

            transforms.addComponent(bullet, {transform->position, {10.0f, 10.0f}});
            
            sf::RectangleShape bulletShape({10.0f, 10.0f});
            bulletShape.setFillColor(sf::Color::Red);
            graphics.addComponent(bullet, {bulletShape});
            
            velocities.addComponent(bullet, {direction * 600.0f});
            bullets.addComponent(bullet, BulletComponent(true)); // Mark as player bullet

            shootCooldown = 0.5f;
        }
    }
};

class RenderSystem {
public:
    void render(sf::RenderWindow& window, ComponentManager<TransformComponent>& transforms,
                ComponentManager<GraphicsComponent>& graphics) {
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

class MeleeSystem {
public:
    void update(float deltaTime, 
                ComponentManager<TransformComponent>& transforms,
                ComponentManager<MeleeComponent>& melees,
                ComponentManager<HealthComponent>& healths,
                ComponentManager<SeekComponent>& seekers,
                ComponentManager<StateMachineComponent>& stateMachines,
                ComponentManager<GraphicsComponent>& graphics,
                const sf::Vector2f& playerPosition,
                Entity player) {
    
        auto playerTransform = transforms.getComponent(player);
        auto playerHealth = healths.getComponent(player);
        if (!playerTransform || !playerHealth) return;

        std::cout << "DEBUG: Current player health: " << playerHealth->health << std::endl;

        for (const auto& pair : melees.getComponents()) {
            Entity entity = pair.first;
            auto melee = pair.second;
            auto transform = transforms.getComponent(entity);
            
            // Check if this is either a red enemy (with SeekComponent) or yellow enemy (with StateMachineComponent)
            bool isValidEnemy = (seekers.getComponent(entity) != nullptr || 
                               stateMachines.getComponent(entity) != nullptr);
            
            if (!transform || !isValidEnemy) continue;

            bool isColliding = (transform->position.x < playerTransform->position.x + playerTransform->size.x &&
                              transform->position.x + transform->size.x > playerTransform->position.x &&
                              transform->position.y < playerTransform->position.y + playerTransform->size.y &&
                              transform->position.y + transform->size.y > playerTransform->position.y);

            if (isColliding) {
                std::cout << "DEBUG: Collision detected with entity: " << entity << std::endl;
                melee->currentCooldown -= deltaTime;
                
                if (melee->currentCooldown <= 0) {
                    playerHealth->health = 0;
                    melee->currentCooldown = melee->attackCooldown;
                    std::cout << "DEBUG: Player killed by melee attack" << std::endl;
                    
                    // Remove player graphics on death
                    graphics.removeComponent(player);
                }
            }
        }
    }
};


class StateMachineSystem {
public:
    void update(float deltaTime,
                ComponentManager<TransformComponent>& transforms,
                ComponentManager<StateMachineComponent>& stateMachines,
                const sf::Vector2f& playerPosition) {
        
        for (const auto& pair : stateMachines.getComponents()) {
            Entity entity = pair.first;
            auto stateMachine = pair.second;
            auto transform = transforms.getComponent(entity);
            
            if (!transform) continue;

            float distance = std::sqrt(
                std::pow(playerPosition.x - transform->position.x, 2) +
                std::pow(playerPosition.y - transform->position.y, 2)
            );

            switch (stateMachine->currentState) {
                case StateMachineComponent::Idle:
                    if (distance < stateMachine->triggerRadius) {
                        stateMachine->currentState = StateMachineComponent::Seek;
                    }
                    break;

                case StateMachineComponent::Seek:
                    // Move towards player
                    if (distance > 0) {
                        sf::Vector2f direction = playerPosition - transform->position;
                        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                        if (magnitude != 0) {
                            direction /= magnitude;
                            transform->position += direction * stateMachine->speed * deltaTime;
                        }
                    }
                    break;
            }
        }
    }
};