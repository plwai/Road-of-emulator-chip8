#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <chrono>
#include <thread>

#include <iostream>

#include <Audio/AudioEngine.h>

#include <ROEChip8Config.h>

#include "ResourceManager.h"
#include "GraphicEngine\Game.h"
#include "ROEChip8\ROEChip8.h"

void keyCallBack(GLFWwindow* window, int key, int scanCode, int action, int mode);
void fpsCounter(int &nbFrames, double &lastTime);

const GLuint SCREEN_WIDTH = 1280;
const GLuint SCREEN_HEIGHT = 640;

Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
ROEChip8 chip8;
GFXdata gfx;

int main(int argc, char* argv[]) {
	// GLFW config
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ROEChip8", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Load Glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load glad" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, keyCallBack);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// V-sync 60fps
	//glfwSwapInterval(1);

	// Declaration for fps counter
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	// Initialize resource
	ResourceManager::GetInstance();

	// Initialize Game
	game.init();
	game.setState(GAME_ACTIVE);

	gfx.height = CHIP8_GRAPHIC_HEIGHT;
	gfx.width = CHIP8_GRAPHIC_WIDTH;
	unsigned char gfxOut[CHIP8_GRAPHIC_WIDTH * CHIP8_GRAPHIC_HEIGHT * 3] = {};

	// Initialize Audio
	AudioEngine::GetInstance()->loadAudio("./resource/audio/bleep.ogg", "bleep");
	chip8.initAudio();

	// Load ROM
	chip8.loadROM("./roms/INVADERS");
	glfwSwapBuffers(window);
	glEnable(GL_MULTISAMPLE);

	// Game Loop
	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//fpsCounter(nbFrames, lastTime);

		glfwPollEvents();

		chip8.emulateCycle();

		if (chip8.getDrawFlag()) {
			chip8.getGFX(gfxOut);
			gfx.gfx = gfxOut;
			game.update(deltaTime, gfx);

			// Clear buffer bit
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			game.render();
			chip8.setDrawFlag(false);
			glfwSwapBuffers(window);
		}
		
		std::this_thread::sleep_for(std::chrono::microseconds(2000));
	}

	// Clear resource
	game.clearBuffer();

	glfwTerminate();

	return 0;
}

void keyCallBack(GLFWwindow* window, int key, int scanCode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			chip8.setKeys(key, GL_TRUE);
		}
		else if (action == GLFW_RELEASE) {
			chip8.setKeys(key, GL_FALSE);
			chip8.setKeysProcessed(key, GL_FALSE);
		}
	}
}


void fpsCounter(int &nbFrames, double &lastTime) {
	nbFrames++;

	if (glfwGetTime() - lastTime >= 1.0) {
		printf("%f fps\n", double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}
