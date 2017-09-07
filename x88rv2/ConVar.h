#ifndef __ICVAR_H__
#define __ICVAR_H__

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

#endif // __ICVAR_H__
