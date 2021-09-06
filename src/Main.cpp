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

void RenderText(SDL_Renderer* renderer, uint16_t fontSize, const char* message, int x = 0, int y = 0, int width = 0, int height = 0)
{
	if (!TTF_WasInit() && TTF_Init() == -1)
	{
		Utils::PrintError("Unable to initialize TTF: %s", TTF_GetError());
		return;
	}

	TTF_Font* font = TTF_OpenFont("Roboto-Light.ttf", fontSize);
	SDL_Color White = { 255, 255, 255 };

	// as TTF_RenderText_Solid could only be used on
	// SDL_Surface then you have to create the surface first
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, message, White);

	// now you can convert it into a texture
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = x;  //controls the rect's x coordinate 
	Message_rect.y = y; // controls the rect's y coordinte
	Message_rect.w = width; // controls the width of the rect
	Message_rect.h = height; // controls the height of the rect

	TTF_SizeText(font, message, &Message_rect.w, &Message_rect.h);

	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
	TTF_CloseFont(font);
}

void Render(Emulator8080*);
bool FullDraw = false;

uint64_t GetCurrentTimeEx(std::chrono::time_point<std::chrono::high_resolution_clock>* StartTime)
{
	auto TimeDifference = std::chrono::high_resolution_clock::now() - *StartTime;
	return std::chrono::duration_cast<std::chrono::microseconds>(TimeDifference).count(); // Get Microseconds
}

#undef main
int main()
{
	const char* fileName = "invaders.rom";
	Emulator8080* emulator = new Emulator8080(fileName);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Utils::PrintError("Failed to initialize the SDL2 library");
		return -1;
	}

	emulator->window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 680, 480, 0);
	emulator->renderer = SDL_CreateRenderer(emulator->window, -1, SDL_RENDERER_ACCELERATED);
	emulator->texture = SDL_CreateTexture(emulator->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, emulator->GameWidth, emulator->GameHeight);

	bool quit = false;
	
	auto cpu = emulator->GetCPU();

	auto lastInterrupt = time(NULL);

	auto StartTime = std::chrono::high_resolution_clock::now();
	auto lastInterrupt2 = GetCurrentTimeEx(&StartTime);
	uint64_t LastDraw = 0;

	const uint8_t* Keyboard = SDL_GetKeyboardState(NULL);

	while (!quit)
	{
		if (cpu->SP < 0x2000)
		{
			quit = true;
			Utils::PrintError("Stack Pointer invalid address");
			break;
		}
		/*if (nrun >= next)
		{
			wait = false;
			nrun = 0;
			next = -1;

			printf("A: %02x | AF: %04x | BC: %04x | DE: %04x | HL: %04x | PC: %04x | SP: %04x\n", cpu->A, (cpu->A << 8) | emulator->GetPSW(), *cpu->BC, *cpu->DE, *cpu->HL, cpu->PC, cpu->SP);
		}

		if (!wait)
		{
			printf("Enter N run: ");
			scanf("%d", &next);
			wait = true;
		}*/

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

		//SDL_RenderClear(emulator->renderer);
		//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//SDL_RenderFillRect(renderer, NULL);

		emulator->ProcessInstruction();

		//if (GetCurrentTimeEx(&StartTime) - lastInterrupt2 > ((1.0 / 60)))
		if((GetCurrentTimeEx(&StartTime) - lastInterrupt2) > (1000000 / 120))
		{
			//if (cpu->interupt_enabled)
			{
				if (FullDraw)
				{
					emulator->GenerateInterrupt(2);

					Render(emulator);
					SDL_RenderPresent(emulator->renderer);
					SDL_Delay(0);
				}
				//else
					//emulator->GenerateInterrupt(1);

				lastInterrupt2 = GetCurrentTimeEx(&StartTime);
				FullDraw = !FullDraw;
			}
		}

		//Render(window, emulator);

		//SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(emulator->renderer);
	SDL_DestroyWindow(emulator->window);

	/*while (true)
	{
		emulator->ProcessInstruction();
		std::cin.ignore(1000, '\n');
	}*/

	//printf("S: %s\n", std::bitset<8>(Utils::LeftRotate(hi, 1)).to_string().c_str());
	//printf("S: %d\n", hi & (1 << 7));
	

	system("pause");
	return 0;
}

char buffer[512];
void Render(Emulator8080* emulator)
{
	auto cpu = emulator->GetCPU();
	auto vram = emulator->GetVRAM();

	//sprintf_s(buffer, 512, "Opcode: %02x | AF: %04x | BC: %04x | DE: %04x | HL: %04x | PC: %04x | SP: %04x",
		//emulator->GetMemory()[cpu->PC - 1], (cpu->A << 8) | emulator->GetPSW(), *cpu->BC, *cpu->DE, *cpu->HL, cpu->PC, cpu->SP);

	//RenderText(renderer, 14, buffer, 0, 0, 96, 24);
	
	uint8_t* pixels;
	int pitch;
	SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

	SDL_LockTexture(emulator->texture, NULL, (void**)&pixels, &pitch);

	uint32_t WHITE_COLOR = SDL_MapRGBA(format, 255, 255, 255, 255);
	uint32_t BLACK_COLOR = SDL_MapRGBA(format, 0, 0, 0, 255);

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

	int gW, gH;
	SDL_GetWindowSize(emulator->window, &gW, &gH);

	SDL_Rect rect;
	rect.x = gW/2 - emulator->GameWidth/2;
	rect.y = gH/2 - emulator->GameHeight/2;
	rect.w = emulator->GameWidth;
	rect.h = emulator->GameHeight;

	SDL_RenderCopyEx(emulator->renderer, emulator->texture, NULL, &rect, 0.0, NULL, SDL_RendererFlip::SDL_FLIP_VERTICAL);
	SDL_FreeFormat(format);
}