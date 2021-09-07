#pragma once

#include "Main.h"

class Emulator8080
{
public:
	struct FlagsStruct
	{
		bool S = false; //sign
		bool Z = false; //zero
		bool P = false; //parity
		bool C = false; //carry
		bool AC = false; //auxiliary carry - not used in Space Invaders
	};

	struct CPUStruct
	{
	public:

		uint8_t InPort[4] = { 0 };
		uint8_t OutPort[4] = { 0 };
		uint8_t shift1 = 0;
		uint8_t shift0 = 0;
		uint8_t shift_offset = 0;

		//Registers
		uint8_t L = 0;
		uint8_t H = 0;
		uint8_t E = 0;
		uint8_t D = 0;
		uint8_t C = 0;
		uint8_t B = 0;
		uint8_t A = 0;

		uint16_t* HL = (uint16_t*)&L; //pointer value would be => (h << 8) | l
		uint16_t* DE = (uint16_t*)&E; //pointer value would be => (d << 8) | e
		uint16_t* BC = (uint16_t*)&C; //pointer value would be => (b << 8) | c

		//Program counter and stack pointer
		uint16_t PC = 0; //program counter
		uint16_t SP = 0; //stack pointer - 2300 is the lowest stack region

		uint32_t cycles = 0;

		bool interrupt_enabled = false;

		//Flags
		FlagsStruct flags;
	};

	Emulator8080();
	Emulator8080(const char* fileName, int startOffset = 0);

	void Load(const char* fileName, int startOffset = 0);
	unsigned char* GetMemory(void) { return &this->memory[0]; }
	unsigned char* GetRAM(void) { return &this->memory[0x2000]; }
	unsigned char* GetVRAM(void) { return &this->memory[0x2400]; }
	CPUStruct* GetCPU(void) { return &this->cpu; }
	inline uint16_t GetPSW(void)
	{
		std::bitset<8> psw;

		psw.set(0, this->cpu.flags.C);
		psw.set(1, true);
		psw.set(2, this->cpu.flags.P);
		psw.set(3, false);
		psw.set(4, this->cpu.flags.AC);
		psw.set(5, false);
		psw.set(6, this->cpu.flags.Z);
		psw.set(7, this->cpu.flags.S);

		return (uint16_t)psw.to_ulong();
	}

	uint8_t ProcessInstruction();
	void GenerateInterrupt(int interrupt_num);

	const uint16_t GameWidth = 224;
	const uint16_t GameHeight = 256;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	TTF_Font* font = nullptr;

private:
	void UnimplementedInstruction(void);
	uint16_t ConvertToNumber(unsigned char high, unsigned char low)
	{
		uint16_t res = (high << 8) | low;
		//if (res > 0x3FFF)
			//printf("Number higher than 0x3FFF: %d\n", res);

		return res;
	}

	inline void PerformOr(uint16_t number)
	{
		uint16_t answer = this->cpu.A | number;
		this->CalculateFlags(answer, true, true, false, true, false);
		/*this->cpu.flags.Z = ((answer & 0xFF) == 0);
		this->cpu.flags.S = (answer & 0x80);
		this->cpu.flags.P = this->Parity<uint8_t>(answer & 0xFF);*/
		this->cpu.flags.C = false;
		this->cpu.flags.AC = false;
		this->cpu.A = answer & 0xFF;
	}

	inline void PerformXor(uint16_t number)
	{
		uint16_t answer = this->cpu.A ^ number;
		this->CalculateFlags(answer, true, true, false, true, false);
		/*this->cpu.flags.Z = ((answer & 0xFF) == 0);
		this->cpu.flags.S = (answer & 0x80);
		this->cpu.flags.P = this->Parity<uint8_t>(answer & 0xFF);*/
		this->cpu.flags.C = false;
		this->cpu.flags.AC = false;
		this->cpu.A = answer & 0xFF;
	}

	inline void PerformAnd(uint16_t number)
	{
		uint16_t answer = this->cpu.A & number;
		this->CalculateFlags(answer, true, true, false, true, false);
		/*this->cpu.flags.Z = ((answer & 0xFF) == 0);
		this->cpu.flags.S = (answer & 0x80);
		this->cpu.flags.P = this->Parity<uint8_t>(answer & 0xFF);*/
		this->cpu.flags.C = false;
		this->cpu.flags.AC = false;
		this->cpu.A = answer & 0xFF;
	}

	inline void PerformAdd(uint16_t number, bool carry = false)
	{
		uint16_t answer = this->cpu.A + number + carry;
		/*this->cpu.flags.Z = ((answer & 0xFF) == 0);
		this->cpu.flags.S = (answer & 0x80);
		this->cpu.flags.C = (answer > 0xFF);
		this->cpu.flags.P = this->Parity<uint8_t>(answer & 0xFF);
		this->cpu.flags.AC = this->HalfCarry(answer, carry);*/
		this->CalculateFlags(answer);
		this->cpu.A = answer & 0xFF;
	}

	inline void PerformSub(uint16_t number, bool carry = false)
	{
		uint16_t answer = this->cpu.A - number - carry;
		/*this->cpu.flags.Z = ((answer & 0xFF) == 0);
		this->cpu.flags.S = (answer & 0x80);
		this->cpu.flags.C = (answer > 0xFF);
		this->cpu.flags.P = this->Parity<uint8_t>(answer & 0xFF);
		this->cpu.flags.AC = this->HalfCarry(answer, carry);*/
		this->CalculateFlags(answer);
		this->cpu.A = answer & 0xFF;
	}

	inline uint8_t ReadMemory(uint16_t addr)
	{
		return this->memory[addr];
	}

	inline void WriteMemory(uint16_t addr, uint8_t value)
	{
		this->memory[addr] = value;
	}

	inline void Push(uint16_t value)
	{
		this->cpu.SP -= 2;
		*(uint16_t*)&this->memory[this->cpu.SP] = value;

		//uint8_t hi, lo;
		//hi = (value >> 8) & 0xff;
		//lo = value & 0xff;

		//WriteMemory(this->cpu.SP + 1, hi);
		//WriteMemory(this->cpu.SP, lo);
	}

	inline uint16_t Pop()
	{
		uint16_t ret = *(uint16_t*)&this->memory[this->cpu.SP];
		this->cpu.SP += 2;
		return ret;
	}

	inline void CalculateFlags(uint16_t answer, bool zero = true, bool sign = true, bool carry = true, bool parity = true, bool auxc = true)
	{
		if(zero) this->cpu.flags.Z = ((answer & 0xFF) == 0);
		if(sign) this->cpu.flags.S = (answer & 0xff) >> 7;
		if(carry) this->cpu.flags.C = (answer > 0xFF);
		if(parity) this->cpu.flags.P = this->Parity<uint8_t>(answer & 0xFF);
		if(auxc) this->cpu.flags.AC = this->HalfCarry(answer, carry);
	}

	template<typename T>
	inline bool Parity(T number)
	{
		std::bitset<sizeof T * 8> parityNumber = std::bitset<sizeof T * 8>(number);
		return (bool)((parityNumber.count() % 2) == 0);
	}

	inline bool HalfCarry(uint32_t number, bool inCarryValue)
	{
		auto value = (this->cpu.A + number + inCarryValue) & 0xFF;
		return ((this->cpu.A ^ number ^ value) & 0x10);
	}

	CPUStruct cpu;
	unsigned char memory[64 * 1024] = { 0 };
};

int Disassemble8080(Emulator8080* emulator, int num);