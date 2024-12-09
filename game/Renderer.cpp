#include "Renderer.h"
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable *> sprites;
static RenderWindow *rw;

void Renderer::initialise(sf::RenderWindow &r) { rw = &r; }

sf::RenderWindow &Renderer::getWindow() { return *rw; }

void Renderer::close() {
	while (!sprites.empty())
		sprites.pop();
}

void Renderer::update (const double& deltaTime) {}

void Renderer::render() {
	if (rw == nullptr) {
		throw std::runtime_error("Render window not set");
	}
	while (!sprites.empty()) {
		rw->draw(*sprites.front());
		sprites.pop();
	}
	
}

void Renderer::addDrawable(const sf::Drawable* drawable) { 
	sprites.push(drawable); 
}
	
