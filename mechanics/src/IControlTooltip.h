#ifndef __ICONTROLTOOLTIP_H__
#define __ICONTROLTOOLTIP_H__

// Std Lib

// Source SDK

// Custom
#include "Tooltip.h"
#include "IControlClickable.h"

#define TOOLTIP_DISTANCE_TO_MOUSE_X		15
#define TOOLTIP_DISTANCE_TO_MOUSE_Y		20

#define TOOLTIP_TIME_TO_DISPLAY			0.5f

class IControlTooltip : public IControlClickable
{
public:
	IControlTooltip(int x = 0, int y = 0, int w = 0, int h = 0);
	virtual ~IControlTooltip();

	/// <summary>
	/// Sets the tooltip text (creates CTooltip instance & takes care of displaying)
	/// ==> Only invoke this method to set the text, everything else is done by IControlTooltip
	/// </summary>
	/// <param name="p">The text to display inside the Tooltip</param>
	void SetTooltipText(const char* pTooltipText);

	virtual void OnMouseMove(int mx, int my) override;
	virtual void Draw(ISurface* pSurface) override;
protected:
	float m_fTimeLastMouseMovement;
	CTooltip* m_pTooltip;
};

#endif // __ICONTROLTOOLTIP_H__