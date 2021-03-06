#ifndef ROECHIP8_H
#define ROECHIP8_H

#include <string>
#include <map>
#include <GLFW/glfw3.h>

// Configuration
#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_GRAPHIC_WIDTH 64
#define CHIP8_GRAPHIC_HEIGHT 32
#define CHIP8_CPU_REGISTERS 16
#define CHIP8_STACK_SIZE 16
#define CHIP8_KEYS 16


class ROEChip8 {
	public:
		ROEChip8();
		~ROEChip8();

		void emulateCycle();
		bool loadROM(std::string romPath);
		void initAudio();

		// Accessors
		bool getDrawFlag() const;
		void getGFX(unsigned char* gfxOut);

		// Mutators
		void setDrawFlag(bool drawFlag);
		void setKeys(int key, GLboolean isPress);
		void setKeysProcessed(int key, GLboolean isPress);

	private:
		// CHIP8 Specification BEGIN
		unsigned short opcode;
		unsigned short I; // Index Register
		unsigned short pc; // Program Counter 0x000 to 0xFFF
		unsigned short sp; // Stack Pointer
		unsigned short stack[CHIP8_STACK_SIZE]; // Memory Stack
		unsigned char delayTimer;
		unsigned char soundTimer;
		unsigned char memory[CHIP8_MEMORY_SIZE]; // Memory up to 4k
		unsigned char V[CHIP8_CPU_REGISTERS]; // CPU registers V0 to VE
		unsigned char gfx[CHIP8_GRAPHIC_WIDTH * CHIP8_GRAPHIC_HEIGHT]; // Graphic Pixel Screen
		unsigned char key[CHIP8_KEYS];
		// CHIP8 Specification END

		bool drawFlag;
		std::map<int, int> keyMap;
		Audio bleepAudio;

		void init();
		void generateFuncTable();
		void generateKeyMap();
		void nextProgramCounter();

		// Opcode Process
		void cpuNull();

		void cpuProcessOpcode0();
		void cpuClearScreen();
		void cpuRetSubroutine();

		void cpuJumpToAddr();
		void cpuCallAddr();
		void cpuSkipEqualNN();
		void cpuSkipNotEqualNN();
		void cpuSkipEqualVY();
		void cpuSetVXNN();
		void cpuAddVXNN();

		void cpuArithmetic();
		void cpuArithmeticAssign();
		void cpuArithmeticOr();
		void cpuArithmeticAnd();
		void cpuArithmeticXor();
		void cpuArithmeticAdd();
		void cpuArithmeticMinusXY();
		void cpuArithmeticShiftRight();
		void cpuArithmeticMinusYX();
		void cpuArithmeticShiftLeft();

		void cpuSkipNotEqualVy();
		void cpuSetINNN();
		void cpuJumpToAddrPlusV0();
		void cpuRandomNum();
		void cpuDraw();

		void cpuProcessAlphaE();
		void cpuSkipIfPress();
		void cpuSkipIfNotPress();

		void cpuProcessAlphaF();
		void cpuGetDelayTimer();
		void cpuStoreKey();
		void cpuSetDelayTimer();
		void cpuSetSoundTimer();
		void cpuAddVXToI();
		void cpuSpriteAddr();
		void cpuStoreBinary();
		void cpuStoreV0();
		void cpuFillV0();

		// Function Table
		void (ROEChip8::*chip8Table[16])();
		void (ROEChip8::*chip8TableArithmetic[16])();
};

#endif // !ROECHIP8_H
