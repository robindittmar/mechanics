#ifndef __PLAYERINFO_H__
#define __PLAYERINFO_H__

#include <cstdlib>
#include <cstring>

typedef long long int64_t;

class PlayerInfo
{
public:
	int GetName(wchar_t* p, int len);

	int64_t __pad0;
	union {
		int64_t xuid;
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char name[128];
	int userid;
	char guid[33];
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};

#endif // __PLAYERINFO_H__