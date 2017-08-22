#ifndef __ICONTROL_H__
#define __ICONTROL_H__

// Std libs
#include <vector>

// Resources
#include "ResourceManager.h"

// Event
#include "InputEvent.h"

// SDK Render utils
#include "ISurface.h"

/*
 * TODO:	- Hover effects?
 *			- Text input? (textboxes for example)
 *			- Listbox <= Might be a little more important
 *			- ???
 */

class IControl
{
public:
	IControl(int x = 0, int y = 0, int w = 0, int h = 0);
	~IControl();

	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);

	/// <summary>
	/// Adds child to container,
	/// the container will take care of cleaning up the child controls
	/// </summary>
	bool AddChild(IControl* pControl);

	/// <summary>
	/// Returns the absolute position of the control
	/// (x and y MUST be initialized with 0 before invoking this method)
	/// </summary>
	virtual void GetAbsolutePosition(int* pX, int* pY);

	void IsEnabled(bool bIsEnabled) { m_bIsEnabled = bIsEnabled; }
	bool IsEnabled() { return m_bIsEnabled; }

	int X() { return m_iX; }
	int Y() { return m_iY; }
	int Width() { return m_iWidth; }
	int Height() { return m_iHeight; }

	void Parent(IControl* pParent) { m_pParent = pParent; }
	IControl* Parent() { return m_pParent; }
protected:
	bool m_bIsEnabled;
	int m_iX;
	int m_iY;
	int m_iWidth;
	int m_iHeight;
	IControl* m_pParent;
	std::vector<IControl*> m_pChildren;
};

typedef void(*SimpleEventHandler_t)(IControl*);

#endif // __ICONTROL_H__