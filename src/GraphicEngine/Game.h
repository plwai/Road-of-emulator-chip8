#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw/glfw3.h>

#include "GameImpl.h"
#include "Audio/AudioEngine.h"
#include "PixelObject.h"

class Game {
	public:
		Game(GLuint width, GLuint height);
		~Game();
		void init();
		void processInput(GLfloat dt);
		void update(GLfloat dt, GFXdata &gfx);
		void render();
		void clearBuffer();

		// Accessors
		Screen getScreen();
		GameState getState();
		GLboolean getKeys(GLuint key);
		GLboolean getKeysProcessed(GLuint key);
		
		// Mutators
		void setScreen(GLuint width, GLuint height);
		void setState(GameState state);
		void setKeys(GLuint key, GLboolean isPress);
		void setKeysProcessed(GLuint key, GLboolean isPress);

	private:
		GameState state;
		GLboolean mappedKeys[16];
		GLboolean keys[1024];
		GLboolean keysProcessed[1024];
		Screen screen;

		ResourceManager* resMgr;
		PixelObject* pixelObject;
		Renderer2D* spriteRenderer;
		Audio* BGM;
		static GameImpl* gameImplementation;
};

#endif // !GAME_H
