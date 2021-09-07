//#include <limits>
#include <chrono>
#include "Main.h"

/*
	Space Invaders, (C) Taito 1978, Midway 1979    

	CPU: Intel 8080 @ 2MHz (CPU similar to the (newer) Zilog Z80)    

	Interrupts: $cf (RST 8) at the start of vblank, $d7 (RST $10) at the end of vblank.    

	Video: 256(x)*224(y) @ 60Hz, vertical monitor. Colours are simulated with a    
	plastic transparent overlay and a background picture.    
	Video hardware is very simple: 7168 bytes 1bpp bitmap (32 bytes per scanline).    

	Sound: SN76477 and samples.    

	Memory map:    
		ROM    
		$0000-$07ff:    invaders.h    
		$0800-$0fff:    invaders.g    
		$1000-$17ff:    invaders.f    
		$1800-$1fff:    invaders.e    

		RAM    
		$2000-$23ff:    work RAM    
		$2400-$3fff:    video RAM    

		$4000-:     RAM mirror
*/

void RenderText(Emulator8080* emulator, uint16_t fontSize, const char* message, int x = 0, int y = 0)
{
	if (!TTF_WasInit() && TTF_Init() == -1)
	{
		Utils::PrintError("Unable to initialize TTF: %s", TTF_GetError());
		return;
	}

	if(emulator->font == nullptr)
		emulator->font = TTF_OpenFont("Roboto-Light.ttf", fontSize);
	else
	{
		
	}

	SDL_Color White = { 255, 255, 255 };

	// as TTF_RenderText_Solid could only be used on
	// SDL_Surface then you have to create the surface first
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(emulator->font, message, White);

	// now you can convert it into a texture
	SDL_Texture* Message = SDL_CreateTextureFromSurface(emulator->renderer, surfaceMessage);

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = x;  //controls the rect's x coordinate 
	Message_rect.y = y; // controls the rect's y coordinte
	TTF_SizeText(emulator->font, message, &Message_rect.w, &Message_rect.h);

	SDL_RenderCopy(emulator->renderer, Message, NULL, &Message_rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}

void Render(Emulator8080*);
bool FullDraw = false;

uint64_t GetCurrentTimeEx(std::chrono::time_point<std::chrono::high_resolution_clock>* StartTime)
{
	auto TimeDifference = std::chrono::high_resolution_clock::now() - *StartTime;
	return std::chrono::duration_cast<std::chrono::microseconds>(TimeDifference).count(); // Get Microseconds
}

#undef main
int main2()
{
	uint16_t SP = 15;
	uint8_t memory[16];

	SP -= 2;
	*(uint16_t*)&memory[SP] = 257;

	SP -= 2;
	*(uint16_t*)&memory[SP] = 512;

	uint16_t A = *(uint16_t*)&memory[SP];
	SP += 2;

	uint16_t B = *(uint16_t*)&memory[SP];
	SP += 2;

	printf("Shit: %d - %d\n", A, B);
	
	return 0;
}

constexpr double tic = 1000.0 / 60.0; // ms per tic
constexpr int cycles_per_ms = 2000; // 2 Mhz
constexpr double cycles_per_tic = cycles_per_ms * tic;

void ExecuteCycles(Emulator8080* emulator, uint32_t cyc)
{
	//2_000_000 cycles / second
	uint32_t cyc_run = 0;
	while (cyc_run < cyc)
	{
		cyc_run += emulator->ProcessInstruction();
	}
}

void PrintInstructions(Emulator8080* emulator, int opNum = 16)
{
	int num = 0;
	for (size_t i = 0; i < opNum; i++)
	{
		num += Disassemble8080(emulator, num);
	}
}

#define DEBUG_SPACE
int main()
{
	//const char* fileName = "invaders.rom";
	const char* fileName = "CPUTEST.COM";
	Emulator8080* emulator = new Emulator8080(fileName, 0x100);
	emulator->GetCPU()->PC = 0x0100;
	emulator->GetMemory()[0x0005] = 0xC9;

	//emulator->GetMemory()[0] = 0xc3;
	//emulator->GetMemory()[1] = 0;
	//emulator->GetMemory()[2] = 0x01;

	//Fix the stack pointer from 0x6ad to 0x7ad
	// this 0x06 byte 112 in the code, which is    
	// byte 112 + 0x100 = 368 in memory    
	//emulator->GetMemory()[368] = 0x7;

	//Skip DAA test    
	//emulator->GetMemory()[0x59c] = 0xc3; //JMP    
	//emulator->GetMemory()[0x59d] = 0xc2
	//emulator->GetMemory()[0x59e] = 0x05;

	bool quit = false;
	auto cpu = emulator->GetCPU();

#ifndef DEBUG_SPACE
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Utils::PrintError("Failed to initialize the SDL2 library");
		return -1;
	}

	emulator->window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 680, 480, 0);
	emulator->renderer = SDL_CreateRenderer(emulator->window, -1, SDL_RENDERER_ACCELERATED);
	emulator->texture = SDL_CreateTexture(emulator->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, emulator->GameWidth, emulator->GameHeight);

	const uint8_t* Keyboard = SDL_GetKeyboardState(NULL);
