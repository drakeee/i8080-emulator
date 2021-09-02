#pragma once

class Utils
{
public:
	static void Print(std::stringstream &color, const char* format, ...)
	{
		va_list argList;

		std::stringstream ss;
		termcolor::colorize(ss);
		ss << termcolor::grey << termcolor::on_bright_red << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(5) << "ERROR" << termcolor::reset << " " << format << std::endl;

		va_start(argList, format);
		vprintf(ss.str().c_str(), argList);
		va_end(argList);
	}

	static void PrintError(const char* format, ...)
	{
		va_list argList;

		std::stringstream ss;
		termcolor::colorize(ss);
		ss << termcolor::grey << termcolor::on_bright_red << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(5) << "ERROR" << termcolor::reset << " " << format << std::endl;

		va_start(argList, format);
		vprintf(ss.str().c_str(), argList);
		va_end(argList);
	}

	static void PrintInfo(const char* format, ...)
	{
		va_list argList;

		std::stringstream ss;
		termcolor::colorize(ss);
		ss << termcolor::grey << termcolor::on_bright_blue << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(5) << "INFO" << termcolor::reset << " " << format << std::endl;

		va_start(argList, format);
		vprintf(ss.str().c_str(), argList);
		va_end(argList);
	}

	static void PrintDebug(const char* format, ...)
	{
		va_list argList;

		std::stringstream ss;
		termcolor::colorize(ss);
		ss << termcolor::grey << termcolor::on_bright_grey << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(5) << "DEBUG" << termcolor::reset << " " << format << std::endl;

		va_start(argList, format);
		vprintf(ss.str().c_str(), argList);
		va_end(argList);
	}

	static void HexDump(const char* title, const unsigned char* data, uint32_t size = 8192, uint16_t bytesPerLine = 16, uint16_t startIndex = 0)
	{
		printf("Dump %s\n", title);

		int numBytes = 1;
		for (size_t i = startIndex; i < (startIndex + size); i++)
		{
			unsigned char byte = data[i];

			if (numBytes == 1)
				printf("%04x\t", i);

			printf("%02x ", byte);

			numBytes++;
			if (numBytes > bytesPerLine)
			{
				printf("\t");
				for (size_t bufIndex = 0; bufIndex < bytesPerLine; bufIndex++)
				{
					printf("%c", isprint(data[(i - bytesPerLine + 1) + bufIndex]) ? data[(i - bytesPerLine + 1) + bufIndex] : '.');
				}

				printf("\n");
				numBytes = 1;
			}
		}
	}

	template<typename T>
	static int LeftRotate(T n, unsigned int d)
	{

		/* In n<<d, last d bits are 0. To
		put first 3 bits of n at
		last, do bitwise or of n<<d
		with n >>(INT_BITS - d) */
		return (n << d) | (n >> ((sizeof T * 8) - d));
	}

	template<typename T>
	static int RightRotate(T n, unsigned int d)
	{
		/* In n>>d, first d bits are 0.
		To put last 3 bits of at
		first, do bitwise or of n>>d
		with n <<(INT_BITS - d) */
		return (n >> d) | (n << ((sizeof T * 8) - d));
	}
};

#define hexdumprom(x) Utils::HexDump("ROM",x,8192,16,0x0000)
#define hexdumpram(x) Utils::HexDump("RAM",x,1024,16,0x2000)
#define hexdumpvram(x) Utils::HexDump("VRAM",x,7168,16,0x2400)