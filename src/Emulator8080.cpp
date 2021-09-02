#include "Main.h"

Emulator8080::Emulator8080()
{
	
}

Emulator8080::Emulator8080(const char* fileName)
{
	this->Load(fileName);
}

void Emulator8080::Load(const char* fileName)
{
	FILE* rom = fopen(fileName, "rb");
	if (rom == nullptr)
	{
		Utils::PrintError("Unable to open \"%s\"", fileName);
		return;
	}

	fseek(rom, 0, SEEK_END);
	int fileSize = ftell(rom);
	fseek(rom, 0, SEEK_SET);

	fread((void*)this->memory, 1, fileSize, rom);
}

void Emulator8080::ProcessInstruction()
{
	unsigned char* opcode = &this->memory[this->cpu.PC];

	this->cpu.PC += 1;

	switch (*opcode)
	{
		//NOP
		case 0x00:
		{
			this->UnimplementedInstruction();
			break;
		}

		//LXI B,D16
		case 0x01:
		{

			//this->cpu.B = opcode[2];
			//this->cpu.C = opcode[1];

			*this->cpu.BC = this->ConvertToNumber(opcode[2], opcode[1]);
			this->cpu.PC += 2;

			break;
		}

		//STAX B
		case 0x02:
		{
			this->memory[*this->cpu.BC] = this->cpu.A;
			break;
		}

		//INX B
		case 0x03:
		{
			(*this->cpu.BC)++;
			break;
		}

		//INR B
		case 0x04:
		{
			this->cpu.B++;
			this->CalculateFlags(this->cpu.B, true, true, false, true, true);
			break;
		}

		//DCR B
		case 0x05:
		{
			this->cpu.B--;
			this->CalculateFlags(this->cpu.B, true, true, false, true, true);
			break;
		}

		//MVI B, D8
		case 0x06:
		{
			this->cpu.B = opcode[1];
			this->cpu.PC += 1;
			break;
		}

		//RLC
		case 0x07:
		{
			this->cpu.flags.C = (this->cpu.A & (1 << 7));
			this->cpu.A = Utils::LeftRotate(this->cpu.A, 1);
			break;
		}

		//NOP
		case 0x08:
		{
			//this->UnimplementedInstruction();
			break;
		}

		//DAD B
		case 0x09:
		{
			this->cpu.flags.C = ((*this->cpu.HL) + (*this->cpu.BC)) > 0xFFFF;
			*this->cpu.HL += *this->cpu.BC;
			break;
		}

		//LDAX B
		case 0x0a:
		{
			this->cpu.A = this->memory[*this->cpu.BC];
			break;
		}

		//DCX B
		case 0x0b:
		{
			(*this->cpu.BC)--;
			break;
		}

		//INR C
		case 0x0c:
		{
			this->cpu.C++;
			this->CalculateFlags(this->cpu.C, true, true, false, true, true);
			break;
		}

		//DCR C
		case 0x0d:
		{
			this->cpu.C--;
			this->CalculateFlags(this->cpu.C, true, true, false, true, true);
			break;
		}

		//MVI C,D8
		case 0x0e:
		{
			this->cpu.C = opcode[1];
			this->cpu.PC += 1;
			break;
		}

		//RRC
		case 0x0f:
		{
			this->cpu.flags.C = this->cpu.A & 1;
			this->cpu.A = Utils::RightRotate(this->cpu.A, 1);
			break;
		}

		//NOP
		case 0x10:
		{
			//this->UnimplementedInstruction();
			break;
		}

		//LXI D,D16
		case 0x11:
		{
			*this->cpu.DE = this->ConvertToNumber(opcode[2], opcode[1]);
			this->cpu.PC += 2;
			break;
		}

		//STAX D
		case 0x12:
		{
			*this->cpu.DE = this->cpu.A;
			break;
		}

		//INX D
		case 0x13:
		{
			(*this->cpu.DE)++;
			break;
		}

		//INR D
		case 0x14:
		{
			this->cpu.D++;
			this->CalculateFlags(this->cpu.B, true, true, false, true, true);
			break;
		}

		//DCR D
		case 0x15:
		{
			this->cpu.D--;
			this->CalculateFlags(this->cpu.B, true, true, false, true, true);
			break;
		}

		//MVI D, D8
		case 0x16:
		{
			this->cpu.D = opcode[1];
			this->cpu.PC += 1;
			break;
		}

		//RAL
		case 0x17:
		{
			uint8_t temp = this->cpu.flags.C; //store carry flag state
			this->cpu.flags.C = this->cpu.A & (1 << 7); //fill the carry with MSB
			this->cpu.A = Utils::LeftRotate(this->cpu.A, 1); //circular rotate to the left
			this->cpu.A &= ~1; //set LSB to 0
			this->cpu.A |= temp; //set LSB according to carry
			break;
		}

		//NOP
		case 0x18:
		{
			//this->UnimplementedInstruction();
			break;
		}

		//DAD D
		case 0x19:
		{
			this->cpu.flags.C = ((*this->cpu.HL) + (*this->cpu.DE)) > 0xFFFF;
			*this->cpu.HL += *this->cpu.DE;
			break;
		}

		//LDAX D
		case 0x1a:
		{
			this->cpu.A = this->memory[*this->cpu.DE];
			break;
		}

		//DCX D
		case 0x1b:
		{
			(*this->cpu.DE)--;
			break;
		}

		//INR E
		case 0x1c:
		{
			this->cpu.E++;
			this->CalculateFlags(this->cpu.B, true, true, false, true, true);
			break;
		}

		//DCR E
		case 0x1d:
		{
			this->cpu.E--;
			this->CalculateFlags(this->cpu.B, true, true, false, true, true);
			break;
		}

		//MVI E,D8
		case 0x1e:
		{
			this->cpu.E = opcode[1];
			this->cpu.PC += 1;
			break;
		}

		//RAR
		case 0x1f:
		{
			uint8_t temp = this->cpu.flags.C; //store carry flag state
			this->cpu.flags.C = this->cpu.A & 1; //fill the carry with LSB
			this->cpu.A = Utils::RightRotate(this->cpu.A, 1); //circular rotate to the right
			this->cpu.A &= ~(1 << 7); //set MSB to 0
			this->cpu.A |= temp << 7; //set MSB according to carry
			break;
		}

		//NOP
		case 0x20:
		{
			//this->UnimplementedInstruction();
			break;
		}

		//LXI H,D16
		case 0x21:
		{
			*this->cpu.HL = this->ConvertToNumber(opcode[2], opcode[1]);
			this->cpu.PC += 2;
			break;
		}

		//SHLD adr
		case 0x22:
		{
			*(uint16_t*)&this->memory[this->ConvertToNumber(opcode[2], opcode[1])] = *this->cpu.HL;
			this->cpu.PC += 2;
			break;
		}

		//INX H
		case 0x23:
		{
			(*this->cpu.HL)++;
			break;
		}

		//INR H
		case 0x24:
		{
			this->cpu.H++;
			this->CalculateFlags(this->cpu.H, true, true, false, true, false);
			break;
		}

		//DCR H
		case 0x25:
		{
			this->cpu.H--;
			this->CalculateFlags(this->cpu.H, true, true, false, true, false);
			break;
		}

		//MVI H,D8
		case 0x26:
		{
			this->cpu.H = opcode[1];
			this->cpu.PC += 1;
			break;
		}

		//DAA
		case 0x27:
		{
			uint8_t msb = this->cpu.A >> 4;
			uint8_t lsb = this->cpu.A & 0xF;

			if (lsb > 9 || this->cpu.flags.AC)
				this->cpu.A += 0x06; //adds six to the accumulator

			if (msb > 9 || this->cpu.flags.C)
				this->cpu.A += 0x60; //adds six to the most significant 4 bits

			if (lsb + 0x06 > 0xF)
				this->cpu.flags.AC = true;

			if (msb + 0x60 > 0xF)
				this->cpu.flags.C = true;

			this->CalculateFlags(this->cpu.A, true, true, false, true, false);

			break;
		}

		//NOP
		case 0x28:
		{
			//this->UnimplementedInstruction();
			break;
		}

		//DAD H
		case 0x29:
		{
			this->cpu.flags.C = ((*this->cpu.HL) + (*this->cpu.HL)) > 0xFFFF;
			(*this->cpu.HL) += (*this->cpu.HL);
			break;
		}

		//LHLD adr
		case 0x2a:
		{
			*this->cpu.HL = *(uint16_t*)this->memory[this->ConvertToNumber(opcode[2], opcode[1])];
			this->cpu.PC += 2;

			break;
		}

		//DCX H
		case 0x2b:
		{
			(*this->cpu.HL)--;
			break;
		}

		//INR L
		case 0x2c:
		{
			this->cpu.L++;
			this->CalculateFlags(this->cpu.B, true, true, false, true, true);
			break;
		}

		//DCR L
		case 0x2d:
		{
			this->cpu.L--;
			this->CalculateFlags(this->cpu.B, true, true, false, true, true);
			break;
		}

		//MVI L, D8
		case 0x2e:
		{
			this->cpu.L = opcode[1];
			this->cpu.PC += 1;
			break;
		}

		//CMA
		case 0x2f:
		{
			this->cpu.A = ~this->cpu.A;
			break;
		}

		//NOP
		case 0x30:
		{
			//this->UnimplementedInstruction();
			break;
		}

		//LXI SP, D16
		case 0x31:
		{
			this->cpu.SP = this->ConvertToNumber(opcode[2], opcode[1]);
			this->cpu.PC += 2;
			break;
		}

		//STA adr
		case 0x32:
		{
			this->memory[this->ConvertToNumber(opcode[2], opcode[1])] = this->cpu.A;
			this->cpu.PC += 2;
			break;
		}

		//INX SP
		case 0x33:
		{
			this->cpu.SP++;
			break;
		}

		//INR M
		case 0x34:
		{
			this->memory[*this->cpu.HL]++;
			this->CalculateFlags(*this->cpu.HL, true, true, false, true, false);

			break;
		}

		//DCR M
		case 0x35:
		{
			this->memory[*this->cpu.HL]--;
			this->CalculateFlags(*this->cpu.HL, true, true, false, true, false);

			break;
		}

		//MVI M,D8
		case 0x36:
		{
			this->memory[*this->cpu.HL] = opcode[1];
			this->cpu.PC += 1;
			break;
		}

		//STC
		case 0x37:
		{
			this->cpu.C = true;
			break;
		}

		//NOP
		case 0x38:
		{
			//this->UnimplementedInstruction();
			break;
		}

		//DAD SP
		case 0x39:
		{
			this->cpu.flags.C = ((*this->cpu.HL) + (this->cpu.SP)) > 0xFFFF;
			(*this->cpu.HL) += this->cpu.SP;
			break;
		}

		//LDA adr
		case 0x3a:
		{
			this->cpu.A = this->memory[this->ConvertToNumber(opcode[2], opcode[1])];
			this->cpu.PC += 2;
			break;
		}

		//DCX SP
		case 0x3b:
		{
			this->cpu.SP--;
			break;
		}

		//INR A
		case 0x3c:
		{
			this->cpu.A++;
			this->CalculateFlags(this->cpu.A, true, true, false, true, false);
			break;
		}

		//DCR A
		case 0x3d:
		{
			this->cpu.A--;
			this->CalculateFlags(this->cpu.A, true, true, false, true, false);
			break;
		}

		//MVI A,D8
		case 0x3e:
		{
			this->cpu.A = opcode[1];
			this->cpu.PC += 1;
			break;
		}

		//CMC
		case 0x3f:
		{
			this->cpu.flags.C = !this->cpu.flags.C;
			break;
		}

		//MOV B,B
		case 0x40:
		{
			this->cpu.B = this->cpu.B;
			break;
		}

		//MOV B,C
		case 0x41:
		{
			this->cpu.B = this->cpu.C;
			break;
		}

		//MOV B,D
		case 0x42:
		{
			this->cpu.B = this->cpu.D;
			break;
		}

		//MOV B,E
		case 0x43:
		{
			this->cpu.B = this->cpu.E;
			break;
		}

		//MOV B,H
		case 0x44:
		{
			this->cpu.B = this->cpu.H;
			break;
		}

		//MOV B,L
		case 0x45:
		{
			this->cpu.B = this->cpu.L;
			break;
		}

		//MOV B,M
		case 0x46:
		{
			//this->cpu.B = this->memory[this->ConvertToNumber(this->cpu.H, this->cpu.L)];
			this->cpu.B = this->memory[*this->cpu.HL];
			break;
		}

		//MOV B,A
		case 0x47:
		{
			this->cpu.B = this->cpu.A;
			break;
		}

		//MOV C,B
		case 0x48:
		{
			this->cpu.C = this->cpu.B;
			break;
		}

		//MOV C,C
		case 0x49:
		{
			this->cpu.C = this->cpu.C;
			break;
		}

		//MOV C,D
		case 0x4a:
		{
			this->cpu.C = this->cpu.D;
			break;
		}

		//MOV C,E
		case 0x4b:
		{
			this->cpu.C = this->cpu.E;
			break;
		}

		//MOV C,H
		case 0x4c:
		{
			this->cpu.C = this->cpu.H;
			break;
		}

		//MOV C,L
		case 0x4d:
		{
			this->cpu.C = this->cpu.L;
			break;
		}

		//MOV C,M
		case 0x4e:
		{
			//this->cpu.C = this->memory[this->ConvertToNumber(this->cpu.H, this->cpu.L)];
			this->cpu.C = this->memory[*this->cpu.HL];
			break;
		}

		//MOV C,A
		case 0x4f:
		{
			this->cpu.C = this->cpu.A;
			break;
		}

		//MOV D,B
		case 0x50:
		{
			this->cpu.D = this->cpu.B;
			break;
		}

		//MOV D,C
		case 0x51:
		{
			this->cpu.D = this->cpu.C;
			break;
		}

		//MOV D,D
		case 0x52:
		{
			this->cpu.D = this->cpu.D;
			break;
		}

		//MOV D,E
		case 0x53:
		{
			this->cpu.D = this->cpu.E;
			break;
		}

		//MOV D,H
		case 0x54:
		{
			this->cpu.D = this->cpu.H;
			break;
		}

		//MOV D,L
		case 0x55:
		{
			this->cpu.D = this->cpu.L;
			break;
		}

		//MOV D,M
		case 0x56:
		{
			//this->cpu.D = this->memory[this->ConvertToNumber(this->cpu.H, this->cpu.L)];
			this->cpu.D = this->memory[*this->cpu.HL];
			break;
		}

		//MOV D,A
		case 0x57:
		{
			this->cpu.D = this->cpu.A;
			break;
		}

		//MOV E,B
		case 0x58:
		{
			this->cpu.E = this->cpu.B;
			break;
		}

		//MOV E,C
		case 0x59:
		{
			this->cpu.E = this->cpu.C;
			break;
		}

		//MOV E,D
		case 0x5a:
		{
			this->cpu.E = this->cpu.D;
			break;
		}

		//MOV E,E
		case 0x5b:
		{
			this->cpu.E = this->cpu.E;
			break;
		}

		//MOV E,H
		case 0x5c:
		{
			this->cpu.E = this->cpu.H;
			break;
		}

		//MOV E,L
		case 0x5d:
		{
			this->cpu.E = this->cpu.L;
			break;
		}

		//MOV E,M
		case 0x5e:
		{
			//this->cpu.E = this->memory[this->ConvertToNumber(this->cpu.H, this->cpu.L)];
			this->cpu.E = this->memory[*this->cpu.HL];
			break;
		}

		//MOV E,A
		case 0x5f:
		{
			this->cpu.E = this->cpu.A;
			break;
		}

		//MOV H,B
		case 0x60:
		{
			this->cpu.H = this->cpu.B;
			break;
		}

		//MOV H,C
		case 0x61:
		{
			this->cpu.H = this->cpu.C;
			break;
		}

		//MOV H,D
		case 0x62:
		{
			this->cpu.H = this->cpu.D;
			break;
		}

		//MOV H,E
		case 0x63:
		{
			this->cpu.H = this->cpu.E;
			break;
		}

		//MOV H,H
		case 0x64:
		{
			this->cpu.H = this->cpu.H;
			break;
		}

		//MOV H,L
		case 0x65:
		{
			this->cpu.H = this->cpu.L;
			break;
		}

		//MOV H,M
		case 0x66:
		{
			//this->cpu.H = this->memory[this->ConvertToNumber(this->cpu.H, this->cpu.L)];
			this->cpu.H = this->memory[*this->cpu.HL];
			break;
		}

		//MOV H,A
		case 0x67:
		{
			this->cpu.H = this->cpu.A;
			break;
		}

		//MOV L,B
		case 0x68:
		{
			this->cpu.L = this->cpu.B;
			break;
		}

		//MOV L,C
		case 0x69:
		{
			this->cpu.L = this->cpu.C;
			break;
		}

		//MOV L,D
		case 0x6a:
		{
			this->cpu.L = this->cpu.D;
			break;
		}

		//MOV L,E
		case 0x6b:
		{
			this->cpu.L = this->cpu.E;
			break;
		}

		//MOV L,H
		case 0x6c:
		{
			this->cpu.L = this->cpu.H;
			break;
		}

		//MOV L,L
		case 0x6d:
		{
			this->cpu.L = this->cpu.L;
			break;
		}

		//MOV L,M
		case 0x6e:
		{
			//this->cpu.L = this->memory[this->ConvertToNumber(this->cpu.H, this->cpu.L)];
			this->cpu.L = this->memory[*this->cpu.HL];
			break;
		}

		//MOV L,A
		case 0x6f:
		{
			this->cpu.L = this->cpu.A;
			break;
		}

		//MOV M,B
		case 0x70:
		{
			/*uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			this->memory[offset] = this->cpu.B;*/
			this->memory[*this->cpu.HL] = this->cpu.B;
			break;
		}

		//MOV M,C
		case 0x71:
		{
			//uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			//this->memory[offset] = this->cpu.C;
			this->memory[*this->cpu.HL] = this->cpu.C;
			break;
		}

		//MOV M,D
		case 0x72:
		{
			/*uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			this->memory[offset] = this->cpu.D;*/
			this->memory[*this->cpu.HL] = this->cpu.D;
			break;
		}

		//MOV M,E
		case 0x73:
		{
			/*uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			this->memory[offset] = this->cpu.E;*/
			this->memory[*this->cpu.HL] = this->cpu.E;
			break;
		}

		//MOV M,H
		case 0x74:
		{
			/*uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			this->memory[offset] = this->cpu.H;*/
			this->memory[*this->cpu.HL] = this->cpu.H;
			break;
		}

		//MOV M,L
		case 0x75:
		{
			/*uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			this->memory[offset] = this->cpu.L;*/
			this->memory[*this->cpu.HL] = this->cpu.L;
			break;
		}

		//HLT
		case 0x76:
		{
			this->UnimplementedInstruction();
			break;
		}

		//MOV M,A
		case 0x77:
		{
			/*uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			this->memory[offset] = this->cpu.A;*/
			this->memory[*this->cpu.HL] = this->cpu.A;
			break;
		}

		//MOV A,B
		case 0x78:
		{
			this->cpu.A = this->cpu.B;
			break;
		}

		//MOV A,C
		case 0x79:
		{
			this->cpu.A = this->cpu.C;
			break;
		}

		//MOV A,D
		case 0x7a:
		{
			this->cpu.A = this->cpu.D;
			break;
		}

		//MOV A,E
		case 0x7b:
		{
			this->cpu.A = this->cpu.E;
			break;
		}

		//MOV A,H
		case 0x7c:
		{
			this->cpu.A = this->cpu.H;
			break;
		}

		//MOV A,L
		case 0x7d:
		{
			this->cpu.A = this->cpu.L;
			break;
		}

		//MOV A,M
		case 0x7e:
		{
			//this->cpu.A = this->memory[this->ConvertToNumber(this->cpu.H, this->cpu.L)];
			this->cpu.A = this->memory[*this->cpu.HL];
			break;
		}

		//MOV A,A
		case 0x7f:
		{
			this->cpu.A = this->cpu.A;
			break;
		}

		//ADD B
		case 0x80:
		{
			this->PerformAdd(this->cpu.B);
			break;
		}

		//ADD C
		case 0x81:
		{
			this->PerformAdd(this->cpu.C);
			break;
		}

		//ADD D
		case 0x82:
		{
			this->PerformAdd(this->cpu.D);
			break;
		}

		//ADD E
		case 0x83:
		{
			this->PerformAdd(this->cpu.E);
			break;
		}

		//ADD H
		case 0x84:
		{
			this->PerformAdd(this->cpu.H);
			break;
		}

		//ADD L
		case 0x85:
		{
			this->PerformAdd(this->cpu.L);
			break;
		}

		//ADD M
		case 0x86:
		{
			this->PerformAdd(this->memory[*this->cpu.HL]);
			break;
		}

		//ADD A
		case 0x87:
		{
			this->PerformAdd(this->cpu.A);
			break;
		}

		//ADC B
		case 0x88:
		{
			this->PerformAdd(this->cpu.B, true);
			break;
		}

		//ADC C
		case 0x89:
		{
			this->PerformAdd(this->cpu.C, true);
			break;
		}

		//ADC D
		case 0x8a:
		{
			this->PerformAdd(this->cpu.D, true);
			break;
		}

		//ADC E
		case 0x8b:
		{
			this->PerformAdd(this->cpu.E, true);
			break;
		}

		//ADC H
		case 0x8c:
		{
			this->PerformAdd(this->cpu.H, true);
			break;
		}

		//ADC L
		case 0x8d:
		{
			this->PerformAdd(this->cpu.L, true);
			break;
		}

		//ADC M
		case 0x8e:
		{
			this->PerformAdd(this->memory[*this->cpu.HL], true);
			break;
		}

		//ADC A
		case 0x8f:
		{
			this->PerformAdd(this->cpu.A, true);
			break;
		}

		//SUB B
		case 0x90:
		{
			this->PerformSub(this->cpu.B);
			break;
		}

		//SUB C
		case 0x91:
		{
			this->PerformSub(this->cpu.C);
			break;
		}

		//SUB D
		case 0x92:
		{
			this->PerformSub(this->cpu.D);
			break;
		}

		//SUB E
		case 0x93:
		{
			this->PerformSub(this->cpu.E);
			break;
		}

		//SUB H
		case 0x94:
		{
			this->PerformSub(this->cpu.H);
			break;
		}

		//SUB L
		case 0x95:
		{
			this->PerformSub(this->cpu.L);
			break;
		}

		//SUB M
		case 0x96:
		{
			this->PerformSub(this->memory[*this->cpu.HL]);
			break;
		}

		//SUB A
		case 0x97:
		{
			this->PerformSub(this->cpu.A);
			break;
		}

		//SBB B
		case 0x98:
		{
			this->PerformSub(this->cpu.B, true);
			break;
		}

		//SBB C
		case 0x99:
		{
			this->PerformSub(this->cpu.C, true);
			break;
		}

		//SBB D
		case 0x9a:
		{
			this->PerformSub(this->cpu.D, true);
			break;
		}

		//SBB E
		case 0x9b:
		{
			this->PerformSub(this->cpu.E, true);
			break;
		}

		//SBB H
		case 0x9c:
		{
			this->PerformSub(this->cpu.H, true);
			break;
		}

		//SBB L
		case 0x9d:
		{
			this->PerformSub(this->cpu.L, true);
			break;
		}

		//SBB M
		case 0x9e:
		{
			this->PerformSub(this->memory[*this->cpu.HL], true);
			break;
		}

		//SBB A
		case 0x9f:
		{
			this->PerformSub(this->cpu.A, true);
			break;
		}

		//ANA B
		case 0xa0:
		{
			this->PerformAnd(this->cpu.B);
			break;
		}

		//ANA C
		case 0xa1:
		{
			this->PerformAnd(this->cpu.C);
			break;
		}

		//ANA D
		case 0xa2:
		{
			this->PerformAnd(this->cpu.D);
			break;
		}

		//ANA E
		case 0xa3:
		{
			this->PerformAnd(this->cpu.E);
			break;
		}

		//ANA H
		case 0xa4:
		{
			this->PerformAnd(this->cpu.H);
			break;
		}

		//ANA L
		case 0xa5:
		{
			this->PerformAnd(this->cpu.L);
			break;
		}

		//ANA M
		case 0xa6:
		{
			this->PerformAnd(this->memory[*this->cpu.HL]);
			break;
		}

		//ANA A
		case 0xa7:
		{
			this->PerformAnd(this->cpu.A);
			break;
		}

		//XRA B
		case 0xa8:
		{
			this->PerformXor(this->cpu.B);
			break;
		}

		//XRA C
		case 0xa9:
		{
			this->PerformXor(this->cpu.C);
			break;
		}

		//XRA D
		case 0xaa:
		{
			this->PerformXor(this->cpu.D);
			break;
		}

		//XRA E
		case 0xab:
		{
			this->PerformXor(this->cpu.E);
			break;
		}

		//XRA H
		case 0xac:
		{
			this->PerformXor(this->cpu.H);
			break;
		}

		//XRA L
		case 0xad:
		{
			this->PerformXor(this->cpu.L);
			break;
		}

		//XRA M
		case 0xae:
		{
			this->PerformXor(this->memory[*this->cpu.HL]);
			break;
		}

		//XRA A
		case 0xaf:
		{
			this->PerformXor(this->cpu.A);
			break;
		}

		//ORA B
		case 0xb0:
		{
			this->PerformOr(this->cpu.B);
			break;
		}

		//ORA C
		case 0xb1:
		{
			this->PerformOr(this->cpu.C);
			break;
		}

		//ORA D
		case 0xb2:
		{
			this->PerformOr(this->cpu.D);
			break;
		}

		//ORA E
		case 0xb3:
		{
			this->PerformOr(this->cpu.E);
			break;
		}

		//ORA H
		case 0xb4:
		{
			this->PerformOr(this->cpu.H);
			break;
		}

		//ORA L
		case 0xb5:
		{
			this->PerformOr(this->cpu.L);
			break;
		}

		//ORA M
		case 0xb6:
		{
			this->PerformOr(this->memory[*this->cpu.HL]);
			break;
		}

		//ORA A
		case 0xb7:
		{
			this->PerformOr(this->cpu.A);
			break;
		}

		//CMP B
		case 0xb8:
		{
			this->CalculateFlags(this->cpu.A - this->cpu.B, true, true, true, true, false);
			break;
		}

		//CMP C
		case 0xb9:
		{
			this->CalculateFlags(this->cpu.A - this->cpu.C, true, true, true, true, false);
			break;
		}

		//CMP D
		case 0xba:
		{
			this->CalculateFlags(this->cpu.A - this->cpu.D, true, true, true, true, false);
			break;
		}

		//CMP E
		case 0xbb:
		{
			this->CalculateFlags(this->cpu.A - this->cpu.E, true, true, true, true, false);
			break;
		}

		//CMP H
		case 0xbc:
		{
			this->CalculateFlags(this->cpu.A - this->cpu.H, true, true, true, true, false);
			break;
		}

		//CMP L
		case 0xbd:
		{
			this->CalculateFlags(this->cpu.A - this->cpu.L, true, true, true, true, false);
			break;
		}

		//CMP M
		case 0xbe:
		{
			this->CalculateFlags(this->cpu.A - this->memory[*this->cpu.HL], true, true, true, true, false);
			break;
		}

		//CMP A
		case 0xbf:
		{
			this->CalculateFlags(this->cpu.A - this->cpu.A, true, true, true, true, false);
			break;
		}

		//RNZ
		case 0xc0:
		{
			this->UnimplementedInstruction();
			break;
		}

		//POP B
		case 0xc1:
		{
			this->UnimplementedInstruction();
			break;
		}

		//JNZ adr
		case 0xc2:
		{
			this->UnimplementedInstruction();
			break;
		}

		//JMP adr
		case 0xc3:
		{
			this->UnimplementedInstruction();
			break;
		}

		//CNZ adr
		case 0xc4:
		{
			this->UnimplementedInstruction();
			break;
		}

		//PUSH B
		case 0xc5:
		{
			this->UnimplementedInstruction();
			break;
		}

		//ADI D8
		case 0xc6:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RST 0
		case 0xc7:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RZ
		case 0xc8:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RET
		case 0xc9:
		{
			this->UnimplementedInstruction();
			break;
		}

		//JZ adr
		case 0xca:
		{
			this->UnimplementedInstruction();
			break;
		}

		//NOP
		case 0xcb:
		{
			this->UnimplementedInstruction();
			break;
		}

		//CZ adr
		case 0xcc:
		{
			this->UnimplementedInstruction();
			break;
		}

		//CALL adr
		case 0xcd:
		{
			this->UnimplementedInstruction();
			break;
		}

		//ACI D8
		case 0xce:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RST 1
		case 0xcf:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RNC
		case 0xd0:
		{
			this->UnimplementedInstruction();
			break;
		}

		//POP D
		case 0xd1:
		{
			this->UnimplementedInstruction();
			break;
		}

		//JNC adr
		case 0xd2:
		{
			this->UnimplementedInstruction();
			break;
		}

		//OUT D8
		case 0xd3:
		{
			this->UnimplementedInstruction();
			break;
		}

		//CNC adr
		case 0xd4:
		{
			this->UnimplementedInstruction();
			break;
		}

		//PUSH D
		case 0xd5:
		{
			this->UnimplementedInstruction();
			break;
		}

		//SUI D8
		case 0xd6:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RST 2
		case 0xd7:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RC
		case 0xd8:
		{
			this->UnimplementedInstruction();
			break;
		}

		//NOP
		case 0xd9:
		{
			this->UnimplementedInstruction();
			break;
		}

		//JC adr
		case 0xda:
		{
			this->UnimplementedInstruction();
			break;
		}

		//IN D8
		case 0xdb:
		{
			this->UnimplementedInstruction();
			break;
		}

		//CC adr
		case 0xdc:
		{
			this->UnimplementedInstruction();
			break;
		}

		//NOP
		case 0xdd:
		{
			this->UnimplementedInstruction();
			break;
		}

		//SBI D8
		case 0xde:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RST 3
		case 0xdf:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RPO
		case 0xe0:
		{
			this->UnimplementedInstruction();
			break;
		}

		//POP H
		case 0xe1:
		{
			this->UnimplementedInstruction();
			break;
		}

		//JPO adr
		case 0xe2:
		{
			this->UnimplementedInstruction();
			break;
		}

		//XTHL
		case 0xe3:
		{
			this->UnimplementedInstruction();
			break;
		}

		//CPO adr
		case 0xe4:
		{
			this->UnimplementedInstruction();
			break;
		}

		//PUSH H
		case 0xe5:
		{
			this->UnimplementedInstruction();
			break;
		}

		//ANI D8
		case 0xe6:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RST 4
		case 0xe7:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RPE
		case 0xe8:
		{
			this->UnimplementedInstruction();
			break;
		}

		//PCHL
		case 0xe9:
		{
			this->UnimplementedInstruction();
			break;
		}

		//JPE adr
		case 0xea:
		{
			this->UnimplementedInstruction();
			break;
		}

		//XCHG
		case 0xeb:
		{
			this->UnimplementedInstruction();
			break;
		}

		//CPE adr
		case 0xec:
		{
			this->UnimplementedInstruction();
			break;
		}

		//NOP
		case 0xed:
		{
			this->UnimplementedInstruction();
			break;
		}

		//XRI D8
		case 0xee:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RST 5
		case 0xef:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RP
		case 0xf0:
		{
			this->UnimplementedInstruction();
			break;
		}

		//POP PSW
		case 0xf1:
		{
			this->UnimplementedInstruction();
			break;
		}

		//JP adr
		case 0xf2:
		{
			this->UnimplementedInstruction();
			break;
		}

		//DI
		case 0xf3:
		{
			this->UnimplementedInstruction();
			break;
		}

		//CP adr
		case 0xf4:
		{
			this->UnimplementedInstruction();
			break;
		}

		//PUSH PSW
		case 0xf5:
		{
			this->UnimplementedInstruction();
			break;
		}

		//ORI D8
		case 0xf6:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RST 6
		case 0xf7:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RM
		case 0xf8:
		{
			this->UnimplementedInstruction();
			break;
		}

		//SPHL
		case 0xf9:
		{
			this->UnimplementedInstruction();
			break;
		}

		//JM adr
		case 0xfa:
		{
			this->UnimplementedInstruction();
			break;
		}

		//EI
		case 0xfb:
		{
			this->UnimplementedInstruction();
			break;
		}

		//CM adr
		case 0xfc:
		{
			this->UnimplementedInstruction();
			break;
		}

		//NOP
		case 0xfd:
		{
			this->UnimplementedInstruction();
			break;
		}

		//CPI D8
		case 0xfe:
		{
			this->UnimplementedInstruction();
			break;
		}

		//RST 7
		case 0xff:
		{
			this->UnimplementedInstruction();
			break;
		}
	}
}

void Emulator8080::UnimplementedInstruction()
{

}