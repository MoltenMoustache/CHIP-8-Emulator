#include "chip8.h"
#include <fstream>

#define Vx (opcode & 0x0F00u) >> 8u
#define Vy (opcode & 0x00F0u) >> 4u
#define n (opcode & 0x000Fu)
#define nn (opcode & 0x00FFu)
#define nnn (opcode & 0x0FFFu)
#define Vf 0xF

#define height opcode & 0x000Fu

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START = 0x50;
uint8_t fontset[FONTSET_SIZE] =
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

chip8::chip8()
{
	// Sets the program counter to the memory address CHIP8 expects.
	programCounter = START_ADDRESS;

	// Puts the fontset into memory
	for (long i = 0; i < FONTSET_SIZE; i++)
		memory[FONTSET_START + i] = fontset[i];
}
chip8::~chip8()
{
	delete[] memory;
}

void chip8::LoadROM(const char* filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		// Get size of file and allocate a buffer to hold the contents
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		// Go to beginning of file and fill buffer
		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		// Load the ROM contents into CHIP-8's memory, starting at 0x200
		for (long i = 0; i < size; i++)
			memory[START_ADDRESS + i] = buffer[i];

		delete[] buffer;
	}
}

void chip8::EmulateCycle()
{
	Fetch();
	DecodeAndExecute();
}

void chip8::Fetch()
{
	// Read the current instruction, each instruction is two successive bytes.
	opcode = (memory[programCounter] << 8u) | memory[programCounter + 1];

	// Increment program counter by 2
	programCounter += 2;
}

void chip8::DecodeAndExecute()
{
	switch (opcode & 0xF000u)
	{
	case 0x0000u:
		break;
	case 0x1000u:
		OP_1nnn();
		break;
	case 0x2000u:
		OP_2nnn();
		break;
	case 0x3000u:
		OP_3xkk();
		break;
	case 0x4000u:
		OP_4xkk();
		break;
	case 0x5000u:
		OP_5xy0();
		break;
	case 0x6000u:
		OP_6xkk();
		break;
	case 0x7000u:
		OP_7xkk();
		break;
	case 0x8000u:
		switch (opcode & 0x000Fu)
		{
		case 0x0000u:
			OP_8xy0();
			break;
		case 0x00001u:
			OP_8xy1();
			break;
		case 0x0002u:
			OP_8xy2();
			break;
		case 0x0003u:
			OP_8xy3();
			break;
		case 0x0004u:
			OP_8xy4();
			break;
		case 0x0005u:
			OP_8xy5();
			break;
		case 0x0006u:
			OP_8xy6();
			break;
		case 0x0007u:
			OP_8xy7();
			break;
		case 0x000Eu:
			OP_8xyE();
			break;
		}
		break;
	case 0x9000u:
		OP_9xy0();
		break;
	case 0xA000u:
		OP_Annn();
		break;
	case 0xB000u:
		OP_Bnnn();
		break;
	case 0xC000u:
		OP_Cxkk();
		break;
	case 0xD000u:
		OP_Dxyn();
		break;
	case 0xE000u:
		if ((opcode & 0x00FFu) == 0x009Eu)
			OP_Ex9E();
		else if ((opcode & 0x00FFu) == 0x00A1u)
			OP_ExA1();
		break;
	case 0xF000u:
		switch (opcode & 0x00FFu)
		{
		case 0x0007:
			OP_Fx07();
			break;
		case 0x000A:
			OP_Fx0A();
			break;
		case 0x0015:
			OP_Fx15();
			break;
		case 0x0018:
			OP_Fx18();
			break;
		case 0x001E:
			OP_Fx1E();
			break;
		case 0x0029:
			OP_Fx29();
			break;
		case 0x0033:
			OP_Fx33();
			break;
		case 0x0055:
			OP_Fx55();
			break;
		case 0x0065:
			OP_Fx65();
			break;
		}
		break;

	default:
		break;
	}
}

#pragma region Instructions
// Jump to a machine code routine at nnn. (Deprecated by modern interpreters)
void chip8::OP_0nnn()
{

}

// Clear the display
void chip8::OP_00E0()
{
	memset(gfx, 0, 64 * 32);
}

// Return from a subroutine
void chip8::OP_00EE()
{
	programCounter = stack[0];
	stackPointer--;
}

// Jump to location nnn
void chip8::OP_1nnn()
{
	uint64_t address = opcode & 0x0FFFu;
	programCounter = address;
}

// Call subroutine at nnn
void chip8::OP_2nnn()
{
	// The interpreter increments the stack pointer, then puts the current PC on the top of the stack.The PC is then set to nnn.
	stack[stackPointer] = programCounter;
	stackPointer++;
	programCounter = nnn;
}

// Skips next instruction if Vx == kk
void chip8::OP_3xkk()
{
	//The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
	uint8_t byte = opcode & 0x00Fu;

	if (registers[Vx] == byte)
		programCounter += 2;
}

