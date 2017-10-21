#ifndef __CONVAR_H_
#define __CONVAR_H_

class ConVar
{
public:
	void SetValue(const char* value);
	void SetValue(float value);
	void SetValue(int value);
	char* GetName();

	char pad1[36];
	char* value;
	char pad2[38];
	int callback;
};

#endif // __CONVAR_H_
