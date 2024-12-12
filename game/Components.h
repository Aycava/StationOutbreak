#pragma once
#include <SFML/Graphics.hpp>

//represents entity's position and size in game world
struct TransformComponent {
    sf::Vector2f position;
    sf::Vector2f size;
};
//represents entity's velocity
struct VelocityComponent {
    sf::Vector2f velocity;
};
//represents entity's health value
struct HealthComponent {
    int health;
};
//represents entity's graphical representation
struct GraphicsComponent {
    sf::RectangleShape shape;
};
//represents flee behavior for AI driven entity
struct FleeComponent {
    float speed;
    float fleeRadius;
};
//represents shooting behavior for entity
struct ShootingComponent {
    float shootInterval;
    float shootTimer;
};
//represents bullet-specific properties
struct BulletComponent {
    bool isPlayerBullet;
    BulletComponent(bool isPlayer) : isPlayerBullet(isPlayer) {}
};
//represents seek behavior for AI driven entity
struct SeekComponent {
    float speed;
    SeekComponent(float spd) : speed(spd) {}
};
//represents melee attack behavior for entity
struct MeleeComponent {
    float damage;
    float attackRadius;
    float attackCooldown;
    float currentCooldown;
    
    MeleeComponent(float dmg = 1.0f, float radius = 60.0f, float cooldown = 1.0f)
        : damage(dmg), attackRadius(radius), attackCooldown(cooldown), currentCooldown(0.0f) {}
};
//represents state machine behavior for AI driven entity
struct StateMachineComponent {
    enum State { Idle, Seek } currentState;
    float triggerRadius;
    float speed;

    StateMachineComponent(float radius, float spd)
        : currentState(Idle), triggerRadius(radius), speed(spd) {}
};


	
	