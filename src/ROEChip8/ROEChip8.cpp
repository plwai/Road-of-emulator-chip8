#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>

#include "ROEChip8.h"

ROEChip8::ROEChip8() {
	this->init();
	this->generateFuncTable();
}

ROEChip8::~ROEChip8() {

}

void ROEChip8::emulateCycle() {
	// Fetch Opcode
	this->opcode = this->memory[this->pc] << 8 | this->memory[this->pc + 1];

	// Decode
	(this->*chip8Table[(this->opcode & 0xF000) >> 12])();

	// Update Timer
	if (this->delayTimer > 0) {
		this->delayTimer--;
	}

	if (this->soundTimer > 0) {
		if (this->soundTimer == 1) {
			// Play Sound
		}
		this->soundTimer--;
	}
}

bool ROEChip8::loadROM(std::string romPath) {
	std::ifstream rom;

	rom.open(romPath.c_str(), std::ios::binary | std::ios::ate);
	
	if (rom.fail()) {
		rom.close();

		std::cout << "CHIP8::ERROR::Failed to load ROM" << std::endl;

		return false;
	}
	else {
		int fileSize = rom.tellg();
		char* buffer = new char[fileSize];

		// Rewind Position
		rom.clear();
		rom.seekg(0);

		rom.read(buffer, fileSize);

		for (int i = 0; i < fileSize; i++) {
			this->memory[i + 512] = buffer[i];
		}

		delete[] buffer;

		std::cout << "ROM Loaded" << std::endl;
	}

	rom.close();
	return true;
}

bool ROEChip8::getDrawFlag() const {
	return this->drawFlag;
}

void ROEChip8::setDrawFlag(bool drawFlag) {
	this->drawFlag = drawFlag;
}

void ROEChip8::setKeys() {

}

void ROEChip8::init() {
	unsigned char chip8_fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	// Reset and Initialize
	this->pc = 0x200;

	this->opcode = 0;
	this->drawFlag = false;
	this->I = 0;
	this->sp = 0;
	this->delayTimer = 0;
	this->soundTimer = 0;

	for (int i = 0; i < CHIP8_MEMORY_SIZE; i++) {
		this->memory[i] = 0;
	}

	for (int i = 0; i < CHIP8_GRAPHIC_HEIGHT * CHIP8_GRAPHIC_WIDTH; i++) {
		this->gfx[i] = 0;
	}

	for (int i = 0; i < CHIP8_KEYS; i++) {
		this->stack[i] = 0;
		this->key[i] = 0;
		this->V[i] = 0;
	}

	// Load Fontset
	for (int i = 0; i < 80; i++) {
		memory[i] = chip8_fontset[i];
	}

	// Initialize Seed
	srand(time(NULL));
}

void ROEChip8::generateFuncTable() {
	// Reset Function Pointer
	for (int i = 0; i < 16; i++) {
		this->chip8Table[i] = this->cpuNull;
		this->chip8TableArithmetic[i] = this->cpuNull;
	}

	// Main Chip8 Function Pointer Table
	this->chip8Table[0x0] = this->cpuProcessOpcode0;
	this->chip8Table[0x1] = this->cpuJumpToAddr;
	this->chip8Table[0x2] = this->cpuCallAddr;
	this->chip8Table[0x3] = this->cpuSkipEqualNN;
	this->chip8Table[0x4] = this->cpuSkipNotEqualNN;
	this->chip8Table[0x5] = this->cpuSkipEqualVY;
	this->chip8Table[0x6] = this->cpuSetVXNN;
	this->chip8Table[0x7] = this->cpuAddVXNN;
	this->chip8Table[0x8] = this->cpuArithmetic;
	this->chip8Table[0x9] = this->cpuSkipNotEqualVy;
	this->chip8Table[0xA] = this->cpuSetINNN;
	this->chip8Table[0xB] = this->cpuJumpToAddrPlusV0;
	this->chip8Table[0xC] = this->cpuRandomNum;
	this->chip8Table[0xD] = this->cpuDraw;
	this->chip8Table[0xE] = this->cpuProcessAlphaE;
	this->chip8Table[0xF] = this->cpuProcessAlphaF;

	// Chip8 Arithmetic Function Pointer Table
	this->chip8TableArithmetic[0x0] = this->cpuArithmeticAssign;
	this->chip8TableArithmetic[0x1] = this->cpuArithmeticOr;
	this->chip8TableArithmetic[0x2] = this->cpuArithmeticAnd;
	this->chip8TableArithmetic[0x3] = this->cpuArithmeticXor;
	this->chip8TableArithmetic[0x4] = this->cpuArithmeticAdd;
	this->chip8TableArithmetic[0x5] = this->cpuArithmeticMinusXY;
	this->chip8TableArithmetic[0x6] = this->cpuArithmeticShiftRight;
	this->chip8TableArithmetic[0x7] = this->cpuArithmeticMinusYX;
	this->chip8TableArithmetic[0xE] = this->cpuArithmeticShiftLeft;
}

