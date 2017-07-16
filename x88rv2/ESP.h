#ifndef __ESP_H__
#define __ESP_H__

#include "d3d9.h"
#include "d3dx9.h"

#include "Feature.h"

#include "Vector.h"

#define HEALTH_OFFSET 0xFC
#define TEAM_OFFSET 0xF0
#define NAME_OFFSET 0x150
#define CROUCHED_OFFSET 0x
#define SPOTTED_OFFSET 0x939

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
	void CEsp::DrawBoundingBox(IDirect3DDevice9* pDevice, int posX, int posY, int height, int width, D3DCOLOR color);
	bool ScreenTransform(const Vector& point, Vector& screen);
	bool WorldToScreen(const Vector &origin, Vector &screen);

	CApplication* m_pApp;
};

#endif // __ESP_H__