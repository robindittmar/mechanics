#include "..\x88rv2\XorString.h"
#include "..\x88rv2\murmurhash.h"

void printByteString(const char* p, int len)
{
	for (int i = 0; i < len; i++)
		printf("\\x%02X", p[i]);
}

int main(int argc, char** argv)
{
	CXorString strings[]{
		CXorString("models/player"),
		CXorString("cl_mouseenable 0"),
		CXorString("cl_mouseenable 1"),
		CXorString("tgÚ¯x~ö§reä {n¥ò"),
		CXorString("tgÚ¯x~ö§reä {n¥ó"),
		CXorString("VEngineRenderView014"),
		// "VertexLitGeneric" : "UnlitGeneric"
		CXorString("VertexLitGeneric"),
		CXorString("UnlitGeneric"),
		CXorString("An÷¶rsÉ«cLà¬ryì¡"),
		CXorString("Beé«cLà¬ryì¡"),
		CXorString("xhmat_%d.vmt"),
		CXorString("Counter-Strike: Global Offensive"),
		CXorString("Tdð¬cn÷ïD÷«|n¿âPgê vg¥qmà¬dbó§")
	};
	
	CXorString byteStrings[] {
		CXorString("\xA1\x00\x00\x00\x00\x6A\x00\x6A\x00\x6A\x00\x8B\x08\x6A\x00\x50\xFF\x51\x44")
	};
	// This is an array representing the length of the above byte string (they tend to be NOT zero terminated)
	int lenBytes[] {
		19
	};

	FILE* fp = fopen("xor.txt", "w");
	int countstr = sizeof(strings) / sizeof(CXorString);
	for (int i = 0; i < countstr; i++)
	{
		fprintf(fp, "%s\n", strings[i]);
	}
	fclose(fp);

	int countbytestr = sizeof(byteStrings) / sizeof(CXorString);
	for (int i = 0; i < countbytestr; i++)
	{
		printByteString(byteStrings[i].Xor(), lenBytes[i]);
		puts("\n");
	}

	uint32_t player_hurt = murmurhash("player_hurt", strlen("player_hurt"), 0xB16B00B5);
	uint32_t player_death = murmurhash("player_death", strlen("player_death"), 0xB16B00B5);
	uint32_t round_start = murmurhash("round_start", strlen("round_start"), 0xB16B00B5);
	uint32_t round_end = murmurhash("round_end", strlen("round_end"), 0xB16B00B5);

	fflush(stdin);
	getchar();

	return 0;
}