void ROEChip8::nextProgramCounter() {
	this->pc += 2;
}

void ROEChip8::cpuNull() {
	// Error
	std::cout << "ROECHIP8::ERROR::Unknown Opcode ";
}

// Opcodes Operation

// 0x00E_
void ROEChip8::cpuProcessOpcode0() {
	switch (this->opcode & 0x000F) {
		case 0x0000:
			this->cpuClearScreen();
			break;

		case 0x000E:
			this->cpuRetSubroutine();
			break;

		default:
			this->cpuNull();
	}
}

// 0x00E0
void ROEChip8::cpuClearScreen() {
	for (int i = 0; i < CHIP8_GRAPHIC_WIDTH * CHIP8_GRAPHIC_HEIGHT; i++) {
		this->gfx[i] = 0;
	}

	this->nextProgramCounter();
}

// 0x00EE
void ROEChip8::cpuRetSubroutine() {
	this->sp--;
	this->pc = this->stack[this->sp];

	this->nextProgramCounter();
}

// 0x1NNN
void ROEChip8::cpuJumpToAddr() {
	this->pc = this->opcode & 0x0FFF;
}

// 0x2NNN
void ROEChip8::cpuCallAddr() {
	this->stack[this->sp] = this->pc;
	this->sp++;

	this->pc = this->opcode & 0x0FFF;
}

// 0x3XNN
void ROEChip8::cpuSkipEqualNN() {
	if (this->V[(this->opcode & 0x0F00) >> 8] == this->opcode & 0x00FF) {
		this->nextProgramCounter();
	}

	this->nextProgramCounter();
}

// 0x4XNN
void ROEChip8::cpuSkipNotEqualNN() {
	if (this->V[(this->opcode & 0x0F00) >> 8] != this->opcode & 0x00FF) {
		this->nextProgramCounter();
	}

	this->nextProgramCounter();
}

// 0x5XY0
void ROEChip8::cpuSkipEqualVY() {
	if (this->V[(this->opcode & 0x0F00) >> 8] == this->V[(this->opcode & 0x00F0) >> 4]) {
		this->nextProgramCounter();
	}

	this->nextProgramCounter();
}

// 0x6XNN
void ROEChip8::cpuSetVXNN() {
	this->V[(this->opcode & 0x0F00) >> 8] = this->opcode & 0x00FF;

	this->nextProgramCounter();
}

// 0x7XNN
void ROEChip8::cpuAddVXNN() {
	this->V[(this->opcode & 0x0F00) >> 8] += this->opcode & 0x00FF;

	this->nextProgramCounter();
}

// 0x8000
void ROEChip8::cpuArithmetic() {
	(this->*chip8TableArithmetic[this->opcode & 0x000F])();
}

// 0x8XY0
void ROEChip8::cpuArithmeticAssign() {
	this->V[(this->opcode & 0x0F00) >> 8] = this->V[(this->opcode & 0x00F0) >> 4];

	this->nextProgramCounter();
}

// 0x8XY1
void ROEChip8::cpuArithmeticOr() {
	this->V[(this->opcode & 0x0F00) >> 8] |= this->V[(this->opcode & 0x00F0) >> 4];

	this->nextProgramCounter();
}

// 0x8XY2
void ROEChip8::cpuArithmeticAnd() {
	this->V[(this->opcode & 0x0F00) >> 8] &= this->V[(this->opcode & 0x00F0) >> 4];

	this->nextProgramCounter();
}

