#ifndef GAMEIMPL_H
#define GAMEIMPL_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glfw/glfw3.h>

#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "ResourceManager.h"
#include "CommonUtilities.h"
#include "Renderer/SpriteRenderer.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
};

struct Screen {
	GLuint width;
	GLuint height;
};

class GameImpl {
	public:
		GameImpl();
		~GameImpl();

	private:
		ResourceManager* resMgr;
};

#endif // !GAMEIMPL_H