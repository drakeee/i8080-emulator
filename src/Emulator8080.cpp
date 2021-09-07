#include "Main.h"

#define FOR_CPUDIAG

uint8_t cycles8080[] = {
	4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x00..0x0f
	4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x10..0x1f
	4, 10, 16, 5, 5, 5, 7, 4, 4, 10, 16, 5, 5, 5, 7, 4,
	4, 10, 13, 5, 10, 10, 10, 4, 4, 10, 13, 5, 5, 5, 7, 4,

	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, //0x40..0x4f
	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,
	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,
	7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 7, 5,

	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0x80..8x4f
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,

	11, 10, 10, 10, 17, 11, 7, 11, 11, 10, 10, 10, 10, 17, 7, 11, //0xc0..0xcf
	11, 10, 10, 10, 17, 11, 7, 11, 11, 10, 10, 10, 10, 17, 7, 11,
	11, 10, 10, 18, 17, 11, 7, 11, 11, 5, 10, 5, 17, 17, 7, 11,
	11, 10, 10, 4, 17, 11, 7, 11, 11, 5, 10, 4, 17, 17, 7, 11,
};

int Disassemble8080(Emulator8080* emulator, int num = 0)
{
	auto cpu = emulator->GetCPU();
	auto memory = emulator->GetMemory();

	unsigned char* code = &memory[cpu->PC + num];
	int opbytes = 1;

	Utils::PrintDebug("%04x\t%02X\t", cpu->PC + num, *code);

	switch (code[0])
	{
	case 0x00: printf("NOP"); break;
	case 0x01: printf("LXI\tB,$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0x02: printf("STAX\tB"); break;
	case 0x03: printf("INX\tB"); break;
	case 0x04: printf("INR\tB"); break;
	case 0x05: printf("DCR\tB"); break;
	case 0x06: printf("MVI\tB, $%02x", code[1]); opbytes = 2; break;
	case 0x07: printf("RLC"); break;
	case 0x08: break;
	case 0x09: printf("DAD\tB"); break;
	case 0x0a: printf("LDAX\tB"); break;
	case 0x0b: printf("DCX\tB"); break;
	case 0x0c: printf("INR\tC"); break;
	case 0x0d: printf("DCR\tC"); break;
	case 0x0e: printf("MVI\tC,$%02x", code[1]); opbytes = 2; break;
	case 0x0f: printf("RRC"); break;
	case 0x10: break;
	case 0x11: printf("LXI\tD,$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0x12: printf("STAX\tD"); break;
	case 0x13: printf("INX\tD"); break;
	case 0x14: printf("INR\tD"); break;
	case 0x15: printf("DCR\tD"); break;
	case 0x16: printf("MVI\tD, $%02x", code[1]); opbytes = 2; break;
	case 0x17: printf("RAL"); break;
	case 0x18: break;
	case 0x19: printf("DAD\tD"); break;
	case 0x1a: printf("LDAX\tD"); break;
	case 0x1b: printf("DCX\tD"); break;
	case 0x1c: printf("INR\tE"); break;
	case 0x1d: printf("DCR\tE"); break;
	case 0x1e: printf("MVI\tE,$%02x", code[1]); opbytes = 2; break;
	case 0x1f: printf("RAR"); break;
	case 0x20: break;
	case 0x21: printf("LXI\tH,$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0x22: printf("SHLD\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0x23: printf("INX\tH"); break;
	case 0x24: printf("INR\tH"); break;
	case 0x25: printf("DCR\tH"); break;
	case 0x26: printf("MVI\tH,$%02x", code[1]); opbytes = 2; break;
	case 0x27: printf("DAA"); break;
	case 0x28: break;
	case 0x29: printf("DAD\tH"); break;
	case 0x2a: printf("LHLD\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0x2b: printf("DCX\tH"); break;
	case 0x2c: printf("INR\tL"); break;
	case 0x2d: printf("DCR\tL"); break;
	case 0x2e: printf("MVI\tL, $%02x", code[1]); opbytes = 2; break;
	case 0x2f: printf("CMA"); break;
	case 0x30: break;
	case 0x31: printf("LXI\tSP, $%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0x32: printf("STA\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0x33: printf("INX\tSP"); break;
	case 0x34: printf("INR\tM"); break;
	case 0x35: printf("DCR\tM"); break;
	case 0x36: printf("MVI\tM,$%02x", code[1]); opbytes = 2; break;
	case 0x37: printf("STC"); break;
	case 0x38: break;
	case 0x39: printf("DAD\tSP"); break;
	case 0x3a: printf("LDA\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0x3b: printf("DCX\tSP"); break;
	case 0x3c: printf("INR\tA"); break;
	case 0x3d: printf("DCR\tA"); break;
	case 0x3e: printf("MVI\tA,$%02x", code[1]); opbytes = 2; break;
	case 0x3f: printf("CMC"); break;
	case 0x40: printf("MOV\tB,B"); break;
	case 0x41: printf("MOV\tB,C"); break;
	case 0x42: printf("MOV\tB,D"); break;
	case 0x43: printf("MOV\tB,E"); break;
	case 0x44: printf("MOV\tB,H"); break;
	case 0x45: printf("MOV\tB,L"); break;
	case 0x46: printf("MOV\tB,M"); break;
	case 0x47: printf("MOV\tB,A"); break;
	case 0x48: printf("MOV\tC,B"); break;
	case 0x49: printf("MOV\tC,C"); break;
	case 0x4a: printf("MOV\tC,D"); break;
	case 0x4b: printf("MOV\tC,E"); break;
	case 0x4c: printf("MOV\tC,H"); break;
	case 0x4d: printf("MOV\tC,L"); break;
	case 0x4e: printf("MOV\tC,M"); break;
	case 0x4f: printf("MOV\tC,A"); break;
	case 0x50: printf("MOV\tD,B"); break;
	case 0x51: printf("MOV\tD,C"); break;
	case 0x52: printf("MOV\tD,D"); break;
	case 0x53: printf("MOV\tD,E"); break;
	case 0x54: printf("MOV\tD,H"); break;
	case 0x55: printf("MOV\tD,L"); break;
	case 0x56: printf("MOV\tD,M"); break;
	case 0x57: printf("MOV\tD,A"); break;
	case 0x58: printf("MOV\tE,B"); break;
	case 0x59: printf("MOV\tE,C"); break;
	case 0x5a: printf("MOV\tE,D"); break;
	case 0x5b: printf("MOV\tE,E"); break;
	case 0x5c: printf("MOV\tE,H"); break;
	case 0x5d: printf("MOV\tE,L"); break;
	case 0x5e: printf("MOV\tE,M"); break;
	case 0x5f: printf("MOV\tE,A"); break;
	case 0x60: printf("MOV\tH,B"); break;
	case 0x61: printf("MOV\tH,C"); break;
	case 0x62: printf("MOV\tH,D"); break;
	case 0x63: printf("MOV\tH,E"); break;
	case 0x64: printf("MOV\tH,H"); break;
	case 0x65: printf("MOV\tH,L"); break;
	case 0x66: printf("MOV\tH,M"); break;
	case 0x67: printf("MOV\tH,A"); break;
	case 0x68: printf("MOV\tL,B"); break;
	case 0x69: printf("MOV\tL,C"); break;
	case 0x6a: printf("MOV\tL,D"); break;
	case 0x6b: printf("MOV\tL,E"); break;
	case 0x6c: printf("MOV\tL,H"); break;
	case 0x6d: printf("MOV\tL,L"); break;
	case 0x6e: printf("MOV\tL,M"); break;
	case 0x6f: printf("MOV\tL,A"); break;
	case 0x70: printf("MOV\tM,B"); break;
	case 0x71: printf("MOV\tM,C"); break;
	case 0x72: printf("MOV\tM,D"); break;
	case 0x73: printf("MOV\tM,E"); break;
	case 0x74: printf("MOV\tM,H"); break;
	case 0x75: printf("MOV\tM,L"); break;
	case 0x76: printf("HLT"); break;
	case 0x77: printf("MOV\tM,A"); break;
	case 0x78: printf("MOV\tA,B"); break;
	case 0x79: printf("MOV\tA,C"); break;
	case 0x7a: printf("MOV\tA,D"); break;
	case 0x7b: printf("MOV\tA,E"); break;
	case 0x7c: printf("MOV\tA,H"); break;
	case 0x7d: printf("MOV\tA,L"); break;
	case 0x7e: printf("MOV\tA,M"); break;
	case 0x7f: printf("MOV\tA,A"); break;
	case 0x80: printf("ADD\tB"); break;
	case 0x81: printf("ADD\tC"); break;
	case 0x82: printf("ADD\tD"); break;
	case 0x83: printf("ADD\tE"); break;
	case 0x84: printf("ADD\tH"); break;
	case 0x85: printf("ADD\tL"); break;
	case 0x86: printf("ADD\tM"); break;
	case 0x87: printf("ADD\tA"); break;
	case 0x88: printf("ADC\tB"); break;
	case 0x89: printf("ADC\tC"); break;
	case 0x8a: printf("ADC\tD"); break;
	case 0x8b: printf("ADC\tE"); break;
	case 0x8c: printf("ADC\tH"); break;
	case 0x8d: printf("ADC\tL"); break;
	case 0x8e: printf("ADC\tM"); break;
	case 0x8f: printf("ADC\tA"); break;
	case 0x90: printf("SUB\tB"); break;
	case 0x91: printf("SUB\tC"); break;
	case 0x92: printf("SUB\tD"); break;
	case 0x93: printf("SUB\tE"); break;
	case 0x94: printf("SUB\tH"); break;
	case 0x95: printf("SUB\tL"); break;
	case 0x96: printf("SUB\tM"); break;
	case 0x97: printf("SUB\tA"); break;
	case 0x98: printf("SBB\tB"); break;
	case 0x99: printf("SBB\tC"); break;
	case 0x9a: printf("SBB\tD"); break;
	case 0x9b: printf("SBB\tE"); break;
	case 0x9c: printf("SBB\tH"); break;
	case 0x9d: printf("SBB\tL"); break;
	case 0x9e: printf("SBB\tM"); break;
	case 0x9f: printf("SBB\tA"); break;
	case 0xa0: printf("ANA\tB"); break;
	case 0xa1: printf("ANA\tC"); break;
	case 0xa2: printf("ANA\tD"); break;
	case 0xa3: printf("ANA\tE"); break;
	case 0xa4: printf("ANA\tH"); break;
	case 0xa5: printf("ANA\tL"); break;
	case 0xa6: printf("ANA\tM"); break;
	case 0xa7: printf("ANA\tA"); break;
	case 0xa8: printf("XRA\tB"); break;
	case 0xa9: printf("XRA\tC"); break;
	case 0xaa: printf("XRA\tD"); break;
	case 0xab: printf("XRA\tE"); break;
	case 0xac: printf("XRA\tH"); break;
	case 0xad: printf("XRA\tL"); break;
	case 0xae: printf("XRA\tM"); break;
	case 0xaf: printf("XRA\tA"); break;
	case 0xb0: printf("ORA\tB"); break;
	case 0xb1: printf("ORA\tC"); break;
	case 0xb2: printf("ORA\tD"); break;
	case 0xb3: printf("ORA\tE"); break;
	case 0xb4: printf("ORA\tH"); break;
	case 0xb5: printf("ORA\tL"); break;
	case 0xb6: printf("ORA\tM"); break;
	case 0xb7: printf("ORA\tA"); break;
	case 0xb8: printf("CMP\tB"); break;
	case 0xb9: printf("CMP\tC"); break;
	case 0xba: printf("CMP\tD"); break;
	case 0xbb: printf("CMP\tE"); break;
	case 0xbc: printf("CMP\tH"); break;
	case 0xbd: printf("CMP\tL"); break;
	case 0xbe: printf("CMP\tM"); break;
	case 0xbf: printf("CMP\tA"); break;
	case 0xc0: printf("RNZ"); break;
	case 0xc1: printf("POP\tB"); break;
	case 0xc2: printf("JNZ\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xc3: printf("JMP\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xc4: printf("CNZ\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xc5: printf("PUSH\tB"); break;
	case 0xc6: printf("ADI\t$%02x", code[1]); opbytes = 2; break;
	case 0xc7: printf("RST\t0"); break;
	case 0xc8: printf("RZ"); break;
	case 0xc9: printf("RET\t%04x", *(uint16_t*)&memory[cpu->SP]); break;
	case 0xca: printf("JZ\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xcb: break;
	case 0xcc: printf("CZ\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xcd: printf("CALL\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xce: printf("ACI\t$%02x", code[1]); opbytes = 2; break;
	case 0xcf: printf("RST\t1"); break;
	case 0xd0: printf("RNC"); break;
	case 0xd1: printf("POP\tD"); break;
	case 0xd2: printf("JNC\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xd3: printf("OUT\t$%02x", code[1]); opbytes = 2; break;
	case 0xd4: printf("CNC\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xd5: printf("PUSH\tD"); break;
	case 0xd6: printf("SUI\t$%02x", code[1]); opbytes = 2; break;
	case 0xd7: printf("RST\t2"); break;
	case 0xd8: printf("RC"); break;
	case 0xd9: break;
	case 0xda: printf("JC\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xdb: printf("IN\t$%02x", code[1]); opbytes = 2; break;
	case 0xdc: printf("CC\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xdd: break;
	case 0xde: printf("SBI\t$%02x", code[1]); opbytes = 2; break;
	case 0xdf: printf("RST\t3"); break;
	case 0xe0: printf("RPO"); break;
	case 0xe1: printf("POP\tH"); break;
	case 0xe2: printf("JPO\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xe3: printf("XTHL"); break;
	case 0xe4: printf("CPO\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xe5: printf("PUSH\tH"); break;
	case 0xe6: printf("ANI\t$%02x", code[1]); opbytes = 2; break;
	case 0xe7: printf("RST\t4"); break;
	case 0xe8: printf("RPE"); break;
	case 0xe9: printf("PCHL"); break;
	case 0xea: printf("JPE\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xeb: printf("XCHG"); break;
	case 0xec: printf("CPE\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xed: break;
	case 0xee: printf("XRI\t$%02x", code[1]); opbytes = 2; break;
	case 0xef: printf("RST\t5"); break;
	case 0xf0: printf("RP"); break;
	case 0xf1: printf("POP\tPSW"); break;
	case 0xf2: printf("JP\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xf3: printf("DI"); break;
	case 0xf4: printf("CP\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xf5: printf("PUSH\tPSW"); break;
	case 0xf6: printf("ORI\t$%02x", code[1]); opbytes = 2; break;
	case 0xf7: printf("RST\t6"); break;
	case 0xf8: printf("RM"); break;
	case 0xf9: printf("SPHL"); break;
	case 0xfa: printf("JM\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xfb: printf("EI"); break;
	case 0xfc: printf("CM\t$%02x%02x", code[2], code[1]); opbytes = 3; break;
	case 0xfd: break;
	case 0xfe: printf("CPI\t$%02x", code[1]); opbytes = 2; break;
	case 0xff: printf("RST\t7"); break;
	}

	//Utils::PrintDebug("\t ret: %d\n", opbytes);
	//Utils::PrintDebug("\n");
	printf("\n");

	return opbytes;
}

Emulator8080::Emulator8080()
{
	this->cpu.InPort[0] &= 0b10001111;
	this->cpu.InPort[1] &= 0b10001000;
	this->cpu.InPort[2] &= 0b10001011;
}

Emulator8080::Emulator8080(const char* fileName, int startOffset) :
	Emulator8080()
{
	this->Load(fileName, startOffset);
}

void Emulator8080::Load(const char* fileName, int startOffset)
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

	fread((void*)&this->memory[startOffset], 1, fileSize, rom);
}

void Emulator8080::GenerateInterrupt(int interrupt_num)
{
	if (!this->cpu.interrupt_enabled)
		return;

	//perform "PUSH PC"
	this->Push(this->cpu.PC);

	//Set the PC to the low memory vector
	this->cpu.PC = 8 * interrupt_num;

	//"DI"
	this->cpu.interrupt_enabled = false;
}

uint8_t Emulator8080::ProcessInstruction()
{
	unsigned char* opcode = &this->memory[this->cpu.PC];

	//Disassemble8080(this->memory, this->cpu.PC);
	//printf("\n");
	//Utils::PrintDebug("%04x\t%02x\t%s", this->cpu.PC, opcode[0], DISASSEMBLE_TABLE[opcode[0]]);

	this->cpu.PC += 1;

	switch (*opcode)
	{
		//NOP
		case 0x00:
		{
			//this->UnimplementedInstruction();
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
			this->WriteMemory(*this->cpu.BC, this->cpu.A);
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
			this->CalculateFlags(this->cpu.B + 1, true, true, false, true, true);
			this->cpu.B++;
			break;
		}

		//DCR B
		case 0x05:
		{
			this->CalculateFlags(this->cpu.B - 1, true, true, false, true, true);
			this->cpu.B--;
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
			(*this->cpu.HL) += (*this->cpu.BC);
			break;
		}

		//LDAX B
		case 0x0a:
		{
			this->cpu.A = this->ReadMemory(*this->cpu.BC);
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
			this->CalculateFlags(this->cpu.C + 1, true, true, false, true, true);
			this->cpu.C++;
			break;
		}

		//DCR C
		case 0x0d:
		{
			this->CalculateFlags(this->cpu.C - 1, true, true, false, true, true);
			this->cpu.C--;
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
			this->WriteMemory(*this->cpu.DE, this->cpu.A);
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
			this->CalculateFlags(this->cpu.D + 1, true, true, false, true, true);
			this->cpu.D++;
			break;
		}

		//DCR D
		case 0x15:
		{
			this->CalculateFlags(this->cpu.D - 1, true, true, false, true, true);
			this->cpu.D--;
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
			(*this->cpu.HL) += (*this->cpu.DE);
			break;
		}

		//LDAX D
		case 0x1a:
		{
			this->cpu.A = this->ReadMemory(*this->cpu.DE);
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
			this->CalculateFlags(this->cpu.E + 1, true, true, false, true, true);
			this->cpu.E++;
			break;
		}

		//DCR E
		case 0x1d:
		{
			this->CalculateFlags(this->cpu.E - 1, true, true, false, true, true);
			this->cpu.E--;
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
			this->CalculateFlags(this->cpu.H + 1, true, true, false, true, false);
			this->cpu.H++;
			break;
		}

		//DCR H
		case 0x25:
		{
			this->CalculateFlags(this->cpu.H - 1, true, true, false, true, false);
			this->cpu.H--;
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
			uint8_t lsb = this->cpu.A & 0xF;
			if (lsb > 9 || this->cpu.flags.AC)
				this->cpu.A += 0x06; //adds six to the accumulator

			uint8_t msb = this->cpu.A >> 4;
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
			*this->cpu.HL = *(uint16_t*)&this->memory[this->ConvertToNumber(opcode[2], opcode[1])];
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
			this->CalculateFlags(this->cpu.L + 1, true, true, false, true, true);
			this->cpu.L++;
			break;
		}

		//DCR L
		case 0x2d:
		{
			this->CalculateFlags(this->cpu.L - 1, true, true, false, true, true);
			this->cpu.L--;
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
			this->WriteMemory(this->ConvertToNumber(opcode[2], opcode[1]), this->cpu.A);
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
			this->CalculateFlags(this->memory[*this->cpu.HL] + 1, true, true, false, true, false);
			this->memory[*this->cpu.HL]++;

			break;
		}

		//DCR M
		case 0x35:
		{
			this->CalculateFlags(this->memory[*this->cpu.HL] - 1, true, true, false, true, false);
			this->memory[*this->cpu.HL]--;

			break;
		}

		//MVI M,D8
		case 0x36:
		{
			this->WriteMemory(*this->cpu.HL, opcode[1]);
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
			this->cpu.A = this->ReadMemory(this->ConvertToNumber(opcode[2], opcode[1]));
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
			this->CalculateFlags(this->cpu.A + 1, true, true, false, true, true);
			this->cpu.A++;
			break;
		}

		//DCR A
		case 0x3d:
		{
			this->CalculateFlags(this->cpu.A - 1, true, true, false, true, true);
			this->cpu.A--;
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
			//this->cpu.B = this->ReadMemory(this->ConvertToNumber(this->cpu.H, this->cpu.L));
			this->cpu.B = this->ReadMemory(*this->cpu.HL);
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
			//this->cpu.C = this->ReadMemory(this->ConvertToNumber(this->cpu.H, this->cpu.L));
			this->cpu.C = this->ReadMemory(*this->cpu.HL);
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
			//this->cpu.D = this->ReadMemory(this->ConvertToNumber(this->cpu.H, this->cpu.L));
			this->cpu.D = this->ReadMemory(*this->cpu.HL);
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
			//this->cpu.E = this->ReadMemory(this->ConvertToNumber(this->cpu.H, this->cpu.L));
			this->cpu.E = this->ReadMemory(*this->cpu.HL);
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
			//this->cpu.H = this->ReadMemory(this->ConvertToNumber(this->cpu.H, this->cpu.L));
			this->cpu.H = this->ReadMemory(*this->cpu.HL);
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
			//this->cpu.L = this->ReadMemory(this->ConvertToNumber(this->cpu.H, this->cpu.L));
			this->cpu.L = this->ReadMemory(*this->cpu.HL);
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
			this->WriteMemory(offset, this->cpu.B);*/
			this->WriteMemory(*this->cpu.HL, this->cpu.B);
			break;
		}

		//MOV M,C
		case 0x71:
		{
			//uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			//this->WriteMemory(offset, this->cpu.C);
			this->WriteMemory(*this->cpu.HL, this->cpu.C);
			break;
		}

		//MOV M,D
		case 0x72:
		{
			/*uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			this->WriteMemory(offset, this->cpu.D);*/
			this->WriteMemory(*this->cpu.HL, this->cpu.D);
			break;
		}

		//MOV M,E
		case 0x73:
		{
			/*uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			this->WriteMemory(offset, this->cpu.E);*/
			this->WriteMemory(*this->cpu.HL, this->cpu.E);
			break;
		}

		//MOV M,H
		case 0x74:
		{
			/*uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			this->WriteMemory(offset, this->cpu.H);*/
			this->WriteMemory(*this->cpu.HL, this->cpu.H);
			break;
		}

		//MOV M,L
		case 0x75:
		{
			/*uint32_t offset = this->ConvertToNumber(this->cpu.H, this->cpu.L);
			this->WriteMemory(offset, this->cpu.L);*/
			this->WriteMemory(*this->cpu.HL, this->cpu.L);
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
			this->WriteMemory(offset, this->cpu.A);*/
			this->WriteMemory(*this->cpu.HL, this->cpu.A);
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
			//this->cpu.A = this->ReadMemory(this->ConvertToNumber(this->cpu.H, this->cpu.L));
			this->cpu.A = this->ReadMemory(*this->cpu.HL);
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
			if (!this->cpu.flags.Z)
				this->cpu.PC = this->Pop();
			
			break;
		}

		//POP B
		case 0xc1:
		{
			*this->cpu.BC = this->Pop();
			break;
		}

		//JNZ adr
		case 0xc2:
		{
			this->cpu.PC += 2;

			if (!this->cpu.flags.Z)
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);

			break;
		}

		//JMP adr
		case 0xc3:
		{
			this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);
			break;
		}

		//CNZ adr
		case 0xc4:
		{
			uint16_t ret = this->ConvertToNumber(opcode[2], opcode[1]);
			this->cpu.PC += 2;

			if (!this->cpu.flags.Z)
			{
				this->Push(this->cpu.PC);
				this->cpu.PC = ret;
			}

			break;
		}

		//PUSH B
		case 0xc5:
		{
			this->Push(*this->cpu.BC);
			break;
		}

		//ADI D8
		case 0xc6:
		{
			this->CalculateFlags(this->cpu.A + opcode[1], true, true, true, true, false);
			this->cpu.A += opcode[1];
			this->cpu.PC += 1;

			break;
		}

		//RST 0
		case 0xc7:
		{
			this->Push(this->cpu.PC);
			this->cpu.PC = 0x0000;
			break;
		}

		//RZ
		case 0xc8:
		{
			if (this->cpu.flags.Z)
				this->cpu.PC = this->Pop();

			break;
		}

		//RET
		case 0xc9:
		{
			this->cpu.PC = this->Pop();
			break;
		}

		//JZ adr
		case 0xca:
		{
			this->cpu.PC += 2;
			if (this->cpu.flags.Z)
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);

			break;
		}

		//NOP
		case 0xcb:
		{
			//this->UnimplementedInstruction();
			break;
		}

		//CZ adr
		case 0xcc:
		{
			uint16_t ret = this->ConvertToNumber(opcode[2], opcode[1]);
			this->cpu.PC += 2;

			if (this->cpu.flags.Z)
			{
				this->Push(this->cpu.PC);
				this->cpu.PC = ret;
			}

			break;
		}

		//CALL adr
		case 0xcd:
		{
			uint16_t ret = this->ConvertToNumber(opcode[2], opcode[1]);
			this->cpu.PC += 2;

			this->Push(this->cpu.PC);
			this->cpu.PC = ret;

			break;
		}

		//ACI D8
		case 0xce:
		{
			this->CalculateFlags(this->cpu.A + opcode[1] + this->cpu.flags.C, true, true, true, true, true);
			this->cpu.A += (opcode[1] + this->cpu.flags.C);
			this->cpu.PC += 1;

			break;
		}

		//RST 1
		case 0xcf:
		{
			this->Push(this->cpu.PC);
			this->cpu.PC = 0x0008;
			break;
		}

		//RNC
		case 0xd0:
		{
			if (!this->cpu.flags.C)
				this->cpu.PC = this->Pop();

			break;
		}

		//POP D
		case 0xd1:
		{
			*this->cpu.DE = this->Pop();
			break;
		}

		//JNC adr
		case 0xd2:
		{
			this->cpu.PC += 2;
			if (!this->cpu.flags.C)
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);

			break;
		}

		//OUT D8
		case 0xd3:
		{
			if (opcode[1] == 2)
			{
				this->cpu.shift_offset = this->cpu.A & 7;
			}
			else if (opcode[1] == 4)
			{
				this->cpu.shift0 = this->cpu.shift1;
				this->cpu.shift1 = this->cpu.A;
			}
			else this->cpu.OutPort[opcode[1]] = this->cpu.A;

			this->cpu.PC += 1;
			break;
		}

		//CNC adr
		case 0xd4:
		{
			this->cpu.PC += 2;
			if (!this->cpu.flags.C)
			{
				this->Push(this->cpu.PC);
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);
			}

			break;
		}

		//PUSH D
		case 0xd5:
		{
			this->Push(*this->cpu.DE);
			break;
		}

		//SUI D8
		case 0xd6:
		{
			this->CalculateFlags(this->cpu.A - opcode[1], true, true, true, true, true);
			this->cpu.A = this->cpu.A - opcode[1];
			this->cpu.PC += 1;

			break;
		}

		//RST 2
		case 0xd7:
		{
			this->Push(this->cpu.PC);
			this->cpu.PC = 0x0010;
			break;
		}

		//RC
		case 0xd8:
		{
			if (this->cpu.flags.C)
				this->cpu.PC = this->Pop();

			break;
		}

		//NOP
		case 0xd9:
		{
			//this->UnimplementedInstruction();
			break;
		}

		//JC adr
		case 0xda:
		{
			this->cpu.PC += 2;
			if (this->cpu.flags.C)
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);

			break;
		}

		//IN D8
		case 0xdb:
		{
			if (opcode[1] == 3)
			{
				uint16_t v = (this->cpu.shift1 << 8) | this->cpu.shift0;
				this->cpu.A = ((v >> (8 - this->cpu.shift_offset)) & 0xff);
			} else {
				this->cpu.A = this->cpu.InPort[opcode[1]];
			}

			this->cpu.PC += 1;
			break;
		}

		//CC adr
		case 0xdc:
		{
			this->cpu.PC += 2;
			if (this->cpu.flags.C)
			{
				this->Push(this->cpu.PC);
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);
			}
			break;
		}

		//NOP
		case 0xdd:
		{
			//this->UnimplementedInstruction();
			break;
		}

		//SBI D8
		case 0xde:
		{
			this->CalculateFlags(this->cpu.A - opcode[1] - this->cpu.flags.C, true, true, true, true, true);
			this->cpu.A = this->cpu.A - opcode[1] - this->cpu.flags.C;
			this->cpu.PC += 1;

			break;
		}

		//RST 3
		case 0xdf:
		{
			this->Push(this->cpu.PC);
			this->cpu.PC = 0x0018;
			break;
		}

		//RPO
		case 0xe0:
		{
			if (!this->cpu.flags.P)
				this->cpu.PC = this->Pop();

			break;
		}

		//POP H
		case 0xe1:
		{
			*this->cpu.HL = this->Pop();
			break;
		}

		//JPO adr
		case 0xe2:
		{
			this->cpu.PC += 2;
			if (!this->cpu.flags.P)
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);

			break;
		}

		//XTHL
		case 0xe3:
		{
			uint16_t temp = *(uint16_t*)&this->memory[this->cpu.SP];
			*(uint16_t*)&this->memory[this->cpu.SP] = *this->cpu.HL;
			*this->cpu.HL = temp;

			break;
		}

		//CPO adr
		case 0xe4:
		{
			this->cpu.PC += 2;
			if (!this->cpu.flags.P)
			{
				this->Push(this->cpu.PC);
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);
			}

			break;
		}

		//PUSH H
		case 0xe5:
		{
			this->Push(*this->cpu.HL);
			break;
		}

		//ANI D8
		case 0xe6:
		{
			this->CalculateFlags(this->cpu.A & opcode[1], true, true, false, true, false);
			this->cpu.flags.C = this->cpu.flags.AC = false;
			this->cpu.PC += 1;

			break;
		}

		//RST 4
		case 0xe7:
		{
			this->Push(this->cpu.PC);
			this->cpu.PC = 0x0020;
			break;
		}

		//RPE
		case 0xe8:
		{
			if (this->cpu.flags.P)
				this->cpu.PC = this->Pop();

			break;
		}

		//PCHL
		case 0xe9:
		{
			this->cpu.PC = *this->cpu.HL;
			break;
		}

		//JPE adr
		case 0xea:
		{
			this->cpu.PC += 2;
			if (this->cpu.flags.P)
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);

			break;
		}

		//XCHG
		case 0xeb:
		{
			uint16_t temp = *this->cpu.HL;
			*this->cpu.HL = *this->cpu.DE;
			*this->cpu.DE = temp;
			break;
		}

		//CPE adr
		case 0xec:
		{
			this->cpu.PC += 2;
			if (this->cpu.flags.P)
			{
				this->Push(this->cpu.PC);
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);
			}
			break;
		}

		//NOP
		case 0xed:
		{
			//this->UnimplementedInstruction();
			break;
		}

		//XRI D8
		case 0xee:
		{
			this->CalculateFlags(this->cpu.A ^ opcode[1], true, true, false, true, false);
			this->cpu.A = this->cpu.A ^ opcode[1];
			this->cpu.PC += 1;

			this->cpu.flags.C = this->cpu.flags.AC = false;

			break;
		}

		//RST 5
		case 0xef:
		{
			this->Push(this->cpu.PC);
			this->cpu.PC = 0x0028;
			break;
		}

		//RP
		case 0xf0:
		{
			if (!this->cpu.flags.S)
				this->cpu.PC = this->Pop();

			break;
		}

		//POP PSW
		case 0xf1:
		{
			uint16_t pop = this->Pop();
			this->cpu.A = (pop >> 8) & 0xFF00;

			std::bitset<8> psw(pop & 0xFF);
			this->cpu.flags.C = psw.test(0);
			this->cpu.flags.P = psw.test(2);
			this->cpu.flags.AC = psw.test(4);
			this->cpu.flags.Z = psw.test(6);
			this->cpu.flags.S = psw.test(7);

			break;
		}

		//JP adr
		case 0xf2:
		{
			this->cpu.PC += 2;
			if (!this->cpu.flags.S)
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);
			
			break;
		}

		//DI
		case 0xf3:
		{
			this->cpu.interrupt_enabled = false;
			break;
		}

		//CP adr
		case 0xf4:
		{
			this->cpu.PC += 2;
			if (!this->cpu.flags.S)
			{
				this->Push(this->cpu.PC);
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);
			}
			break;
		}

		//PUSH PSW
		case 0xf5:
		{
			this->Push(this->ConvertToNumber(this->cpu.A, this->GetPSW()));
			break;
		}

		//ORI D8
		case 0xf6:
		{
			this->CalculateFlags(this->cpu.A | opcode[1], true, true, false, true, false);
			this->cpu.A = this->cpu.A | opcode[1];
			this->cpu.PC += 1;
			break;
		}

		//RST 6
		case 0xf7:
		{
			this->Push(this->cpu.PC);
			this->cpu.PC = 0x0030;
			break;
		}

		//RM
		case 0xf8:
		{
			if (this->cpu.flags.S)
				this->cpu.PC = this->Pop();

			break;
		}

		//SPHL
		case 0xf9:
		{
			this->cpu.SP = *this->cpu.HL;
			break;
		}

		//JM adr
		case 0xfa:
		{
			this->cpu.PC += 2;
			if (this->cpu.flags.S)
			{
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);
			}

			break;
		}

		//EI
		case 0xfb:
		{
			this->cpu.interrupt_enabled = true;
			break;
		}

		//CM adr
		case 0xfc:
		{
			this->cpu.PC += 2;
			if (this->cpu.flags.S)
			{
				this->Push(this->cpu.PC);
				this->cpu.PC = this->ConvertToNumber(opcode[2], opcode[1]);
			}
			break;
		}

		//NOP
		case 0xfd:
		{
			/*this->UnimplementedInstruction();*/
			break;
		}

		//CPI D8
		case 0xfe:
		{
			this->CalculateFlags(this->cpu.A - opcode[1], true, true, true, true, true);
			this->cpu.PC += 1;
			break;
		}

		//RST 7
		case 0xff:
		{
			this->Push(this->cpu.PC);
			this->cpu.PC = 0x0038;
			break;
		}
	}

	return cycles8080[*opcode];
}

void Emulator8080::UnimplementedInstruction()
{
	Utils::PrintError("Unimplemented instruction: %02x", this->memory[this->cpu.PC - 1]);
}