// Skip next instruction if Vx != kk
void chip8::OP_4xkk()
{
	//The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
	if (registers[Vx] == nn)
		programCounter += 2;
}

// Skip next instruction if Vx = Vy.
void chip8::OP_5xy0()
{
	//The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
	if (registers[Vx] == registers[Vy])
		programCounter += 2;
}

// Set Vx = kk
void chip8::OP_6xkk()
{
	// The interpreter puts the value kk into register Vx.
	registers[Vx] = nn;
}

// Set Vx = Vx + kk.
void chip8::OP_7xkk()
{
	// Adds the value kk to the value of register Vx, then stores the result in Vx.
	registers[Vx] += nn;
}

// Set Vx = Vy
void chip8::OP_8xy0()
{
	registers[Vx] = registers[Vy];
}

// Set Vx = Vx OR Vy.
void chip8::OP_8xy1()
{
	registers[Vx] |= registers[Vy];
}

// Set Vx = Vx AND Vy.
void chip8::OP_8xy2()
{
	registers[Vx] &= registers[Vy];
}

// Set Vx = Vx XOR Vy.
void chip8::OP_8xy3()
{
	// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. 
	// An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1.
	// Otherwise, it is 0.
	registers[Vx] ^= registers[Vy];
}

// Set Vx = Vx + Vy, set VF = carry.
void chip8::OP_8xy4()
{
	// The values of Vx and Vy are added together. 
	// If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
	uint16_t val = registers[Vx] + registers[Vy];
	if (val > 255)
		registers[Vf] = 1;
	else
		registers[Vf] = 0;

	registers[Vx] = val & 0x00FFu;
}

// Set Vx = Vx - Vy, set VF = NOT borrow.
void chip8::OP_8xy5()
{
	// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
	if (registers[Vx] > registers[Vy])
		registers[Vf] = 1;
	else
		registers[Vf] = 0;

	registers[Vx] -= registers[Vy];
}

// Set Vx = Vx SHR 1.
void chip8::OP_8xy6()
{
	// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
	if ((registers[Vx] & 0xFu) == 1U)
		registers[Vf] = 1;
	else
		registers[Vf] = 0;

	registers[Vx] >>= 1;
}

// Set Vx = Vy - Vx, set VF = NOT borrow.
void chip8::OP_8xy7()
{
	// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
	if (registers[Vy] > registers[Vx])
		registers[Vf] = 1;
	else
		registers[Vf] = 0;

	registers[Vx] = registers[Vy] - registers[Vx];
}

// Set Vx = Vx SHL 1.
void chip8::OP_8xyE()
{
	// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
	if ((registers[Vx] & 0xF000u) == 1)
		registers[Vf] = 1;
	else
		registers[Vf] = 0;

	registers[Vx] <<= 1;
}

// Skip next instruction if Vx != Vy.
void chip8::OP_9xy0()
{
	// The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
	if (registers[Vx] != registers[Vy])
		programCounter += 2;
}

// Set I = nnn.
void chip8::OP_Annn()
{
	// The value of register I is set to nnn.
	index = nnn;
}

// Jump to location nnn + V0.
void chip8::OP_Bnnn()
{
	// The program counter is set to nnn plus the value of V0.
	programCounter = registers[0] + nnn;
}

// Set Vx = random byte AND kk.
void chip8::OP_Cxkk()
{
	// The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.
	// TODO: Add rand()
}

// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
void chip8::OP_Dxyn()
{
	/*The interpreter reads n bytes from memory, starting at the address stored in I.
	These bytes are then displayed as sprites on screen at coordinates(Vx, Vy).
	Sprites are XORed onto the existing screen.If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
	If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
	See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip - 8 screen and sprites.
	*/
	int x = registers[Vx] % 64;
	int y = registers[Vy] % 32;
	registers[Vf] = 0;

	for (long col = 0; col < height; col++)
	{
		int8_t pixel = memory[index + col];
		// For each bit in the row
		for (int row = 0; row < 8; row++)
		{
			// Pixel is on
			if ((pixel & 0x1000u >> row) == 1)
			{
				// Pixel at x,y is on
				if (gfx[x + row + ((y + col) * 64)] == 1)
					registers[Vf] = 1;

				// XOR the result
				gfx[x + row + ((y + col) * 64)] ^= 1;
			}
		}
	}
}

// Display n - byte sprite starting at memory location I at(Vx, Vy), set VF = collision.
void chip8::OP_Ex9E()
{
	// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
	if (keys[Vx] == 0)
		programCounter += 2;
}

// Skip next instruction if key with the value of Vx is not pressed.
void chip8::OP_Ex9E()
{
	// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
	if (keys[Vx] == 1)
		programCounter += 2;
}

// Set Vx = delay timer value.
void chip8::OP_Ex9E()
{
	// The value of DT is placed into Vx.
	registers[Vx] = delayTimer;
}

// Wait for a key press, store the value of the key in Vx.
void chip8::OP_Ex9E()
{
	// All execution stops until a key is pressed, then the value of that key is stored in Vx.
}
#pragma endregion Instructions
