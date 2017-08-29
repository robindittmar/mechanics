#ifndef __ICLIENTSTATE_H__
#define __ICLIENTSTATE_H__

#include <cstdint>

class IClientState {
public:
	char		pad[0x9C];
	void*       net_channel;
	char		pad1[0x10];
	uint32_t    retry_number;
	char	    pad2[0x54];
	int         sign_on_state;
	char		pad3[0x8];
	float       next_cmd_time;
	int         server_count;
	int         curseq;
	char		pad4[0x4B];
	int         server_tick;
	int         client_tick;
	int         delta_tick;
	char		pad5[0x4];
	int         view_entity;
	char		pad6[0x8];
	char        level_name[260];
	char        level_name_short[40];
	char		pad7[0x49FD];
	int         last_out_cmd;
	int         choked_cmds;
	int         last_cmd;
	int         cmd;
	int         sound_sequence;
	char		pad8[0x8];
	bool        is_hltv;

	void ForceFullUpdate()
	{
		if (delta_tick != -1)
			delta_tick = -1;
	}
};

#endif // __ICLIENTSTATE_H__