#ifndef __ESP_H__
#define __ESP_H__

#include "d3d9.h"
#include "d3dx9.h"

#include "Feature.h"
#include "Vector.h"
#include "ISurface.h"

#define EnableHealthbar true
#define EnableArmorbar false

#define CT_TEAMID 3
#define T_TEAMID 2

class CApplication;

class CEsp : public IFeature
{
public:
	CEsp();
	~CEsp();

	void ShouldDrawBoundingBox(bool bDrawBoundingBox) { m_bDrawBoundingBox = bDrawBoundingBox; };
	bool ShouldDrawBoundingBox() { return m_bDrawBoundingBox; };

	void ShouldDrawHealthBar(bool bDrawHealthBar) { m_bDrawHealthBar = bDrawHealthBar; };
	bool ShouldDrawHealthBar() { return m_bDrawHealthBar; };

	void ShouldDrawArmorBar(bool bDrawArmorBar) { m_bDrawArmorBar = bDrawArmorBar; };
	bool ShouldDrawArmorBar() { return m_bDrawArmorBar; };

	void ShouldDrawOwnTeam(bool bDrawOwnTeam) { m_bDrawOwnTeam = bDrawOwnTeam; };
	bool ShouldDrawOwnTeam() { return m_bDrawOwnTeam; };

	void ShouldDrawOwnModel(bool bDrawOwnModel) { m_bDrawOwnModel = bDrawOwnModel; };
	bool ShouldDrawOwnModel() { return m_bDrawOwnModel; };

	void ShouldDrawOnlySpotted(bool bDrawOnlySpotted) { m_bDrawOnlySpotted = bDrawOnlySpotted; };
	bool ShouldDrawOnlySpotted() { return m_bDrawOnlySpotted; };

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	void DrawArmorBar(int posX, int posY, int height, int width, int armor);
	void DrawBoundingBox(int posX, int posY, int height, int width, Color color);
	void DrawHealthBar(int posX, int posY, int height, int width, int health);
	void DrawHelmet(int posX, int posY, int height, int width);

	bool ScreenTransform(const Vector& point, Vector& screen);
	bool WorldToScreen(const Vector &origin, Vector &screen);

	bool m_bDrawBoundingBox;
	bool m_bDrawHealthBar;
	bool m_bDrawArmorBar;
	bool m_bDrawOwnTeam;
	bool m_bDrawOwnModel;
	bool m_bDrawOnlySpotted;
};

#endif // __ESP_H__