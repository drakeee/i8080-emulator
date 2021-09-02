#pragma once

#include "Main.h"

class Emulator8080
{
public:
	struct FlagsStruct
	{
		bool S; //sign
		bool Z; //zero
		bool P; //parity
		bool C; //carry
		bool AC; //auxiliary carry - not used in Space Invaders
	};

	struct CPUStruct
	{
	public:

		//Registers
		uint8_t L;
		uint8_t H;
		uint8_t E;
		uint8_t D;
		uint8_t C;
		uint8_t B;
		uint8_t A;

		uint16_t* HL = (uint16_t*)&L; //pointer value would be => (h << 8) | l
		uint16_t* DE = (uint16_t*)&E; //pointer value would be => (d << 8) | e
		uint16_t* BC = (uint16_t*)&C; //pointer value would be => (b << 8) | c

		//Program counter and stack pointer
		uint16_t PC = 0; //program counter
		uint16_t SP = 0; //stack pointer

		//Flags
		FlagsStruct flags;
	};

	Emulator8080();
	Emulator8080(const char* fileName);

	void Load(const char* fileName);
	unsigned char* GetMemory(void) { return this->memory; }
	CPUStruct* GetCPU(void) { return &this->cpu; }

private:
	/*void setFunction(uint16_t value) { this->memory[*this->cpu.HL] = value; }
	uint16_t getFunction(void) { return this->memory[*this->cpu.HL]; }
	__declspec(property (put = setFunction, get = getFunction)) uint16_t m;*/

	void ProcessInstruction(void);
	void UnimplementedInstruction(void);
	uint16_t ConvertToNumber(unsigned char byte1, unsigned char byte2) { return (byte1 << 8) | byte2; }

	inline void PerformOr(uint16_t number)
	{
		uint16_t answer = (uint16_t)this->cpu.A | (uint16_t)number;
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
		uint16_t answer = (uint16_t)this->cpu.A ^ (uint16_t)number;
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
		uint16_t answer = (uint16_t)this->cpu.A & (uint16_t)number;
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
		uint16_t answer = (uint16_t)this->cpu.A + (uint16_t)number + (uint16_t)carry;
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
		uint16_t answer = (uint16_t)this->cpu.A - (uint16_t)number - (uint16_t)carry;
		/*this->cpu.flags.Z = ((answer & 0xFF) == 0);
		this->cpu.flags.S = (answer & 0x80);
		this->cpu.flags.C = (answer > 0xFF);
		this->cpu.flags.P = this->Parity<uint8_t>(answer & 0xFF);
		this->cpu.flags.AC = this->HalfCarry(answer, carry);*/
		this->CalculateFlags(answer);
		this->cpu.A = answer & 0xFF;
	}

	inline void CalculateFlags(uint16_t answer, bool zero = true, bool sign = true, bool carry = true, bool parity = true, bool auxc = true)
	{
		if(zero) this->cpu.flags.Z = ((answer & 0xFF) == 0);
		if(sign) this->cpu.flags.S = (answer & 0x80);
		if(carry) this->cpu.flags.C = (answer > 0xFF);
		if(parity) this->cpu.flags.P = this->Parity<uint8_t>(answer & 0xFF);
		if(auxc) this->cpu.flags.AC = this->HalfCarry(answer, carry);
	}

	template<typename T>
	inline bool Parity(T number)
	{
		std::bitset<sizeof T * 8> parityNumber = std::bitset<sizeof T * 8>(number);
		return parityNumber.count() % 2;
	}

	inline bool HalfCarry(uint32_t number, bool inCarryValue)
	{
		auto value = (this->cpu.A + number + inCarryValue) & 0xFF;
		return ((this->cpu.A ^ number ^ value) & 0x10);
	}

	CPUStruct cpu;
	unsigned char memory[64 * 1024];
};