#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "LevelOneScene.h"
#include <memory>
#include <map>

std::map<std::string, std::shared_ptr<Scene>> scenes;
std::shared_ptr<Scene> activeScene;

void loadScenes() {
	scenes["LevelOne"] = std::make_shared<LevelOneScene>();

	for (auto& pair : scenes) {
		const auto& name = pair.first;
		auto& scene = pair.second;
		scene->load();
	}
}

int main() {
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "StationOutbreak");
	Renderer::initialise(window);

	loadScenes();
	activeScene = scenes["LevelOne"];

	sf::Clock clock;
	bool isRunning = true;

	while (isRunning) {
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				isRunning = false;
				window.close();
			}
		}

		if (activeScene) {
			activeScene->update(deltaTime);
		}

		window.clear();

		if (activeScene) {
			activeScene->render();
		}
		Renderer::render();
		window.display();
	}

	Renderer::close();
	return 0;
}