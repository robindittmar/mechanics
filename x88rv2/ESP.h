#ifndef __ESP_H__
#define __ESP_H__

#include "d3d9.h"
#include "d3dx9.h"

#include "Feature.h"

#include "Vector.h"

#define ARMOR_OFFSET 0xB248
#define HEALTH_OFFSET 0xFC
#define HELMET_OFFSET 0xB23C
#define NAME_OFFSET 0x150
#define SPOTTED_OFFSET 0x939
#define TEAM_OFFSET 0xF0

#define MODELINDEX_OFFSET 0x254

#define CT_TEAMID 3
#define T_TEAMID 2

class CApplication;

class CEsp : public IFeature
{
public:
	CEsp();
	~CEsp();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	void DrawArmorBar(IDirect3DDevice9* pDevice, int posX, int posY, int height, int width, int armor);
	void DrawBoundingBox(IDirect3DDevice9* pDevice, int posX, int posY, int height, int width, D3DCOLOR color);
	void DrawHealthBar(IDirect3DDevice9* pDevice, int posX, int posY, int height, int width, int health);
	void DrawHelmet(IDirect3DDevice9* pDevice, int posX, int posY, int height, int width);

	bool ScreenTransform(const Vector& point, Vector& screen);
	bool WorldToScreen(const Vector &origin, Vector &screen);

	CApplication* m_pApp;
};

#endif // __ESP_H__