#include "Game.h"

#include <iostream>
#include "ResourceManager.h"
#include "CommonUtilities.h"
#include "Renderer/SpriteRenderer.h"

#define CHIP8_GRAPHIC_WIDTH 64
#define CHIP8_GRAPHIC_HEIGHT 32

Game::Game(GLuint width, GLuint height) 
	: screen({width, height}) {
	if (gameImplementation == nullptr) {
		gameImplementation = new GameImpl();
	}

	resMgr = ResourceManager::GetInstance();
}

Game::~Game() {
	delete gameImplementation;
}

void Game::init() {
	resMgr->loadShader(CommonUtilities::getFullPath("resource/shaders/sprite.vert").c_str(), CommonUtilities::getFullPath("resource/shaders/sprite.frag").c_str(), nullptr, "sprite");

	glm::mat4 projection = glm::ortho(0.0f, 64.0f, 32.0f, 0.0f, -1.0f, 1.0f);
	resMgr->getShader("sprite").setInteger("image", 0, GL_TRUE);
	resMgr->getShader("sprite").setMatrix4("projection", projection);

	this->spriteRenderer = new SpriteRenderer(resMgr->getShader("sprite"));

	pixelObject = new PixelObject(glm::vec2(64, 32));
}

void Game::processInput(GLfloat dt) {
}

void Game::update(GLfloat dt, GFXdata &gfx) {
	pixelObject->update(gfx);
}

void Game::render() {
	pixelObject->draw(*this->spriteRenderer);
}

void Game::clearBuffer() {
	ResourceManager::GetInstance()->clear();
	AudioEngine::GetInstance()->destroy();
}

// Accessors
Screen Game::getScreen() {
	return this->screen;
}

GameState Game::getState() {
	return this->state;
}

GLboolean Game::getKeys(GLuint key) {
	return this->keys[key];
}

GLboolean Game::getKeysProcessed(GLuint key) {
	return this->keysProcessed[key];
}

// Mutators
void Game::setScreen(GLuint width, GLuint height) {
	this->screen = { width, height };
}

void Game::setState(GameState state) {
	this->state = state;
}

void Game::setKeys(GLuint key, GLboolean isPress) {
	this->keys[key] = isPress;
}

void Game::setKeysProcessed(GLuint key, GLboolean isPress) {
	this->keysProcessed[key] = isPress;
}

GameImpl* Game::gameImplementation;