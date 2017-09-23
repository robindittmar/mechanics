#ifndef __ESP_H__
#define __ESP_H__

// Std lib
#include <assert.h>
#include <vector>

// Source SDK
#include "Vector.h"
#include "ISurface.h"
#include "ClientEntity.h"

// Custom
#include "IFeature.h"
#include "Gui.h"

#define ESP_STYLE_NONE			0
#define ESP_STYLE_EDGE			1
#define ESP_STYLE_FULL			2

class CEsp : public IFeature
{
public:
	CEsp();
	~CEsp();

	void SetDrawBoundingBox(int iDrawBoundingBox) { m_iDrawBoundingBox = iDrawBoundingBox; }
	int GetDrawBoundingBox() { return m_iDrawBoundingBox; }

	void SetDrawHealthBar(bool bDrawHealthBar) { m_bDrawHealthBar = bDrawHealthBar; }
	bool GetDrawHealthBar() { return m_bDrawHealthBar; }

	void SetDrawHealthNumber(bool bDrawHealthNumber) { m_bDrawHealthNumber = bDrawHealthNumber; }
	bool GetDrawHealthNumber() { return m_bDrawHealthNumber; }

	void SetDrawArmorBar(bool bDrawArmorBar) { m_bDrawArmorBar = bDrawArmorBar; }
	bool GetDrawArmorBar() { return m_bDrawArmorBar; }

	void SetDrawOwnTeam(bool bDrawOwnTeam) { m_bDrawOwnTeam = bDrawOwnTeam; }
	bool GetDrawOwnTeam() { return m_bDrawOwnTeam; }

	void SetDrawOwnModel(bool bDrawOwnModel) { m_bDrawOwnModel = bDrawOwnModel; }
	bool GetDrawOwnModel() { return m_bDrawOwnModel; }

	void SetDrawOnlyVisible(bool bDrawOnlyVisible) { m_bDrawOnlyVisible = bDrawOnlyVisible; }
	bool GetDrawOnlyVisible() { return m_bDrawOnlyVisible; }

	void SetDrawOnlySpotted(bool bDrawOnlySpotted) { m_bDrawOnlySpotted = bDrawOnlySpotted; }
	bool GetDrawOnlySpotted() { return m_bDrawOnlySpotted; }

	void SetDrawOutline(bool bDrawOutlineEsp) { m_bDrawOutline = bDrawOutlineEsp; }
	bool GetDrawOutline() { return m_bDrawOutline; }

	void SetDrawNames(bool bDrawNames) { m_bDrawNames = bDrawNames; }
	bool GetDrawNames() { return m_bDrawNames; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	void DrawArmorBar(int posX, int posY, int height, int width, int armor);
	void DrawBoundingBox(int posX, int posY, int height, int width, Color color);
	void DrawHealthBar(ISurface* pSurface, int posX, int posY, int height, int width, int health);
	void DrawHealthNumber(int posX, int posY, int height, int width, int health);
	void DrawHelmet(int posX, int posY, int height, int width);
	void DrawName(IClientEntity* pEntity, int posX, int posY, int height, int width);

	int m_iDrawBoundingBox;

	bool m_bDrawHealthBar;
	bool m_bDrawHealthNumber;
	bool m_bDrawArmorBar;
	bool m_bDrawOwnTeam;
	bool m_bDrawOwnModel;
	bool m_bDrawOnlyVisible;
	bool m_bDrawOnlySpotted;
	bool m_bDrawOutline;
	bool m_bDrawNames;

	int m_iFadeoutTime;

	CGui* m_pGui;
};

#endif // __ESP_H__