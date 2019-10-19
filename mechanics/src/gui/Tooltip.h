#ifndef __TOOLTIP_H__
#define __TOOLTIP_H__

#include "../source_sdk/ISurface.h"

#define TOOLTIP_PADDING_TO_TEXT		4

const Color g_clrTooltipBackground(255, 60, 60, 60);
const Color g_clrTooltipBorder(255, 255, 128, 0);

class CLabel;

class CTooltip
{
public:
	CTooltip();
	~CTooltip();

	void SetText(const char* pText);
	void SetPosition(int x, int y);
	
	void Draw(ISurface* pSurface);
private:
	int m_iX, m_iY;
	CLabel* m_pLabel;
};

#endif // __TOOLTIP_H__