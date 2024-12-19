#pragma once
#include "ComponentManager.h"
#include "EntityManager.h"
#include "Components.h"
#include "Renderer.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>

//movement system for updating entity position based on velocity and behavior
class MovementSystem {
public:
    //updates positions baased on velocity
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
    //update flee behavior for entities with flee component
    void updateFlee(float deltaTime, ComponentManager<TransformComponent>& transforms,
                    ComponentManager<FleeComponent>& flees, const sf::Vector2f& playerPosition) {
        for (const auto& pair : flees.getComponents()) {
            Entity entity = pair.first;
            auto flee = pair.second;
            auto transform = transforms.getComponent(entity);

            if (transform) {
                //calculate distance between player and entity
                float distance = std::sqrt(std::pow(playerPosition.x - transform->position.x, 2) +
                                           std::pow(playerPosition.y - transform->position.y, 2));
                if (distance < flee->fleeRadius) {
                    //move entity away from player
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
    //update seeking behavior for entities with seek component
    void updateSeek(float deltaTime, ComponentManager<TransformComponent>& transforms,
                    ComponentManager<SeekComponent>& seekers, const sf::Vector2f& playerPosition) {
        for (const auto& pair : seekers.getComponents()) {
            Entity entity = pair.first;
            auto seeker = pair.second;
            auto transform = transforms.getComponent(entity);

            if (transform) {
                //move entity towards player
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
//bullet system for handling bullets, collisions, and enemy shooting
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
                //check if bullet collides
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
                                if (shootings.getComponent(target) == nullptr) {  // If target is player
                                    entityManager.destroyEntity(target);  // Completely destroy player
                                    // Remove all components
                                    transforms.removeComponent(target);
                                    healths.removeComponent(target);
                                    velocities.removeComponent(target);
                                    graphics.removeComponent(target);
                                } else {  // If target is an enemy
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

                    // Different range for boss
                    float shootRange = transform->size.x > 100 ? 1000.0f : 600.0f;

                    if (distance < shootRange) {
                        sf::Vector2f direction = playerPosition - transform->position;
                        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                        if (magnitude != 0) {
                            direction /= magnitude;
                        }

                        Entity bullet = entityManager.createEntity();
                        
                        // Check if this is boss by size
                        bool isBoss = transform->size.x > 100;
                        float bulletSize = isBoss ? 30.0f : 10.0f;
                        float bulletSpeed = isBoss ? 100.0f : 400.0f;
                        
                        transforms.addComponent(bullet, {transform->position, {bulletSize, bulletSize}});
                        
                        sf::RectangleShape bulletShape({bulletSize, bulletSize});
                        bulletShape.setFillColor(isBoss ? sf::Color::Red : sf::Color::Yellow);
                        graphics.addComponent(bullet, {bulletShape});
                        
                        velocities.addComponent(bullet, {direction * bulletSpeed});
                        bullets.addComponent(bullet, BulletComponent(false));
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
    //checks if entities are colliding based on transform components
    bool isColliding(const TransformComponent& a, const TransformComponent& b) {
        return (a.position.x < b.position.x + b.size.x &&
                a.position.x + a.size.x > b.position.x &&
                a.position.y < b.position.y + b.size.y &&
                a.position.y + a.size.y > b.position.y);
    }
};
//handles player movement and shooting based on input
class ControlsSystem {
public:
    void handleInput(float deltaTime, const sf::RenderWindow& window, Entity player,
                     ComponentManager<VelocityComponent>& velocities,
                     ComponentManager<TransformComponent>& transforms,
                     ComponentManager<GraphicsComponent>& graphics,
                     ComponentManager<BulletComponent>& bullets,
                     EntityManager& entityManager) {
        //player movement speed
        const float speed = 150.0f;

        auto velocity = velocities.getComponent(player);
        if (!velocity) return;
        //reset velocity before getting inpiut
        velocity->velocity = {0, 0};
        //check for wasd presses and update velocity accordingly
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
        //handles shooting with left mouse
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootCooldown <= 0.0f) {
            auto transform = transforms.getComponent(player);
            if (!transform) return;
            //mouse position coordinates
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f target = window.mapPixelToCoords(mousePosition);
            //new bullet entity
            Entity bullet = entityManager.createEntity();
            //calculated direction of bullet
            sf::Vector2f direction = target - transform->position;
            float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (magnitude != 0) {
                direction /= magnitude;
            }
            //add components to bullet
            transforms.addComponent(bullet, {transform->position, {10.0f, 10.0f}});
            
            sf::RectangleShape bulletShape({10.0f, 10.0f});
            bulletShape.setFillColor(sf::Color::Red);
            graphics.addComponent(bullet, {bulletShape});
            
            velocities.addComponent(bullet, {direction * 600.0f});
            bullets.addComponent(bullet, BulletComponent(true)); // Mark as player bullet
            //set shoot cooldown
            shootCooldown = 0.5f;
        }
    }
};

class RenderSystem {
public:
    //renders all entities with transform component and graphics component
    void render(sf::RenderWindow& window, ComponentManager<TransformComponent>& transforms,
                ComponentManager<GraphicsComponent>& graphics) {
        for (const auto& pair : graphics.getComponents()) {
            Entity entity = pair.first;
            auto graphic = pair.second;
            //get entity transform component 
            auto transform = transforms.getComponent(entity);
            if (transform) {
                //update drawable's position and render it
                graphic->shape.setPosition(transform->position);
                window.draw(graphic->shape);
            }
        }
    }
};

class MeleeSystem {
public:
    //update melee attacks for entities with melee component
    void update(float deltaTime, 
                ComponentManager<TransformComponent>& transforms,
                ComponentManager<MeleeComponent>& melees,
                ComponentManager<HealthComponent>& healths,
                ComponentManager<SeekComponent>& seekers,
                ComponentManager<StateMachineComponent>& stateMachines,
                ComponentManager<GraphicsComponent>& graphics,
                ComponentManager<BulletComponent>& bullets,      // Add this
                ComponentManager<VelocityComponent>& velocities, // Add this
                EntityManager& entityManager,
                const sf::Vector2f& playerPosition,
                Entity player) {
        //get player transform and health
        auto playerTransform = transforms.getComponent(player);
        auto playerHealth = healths.getComponent(player);
        if (!playerTransform || !playerHealth || !graphics.getComponent(player)) return;

        for (const auto& pair : melees.getComponents()) {
            Entity entity = pair.first;
            auto melee = pair.second;
            auto transform = transforms.getComponent(entity);
            //check if enemy can be melee attacked
            bool isValidEnemy = (seekers.getComponent(entity) != nullptr || 
                               stateMachines.getComponent(entity) != nullptr);
            
            if (!transform || !isValidEnemy || !graphics.getComponent(entity)) continue;
            //check collision between enemy and player
            bool isColliding = (transform->position.x < playerTransform->position.x + playerTransform->size.x &&
                              transform->position.x + transform->size.x > playerTransform->position.x &&
                              transform->position.y < playerTransform->position.y + playerTransform->size.y &&
                              transform->position.y + transform->size.y > playerTransform->position.y);

            if (isColliding) {
                melee->currentCooldown -= deltaTime;
                //perform melee attack if cooldown over
                if (melee->currentCooldown <= 0) {
                    playerHealth->health = 0;
                    melee->currentCooldown = melee->attackCooldown;
                    
                    // Fully destroy player and all components
                    bullets.removeComponent(player);
                    transforms.removeComponent(player);
                    healths.removeComponent(player);
                    velocities.removeComponent(player);
                    graphics.removeComponent(player);
                    entityManager.destroyEntity(player);
                }
            }
        }
    }
};


class StateMachineSystem {
public:
    //updates state based behavior for entities with state machine
    void update(float deltaTime,
                ComponentManager<TransformComponent>& transforms,
                ComponentManager<StateMachineComponent>& stateMachines,
                const sf::Vector2f& playerPosition) {
        
        for (const auto& pair : stateMachines.getComponents()) {
            Entity entity = pair.first;
            auto stateMachine = pair.second;
            auto transform = transforms.getComponent(entity);
            
            if (!transform) continue;
            //calculate distance to player
            float distance = std::sqrt(
                std::pow(playerPosition.x - transform->position.x, 2) +
                std::pow(playerPosition.y - transform->position.y, 2)
            );
            //handle state transition
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