// 0x8XY3
void ROEChip8::cpuArithmeticXor() {
	this->V[(this->opcode & 0x0F00) >> 8] ^= this->V[(this->opcode & 0x00F0) >> 4];

	this->nextProgramCounter();
}

// 0x8XY4
void ROEChip8::cpuArithmeticAdd() {
	this->V[(this->opcode & 0x0F00) >> 8] += this->V[(this->opcode & 0x00F0) >> 4];

	if (this->V[(this->opcode & 0x00F0) >> 4] > (0xFF - this->V[(this->opcode & 0x0F00) >> 8])) {
		this->V[0xF] = 1;
	}
	else {
		this->V[0xF] = 0;
	}

	this->nextProgramCounter();
}

// 0x8XY5
void ROEChip8::cpuArithmeticMinusXY() {
	this->V[(this->opcode & 0x0F00) >> 8] -= this->V[(this->opcode & 0x00F0) >> 4];

	if (this->V[(this->opcode & 0x0F00) >> 8] < this->V[(this->opcode & 0x00F0) >> 4]) {
		this->V[0xF] = 0;
	}
	else {
		this->V[0xF] = 1;
	}

	this->nextProgramCounter();
}

// 0x8XY6
void ROEChip8::cpuArithmeticShiftRight() {
	this->V[0xF] = this->V[(this->opcode & 0x00F0) >> 4] & 0x1;

	this->V[(this->opcode & 0x00F0) >> 4] = this->V[(this->opcode & 0x00F0) >> 4] >> 1;
	this->V[(this->opcode & 0x0F00) >> 8] = this->V[(this->opcode & 0x00F0) >> 4];

	this->nextProgramCounter();
}

// 0x8XY7
void ROEChip8::cpuArithmeticMinusYX() {
	this->V[(this->opcode & 0x0F00) >> 8] = this->V[(this->opcode & 0x00F0) >> 4] - this->V[(this->opcode & 0x0F00) >> 8];

	if (this->V[(this->opcode & 0x00F0) >> 4] < this->V[(this->opcode & 0x0F00) >> 8]) {
		this->V[0xF] = 0;
	}
	else {
		this->V[0xF] = 1;
	}

	this->nextProgramCounter();
}

// 0x8XYE
void ROEChip8::cpuArithmeticShiftLeft() {
	this->V[0xF] = this->V[(this->opcode & 0x00F0) >> 4] >> 7;

	this->V[(this->opcode & 0x00F0) >> 4] = this->V[(this->opcode & 0x00F0) >> 4] << 1;
	this->V[(this->opcode & 0x0F00) >> 8] = this->V[(this->opcode & 0x00F0) >> 4];

	this->nextProgramCounter();
}

// 0x9XY0
void ROEChip8::cpuSkipNotEqualVy() {
	if (this->V[(this->opcode & 0x0F00) >> 8] != this->V[(this->opcode & 0x00F0) >> 4]) {
		this->nextProgramCounter();
	}

	this->nextProgramCounter();
}

// 0xANNN
void ROEChip8::cpuSetINNN() {
	this->I = this->opcode & 0x0FFF;

	this->nextProgramCounter();
}

// 0xBNNN
void ROEChip8::cpuJumpToAddrPlusV0() {
	this->pc = this->V[0] + (this->opcode & 0x0FFF);

	this->nextProgramCounter();
}

// 0xCXNN
void ROEChip8::cpuRandomNum() {
	unsigned char randomNum = rand() % 0xFF + 1;

	this->V[(this->opcode & 0x0F00) >> 8] = randomNum & (this->opcode & 0x00FF);

	this->nextProgramCounter();
}

// 0xDXYN
void ROEChip8::cpuDraw() {
	unsigned short x = this->V[(this->opcode & 0x0F00) >> 8];
	unsigned short y = this->V[(this->opcode & 0x00F0) >> 4];
	unsigned short height = this->opcode & 0x000F;
	unsigned short pixel;

	this->V[0xF] = 0;

	for (int row = 0; row < height; row++) {
		pixel = this->memory[this->I + row];

		for (int col = 0; col < 8; col++) {
			if ((pixel & (0x80 >> col)) != 0) {
				if (this->gfx[(x + col + ((y + row) * 64))] == 1) {
					this->V[0xF] = 1;
				}

				this->gfx[x + col + ((y + row) * 64)] ^= 1;
			}
		}
	}

	this->setDrawFlag(true);
	this->nextProgramCounter();
}

