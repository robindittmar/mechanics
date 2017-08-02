#ifndef __ICONTROL_H__
#define __ICONTROL_H__

// DirectX
#include "d3d9.h"
#include "d3dx9.h"

class IControl
{
public:
	IControl(int x = 0, int y = 0, int w = 0, int h = 0);
	~IControl();

	// TODO: ProcessEvents
	virtual void ProcessEvents() = 0;
	virtual void Draw(IDirect3DDevice9* pDevice, int xOff, int yOff) = 0;

	void IsEnabled(bool bIsEnabled) {
		m_bIsEnabled = bIsEnabled;
	}

	bool IsEnabled() {
		return m_bIsEnabled;
	}
protected:
	bool m_bIsEnabled;
	int m_iX;
	int m_iY;
	int m_iWidth;
	int m_iHeight;
};

#endif // __ICONTROL_H__