#else
	int runNTest = -1;
	int testNum = 0;
	int testSum = 0;
#endif

	while (!quit)
	{

#ifndef DEBUG_SPACE
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				quit = true;

			/*if (event.type == SDL_KEYUP)
			{
				printf("SDL_KEYUP: %d\n", event.key.keysym.sym);

				SDL_Keycode key = event.key.keysym.sym;
				if (key == 118)
				{
					printf("SDL_KEYUP: %d\n", event.key.keysym.sym);
					FILE* file = fopen("dump.vram", "wb");
					if (file == nullptr)
					{
						return -1;
					}

					fwrite(emulator->GetVRAM(), 7 * 1024, 1, file);
					fclose(file);
				}
				if (key == 114)
				{
					printf("SDL_KEYUP: %d\n", event.key.keysym.sym);
					FILE* file = fopen("dump.ram", "wb");
					if (file == nullptr)
					{
						return -1;
					}

					fwrite(emulator->GetRAM(), 7 * 1024, 1, file);
					fclose(file);
				}
			}*/
		}

		cpu->InPort[0] &= 0b10001111;
		cpu->InPort[1] &= 0b10001000;
		cpu->InPort[2] &= 0b10001011;

		if (Keyboard[SDL_SCANCODE_SPACE]) { // Fire
			cpu->InPort[0] |= 1 << 4;
			cpu->InPort[1] |= 1 << 4;
			cpu->InPort[2] |= 1 << 4; // P2
		}

		if (Keyboard[SDL_SCANCODE_A] || Keyboard[SDL_SCANCODE_LEFT]) { // Left
			cpu->InPort[0] |= 1 << 5;
			cpu->InPort[1] |= 1 << 5;
			cpu->InPort[2] |= 1 << 5; // P2
		}

		if (Keyboard[SDL_SCANCODE_D] || Keyboard[SDL_SCANCODE_RIGHT]) { // Right
			cpu->InPort[0] |= 1 << 6;
			cpu->InPort[1] |= 1 << 6;
			cpu->InPort[2] |= 1 << 6; // P2
		}

		if (Keyboard[SDL_SCANCODE_RETURN]) // Credit
			cpu->InPort[1] |= 1 << 0;

		if (Keyboard[SDL_SCANCODE_1]) // 1P Start
			cpu->InPort[1] |= 1 << 2;

		if (Keyboard[SDL_SCANCODE_2]) // 2P Start
			cpu->InPort[1] |= 1 << 1;

		if (Keyboard[SDL_SCANCODE_DELETE]) // Tilt
			cpu->InPort[2] |= 1 << 2;

		SDL_RenderClear(emulator->renderer);

		//ExecuteCycles(emulator, cycles_per_tic / 2);
		ExecuteCycles(emulator, 500000);
		emulator->GenerateInterrupt(1);
		//ExecuteCycles(emulator, cycles_per_tic / 2);
		ExecuteCycles(emulator, 500000);
		{
			Render(emulator);
			SDL_RenderPresent(emulator->renderer);
		}
		emulator->GenerateInterrupt(2);
#else
		
		emulator->ProcessInstruction();

		if (emulator->GetCPU()->PC == 0x0005) {
			if (emulator->GetCPU()->C == 9) {
				uint16_t addr = (emulator->GetCPU()->D << 8) | emulator->GetCPU()->E;

				uint8_t byte;
				while ((byte = emulator->GetMemory()[addr]) != '$') {
					printf("%c", byte);
					++addr;
				}
			}
			else if (emulator->GetCPU()->C == 2) {
				printf("%c", emulator->GetCPU()->E);
			}
		}

		if (emulator->GetCPU()->PC == 0) {
			printf("\nGot to 0000.");
			printf("\nDone instructions.");
			break;
		}
		/*while (testNum < runNTest)
		{
			cpu->cycles += emulator->ProcessInstruction();
			testNum++;
		}

		testSum += runNTest;

		system("cls");

		char buffer[512];
		sprintf_s(buffer, 512, ""
			"Opcode: %02X\n"
			"\n"
			"Last test num: %d\n"
			"Test sum: %d\n"
			"Registers:\n"
			"\tAF: %04X\n"
			"\tBC: %04X\n"
			"\tDE: %04X\n"
			"\tHL: %04X\n"
			"\n"
			"Program counters:\n"
			"\tPC: %04X\n"
			"\tSP: %04X\n"
			"\tFlags: %c%c%c%c%c\n"
			"\tCycles: %d\n\n",
			emulator->GetMemory()[cpu->PC - 1],
			runNTest,
			testSum,
			(cpu->A << 8) | emulator->GetPSW(),
			*cpu->BC,
			*cpu->DE,
			*cpu->HL,
			cpu->PC,
			cpu->SP,
			cpu->flags.Z ? 'z' : '.', cpu->flags.S?'s':'.', cpu->flags.P ? 'p' : '.', cpu->interrupt_enabled ? 'i' : '.', cpu->flags.C ? 'c' : '.',//.sp.c
			cpu->cycles);

		Utils::PrintDebug(buffer);

		PrintInstructions(emulator);

		testNum = 0;
		runNTest = -1;

	test:
		if (runNTest == -1)
		{
			printf("Please enter N test: ");
			scanf("%d", &runNTest);

			if (runNTest == -1)
			{
				delete emulator;
				emulator = new Emulator8080(fileName);

				testSum = 0;

				goto test;
			}
		}*/

		//Render(emulator);
