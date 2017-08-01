#ifndef __USERCMD_H__
#define __USERCMD_H__

struct CUserCmd {
	virtual ~CUserCmd() {};
	int command_number;
	int tick_count;
	float viewangles[3];
	float aimdirection[3];
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	unsigned char impulse;
	int weaponselect;
	int weaponsubtype;
	int random_seed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
	float headangles[3];
	float headoffset[3];
};

#endif // __USERCMD_H__