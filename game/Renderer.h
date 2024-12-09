#include <SFML/Graphics.hpp>
#pragma once

namespace Renderer {
	void initialise(sf::RenderWindow&);
	sf::RenderWindow& getWindow();

	void close();
	void update(const double&);
	void addDrawable(const sf::Drawable* drawable);
	void render();
};