#endif // !DEBUG_SPACE
	}

#ifndef DEBUG_SPACE
	SDL_DestroyRenderer(emulator->renderer);
	SDL_DestroyWindow(emulator->window);
#endif

	system("pause");
	return 0;
}

char buffer[512];
void Render(Emulator8080* emulator)
{
	auto cpu = emulator->GetCPU();
	auto vram = emulator->GetVRAM();
	
	int windowWidth, windowHeight;
	SDL_GetWindowSize(emulator->window, &windowWidth, &windowHeight);

#ifndef DEBUG_SPACE
	uint8_t* pixels;
	int pitch;
	SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

	SDL_LockTexture(emulator->texture, NULL, (void**)&pixels, &pitch);

	uint32_t WHITE_COLOR = SDL_MapRGBA(format, 255, 255, 255, 255);
	uint32_t BLACK_COLOR = SDL_MapRGBA(format, 0, 0, 0, 255);

	uint64_t pixelCount = 0;
	for (size_t y = 0; y < emulator->GameHeight; y += 8)
	{
		for (size_t x = 0; x < emulator->GameWidth; x++)
		{
			int index = x * 4 + y * pitch;
			uint32_t* p = (uint32_t*)&pixels[index];
			uint8_t pi = vram[(x * (256 / 8)) + y / 8];

			for (short i = 0; i < 8; i++)
			{
				*p = pi & (1 << i) ? WHITE_COLOR : BLACK_COLOR;
				p += 224;
			}
		}
	}

	SDL_UnlockTexture(emulator->texture);

	/*SDL_LockTexture(emulator->texture, NULL, (void**)&pixels, &pitch);

	uint32_t WHITE_COLOR = SDL_MapRGBA(format, 255, 255, 255, 255);
	uint32_t BLACK_COLOR = SDL_MapRGBA(format, 0, 0, 0, 255);

	uint64_t pixelCount = 0;
	for (size_t y = 0; y < emulator->GameHeight; y += 8)
	{
		for (size_t x = 0; x < emulator->GameWidth; x++)
		{
			int index = x * 4 + y * pitch;
			uint32_t* p = (uint32_t*)&pixels[index];
			uint8_t pi = vram[(x * (256 / 8)) + y / 8];

			for (short i = 0; i < 8; i++)
			{
				*p = pi & (1 << i) ? WHITE_COLOR : BLACK_COLOR;
				p += 224;

				pixelCount++;
			}
		}
	}

	SDL_UnlockTexture(emulator->texture);*/

	SDL_Rect rect;
	rect.x = windowWidth/2 - emulator->GameWidth/2;
	rect.y = windowHeight/2 - emulator->GameHeight/2;
	rect.w = emulator->GameWidth;
	rect.h = emulator->GameHeight;

	SDL_RenderCopyEx(emulator->renderer, emulator->texture, NULL, &rect, 0.0, NULL, SDL_RendererFlip::SDL_FLIP_VERTICAL);
	SDL_FreeFormat(format);
#else
	sprintf_s(buffer, 512, "Opcode: %02x | AF: %04x | BC: %04x | DE: %04x | HL: %04x | PC: %04x | SP: %04x",
		emulator->GetMemory()[cpu->PC - 1], (cpu->A << 8) | emulator->GetPSW(), *cpu->BC, *cpu->DE, *cpu->HL, cpu->PC, cpu->SP);

	int textWidth = 0, textHeight = 0;
	TTF_SizeText(emulator->font, buffer, &textWidth, &textHeight);

	RenderText(emulator, 14, buffer, windowWidth/2 - textWidth/2, 10);
#endif
}