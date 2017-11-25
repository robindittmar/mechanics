#include "..\..\mechanics\src\XorString.h"
#include "..\..\mechanics\src\murmurhash.h"

void printByteString(FILE* pFile, const char* p, int len)
{
	for (int i = 0; i < len; i++)
		fprintf(pFile, "\\x%02X", (unsigned char)p[i]);

	fputs("\n", pFile);
}

int main(int argc, char** argv)
{
	CXorString strings[]{
		CXorString("\"%s\"\n{\n\t\"$basetexture\" \"vgui/white\"\n\t\"$envmap\" \"\"\n\t\"$model\" \"1\"\n\t\"$flat\" \"%d\"\n\t\"$nocull\" \"0\"\n\t\"$selfillum\" \"1\"\n\t\"$halflambert\" \"1\"\n\t\"$nofog\" \"0\"\n\t\"$ignorez\" \"%d\"\n\t\"$znearer\" \"0\"\n\t\"$wireframe\" \"%d\"\n}"),
		CXorString("VEngineCvar007"),
		CXorString("ü˜…‘Ï¹£›Ìº©™Ã°„šåÞ", 0x1235AFAA),
		CXorString("_cajthRq{nc@vdcmn=67", 0x1A),
		CXorString("“ ‘¹Ÿ““Ÿœ”¯Ÿ–‘‹µ–‹", 0xAFFEAFFE)
	};
	
	CXorString byteStrings[]{
		CXorString("\xA1\x00\x00\x00\x00\x6A\x00\x6A\x00\x6A\x00\x8B\x08\x6A\x00\x50\xFF\x51\x44"),
		CXorString("\xB6\x00\xFD\xFD\xFD\xFD\x38\x00\xA0\xA3\x7F\x50\xF4\xBA\x00\x0C\x43\x00\x3A")
	};
	// This is an array representing the length of the above byte string (they tend to be NOT zero terminated)
	int lenBytes[] {
		19,
		19
	};
	
	int iLen;
	char pBuffer[1024];
	FILE* pFileRead = fopen("plain.txt", "r");
	FILE* pFileWrite = fopen("xor.txt", "w");
	//int countstr = sizeof(strings) / sizeof(CXorString);
	while(fgets(pBuffer, 1024, pFileRead))
	{
		iLen = strlen(pBuffer);
		for(int i = 0; i < iLen; i++)
		{
			if (pBuffer[i] == '\n')
			{
				pBuffer[i] = '\0';
				break;
			}
		}

		fprintf(pFileWrite, "%s\n", CXorString(pBuffer).ToCharArray());
	}
	fclose(pFileRead);
	fclose(pFileWrite);

	pFileWrite = fopen("xor_byte.txt", "w");
	int countbytestr = sizeof(byteStrings) / sizeof(CXorString);
	for (int i = 0; i < countbytestr; i++)
	{
		printByteString(pFileWrite, byteStrings[i].ToCharArray(), lenBytes[i]);
	}
	fclose(pFileWrite);

	uint32_t game_newmap = murmurhash("game_newmap", strlen("game_newmap"), 0xB16B00B5);
	uint32_t cs_game_disconnected = murmurhash("cs_game_disconnected", strlen("cs_game_disconnected"), 0xB16B00B5);
	uint32_t switch_team = murmurhash("switch_team", strlen("switch_team"), 0xB16B00B5);
	uint32_t player_spawned = murmurhash("player_spawned", strlen("player_spawned"), 0xB16B00B5);
	uint32_t player_hurt = murmurhash("player_hurt", strlen("player_hurt"), 0xB16B00B5);
	uint32_t player_death = murmurhash("player_death", strlen("player_death"), 0xB16B00B5);
	uint32_t round_start = murmurhash("round_start", strlen("round_start"), 0xB16B00B5);
	uint32_t round_end = murmurhash("round_end", strlen("round_end"), 0xB16B00B5);

	//fflush(stdin);
	//getchar();

	return 0;
}