#include "..\x88rv2\XorString.h"
#include "..\x88rv2\murmurhash.h"

void printByteString(FILE* pFile, const char* p, int len)
{
	for (int i = 0; i < len; i++)
		fprintf(pFile, "\\x%02X", (unsigned char)p[i]);

	fputs("\n", pFile);
}

int main(int argc, char** argv)
{
	CXorString strings[]{
		CXorString("m_AttributeManager"),
		CXorString("m_Item"),
		CXorString("m_iItemDefinitionIndex"),
		CXorString("m_iClip1"),
		CXorString("m_iClip2"),
		CXorString("LocalActiveWeaponData"),
		CXorString("m_flNextPrimaryAttack"),
		CXorString("m_fAccuracyPenalty"),
		CXorString("m_flPostponeFireReadyTime"),
		CXorString("m_fThrowTime"),
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

	FILE* fp = fopen("xor.txt", "w");
	int countstr = sizeof(strings) / sizeof(CXorString);
	for (int i = 0; i < countstr; i++)
	{
		fprintf(fp, "%s\n", strings[i].ToCharArray());
	}
	fclose(fp);

	fp = fopen("xor_byte.txt", "w");
	int countbytestr = sizeof(byteStrings) / sizeof(CXorString);
	for (int i = 0; i < countbytestr; i++)
	{
		printByteString(fp, byteStrings[i].ToCharArray(), lenBytes[i]);
	}
	fclose(fp);

	uint32_t player_hurt = murmurhash("player_hurt", strlen("player_hurt"), 0xB16B00B5);
	uint32_t player_death = murmurhash("player_death", strlen("player_death"), 0xB16B00B5);
	uint32_t round_start = murmurhash("round_start", strlen("round_start"), 0xB16B00B5);
	uint32_t round_end = murmurhash("round_end", strlen("round_end"), 0xB16B00B5);

	//fflush(stdin);
	//getchar();

	return 0;
}