// 0xEX__
void ROEChip8::cpuProcessAlphaE() {
	switch (this->opcode & 0x00FF) {
		case 0x009E:
			this->cpuSkipIfPress();
			break;

		case 0x00A1:
			this->cpuSkipIfNotPress();
			break;

		default:
			this->cpuNull();
	}
}

// 0xEX9E
void ROEChip8::cpuSkipIfPress() {
	if (this->key[this->V[(this->opcode & 0x0F00) >> 8]] != 0) {
		this->nextProgramCounter();
	}

	this->nextProgramCounter();
}

// 0xEXA1
void ROEChip8::cpuSkipIfNotPress() {
	if (this->key[this->V[(this->opcode & 0x0F00) >> 8]] == 0) {
		this->nextProgramCounter();
	}

	this->nextProgramCounter();
}

// 0xFX__
void ROEChip8::cpuProcessAlphaF() {
	switch (this->opcode & 0x00FF) {
		case 0x0007:
			this->cpuGetDelayTimer();
			break;

		case 0x000A:
			this->cpuStoreKey();
			break;

		case 0x0015:
			this->cpuSetDelayTimer();
			break;

		case 0x0018:
			this->cpuSetSoundTimer();
			break;

		case 0x001E:
			this->cpuAddVXToI();
			break;

		case 0x0029:
			this->cpuSpriteAddr();
			break;

		case 0x0033:
			this->cpuStoreBinary();
			break;

		case 0x0055:
			this->cpuStoreV0();
			break;

		case 0x0065:
			this->cpuFillV0();
			break;

		default:
			this->cpuNull();
	}
}

// 0xFX07
void ROEChip8::cpuGetDelayTimer() {
	this->V[(this->opcode & 0x0F00) >> 8] = this->delayTimer;

	this->nextProgramCounter();
}

// 0xFX0A
void ROEChip8::cpuStoreKey() {
	bool isKeyPressed = false;

	for (int i = 0; i < CHIP8_KEYS; i++) {
		if (this->key[i] != 0) {
			this->V[(this->opcode & 0x0F00) >> 8] = i;
			isKeyPressed = true;
		}
	}
	
	if (!isKeyPressed) {
		return;
	}

	this->nextProgramCounter();
}

// 0xFX15
void ROEChip8::cpuSetDelayTimer() {
	this->delayTimer = this->V[(this->opcode & 0x0F00) >> 8];

	this->nextProgramCounter();
}

// 0xFX18
void ROEChip8::cpuSetSoundTimer() {
	this->soundTimer = this->V[(this->opcode & 0x0F00) >> 8];

	this->nextProgramCounter();
}

// 0xFX1E
void ROEChip8::cpuAddVXToI() {
	this->I += this->V[(this->opcode & 0x0F00) >> 8];

	this->nextProgramCounter();
}

// 0xFX29
void ROEChip8::cpuSpriteAddr() {
	this->I = this->V[(this->opcode & 0x0F00) >> 8] * 0x5;

	this->nextProgramCounter();
}

// 0xFX33
void ROEChip8::cpuStoreBinary() {
	this->memory[this->I] = this->V[(this->opcode & 0x0F00) >> 8] >> 2;
	this->memory[this->I + 1] = (this->V[(this->opcode & 0x0F00) >> 8] >> 1) % 10;
	this->memory[this->I + 2] = this->V[(this->opcode & 0x0F00) >> 8] % 10;

	this->nextProgramCounter();
}

// 0xFX55
void ROEChip8::cpuStoreV0() {
	for (int i = 0; i <= ((this->opcode & 0x0F00) >> 8); ++i) {
		this->I++;
		this->memory[this->I] = this->V[i];
	}

	this->nextProgramCounter();
}

// 0xFX65
void ROEChip8::cpuFillV0() {
	for (int i = 0; i <= ((this->opcode & 0x0F00) >> 8); ++i) {
		this->I++;
		this->V[i] = this->memory[this->I];
	}

	this->nextProgramCounter();
}
