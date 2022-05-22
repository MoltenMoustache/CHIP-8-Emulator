#include <cstdint>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>

class chip8
{
public:
	int8_t gfx[64 * 32];

	void LoadROM(const char* filename);
	void EmulateCycle();

	chip8();
	~chip8();


private:
	int8_t memory[4096];
	int16_t programCounter;

	int16_t index;
	
	int16_t stack[16];
	int8_t stackPointer;
	
	int8_t delayTimer;
	int8_t soundTimer;

	int8_t registers[16];
	uint16_t opcode;

	int8_t keys[16];

	void Fetch();
	void DecodeAndExecute();

	// Instructions
	void OP_0nnn();
	void OP_00E0();
	void OP_00EE();
	void OP_1nnn();
	void OP_2nnn();
	void OP_3xkk();
	void OP_4xkk();
	void OP_5xy0();
	void OP_6xkk();
	void OP_7xkk();
	void OP_8xy0();
	void OP_8xy1();
	void OP_8xy2();
	void OP_8xy3();
	void OP_8xy4();
	void OP_8xy5();
	void OP_8xy6();
	void OP_8xy7();
	void OP_8xyE();
	void OP_9xy0();
	void OP_Annn();
	void OP_Bnnn();
	void OP_Cxkk();
	void OP_Dxyn();
	void OP_Ex9E();
	void OP_ExA1();
	void OP_Fx07();
	void OP_Fx0A();
	void OP_Fx15();
	void OP_Fx18();
	void OP_Fx1E();
	void OP_Fx29();
	void OP_Fx33();
	void OP_Fx55();
	void OP_Fx65();

	template< typename T >
	std::string int_to_hex(T i);
};

