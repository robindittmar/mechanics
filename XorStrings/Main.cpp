#include "..\x88rv2\XorString.h"

void printByteString(const char* p, int len)
{
	for (int i = 0; i < len; i++)
		printf("\\x%02X", p[i]);
}

int main(int argc, char** argv)
{
	CXorString strings[]{
		CXorString("client.dll"),
		CXorString("engine.dll"),
		CXorString("ANë¥~eà{bà¬c;´ö"),
		CXorString("EngineTraceClient004", 0x1A),
		CXorString("_cajthRq{nc@vdcmn=67", 0x1A)
	};

	CXorString byteStrings[] {
		CXorString("\xA1\x00\x00\x00\x00\x6A\x00\x6A\x00\x6A\x00\x8B\x08\x6A\x00\x50\xFF\x51\x44")
	};

	// This is an array representing the length of the above byte string (they tend to be NOT zero terminated)
	int lenBytes[] {
		19
	};

	int countstr = sizeof(strings) / sizeof(CXorString);
	for (int i = 0; i < countstr; i++)
	{
		printf("%s\n", strings[i].Xor());
	}

	int countbytestr = sizeof(byteStrings) / sizeof(CXorString);
	for (int i = 0; i < countbytestr; i++)
	{
		printByteString(byteStrings[i].Xor(), lenBytes[i]);
		puts("\n");
	}

	fflush(stdin);
	getchar();

	return 0;
}