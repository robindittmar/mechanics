#ifndef __ICONTROL_H__
#define __ICONTROL_H__

// Std libs
#include <vector>

// Resources
#include "ResourceManager.h"

// Event
#include "InputEvent.h"

// GUI Stuff (rendering etc)
#include "Gui.h"

/*
 * TODO:	- Hover effects?
 *			- Text input? (textboxes for example)
 *			- Listbox <= Might be a little more important
 *			- ???
 */

const Color g_clrControlDisabled(255, 100, 100, 100);

class IControl
{
public:
	IControl(int x = 0, int y = 0, int w = 0, int h = 0);
	~IControl();

	// Internal control events
	virtual void OnMouseMove(int mx, int my);
	virtual void OnMouseDown(int mx, int my);
	virtual void OnMouseUp(int mx, int my);
	virtual void OnClicked();

	/// <summary>
	/// Adds child to container,
	/// the container will take care of cleaning up the child controls
	/// </summary>
	virtual void AddChild(IControl* pControl);

	/// <summary>
	/// Returns the absolute position of the control
	/// (x and y MUST be initialized with 0 before invoking this method)
	/// </summary>
	virtual void GetAbsolutePosition(int* pX, int* pY);

	// ProcessEvent & Draw
	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);

	IControl* GetParentWindow();

	void SetVisible(bool bIsVisible) { m_bIsVisible = bIsVisible; }
	bool GetVisible() { return m_bIsVisible; }

	virtual void SetEnabled(bool bIsEnabled) { m_bIsEnabled = bIsEnabled; }
	bool GetEnabled() { return m_bIsEnabled; }

	void SetPosition(int x, int y);
	void SetBoundaries(int x, int y, int w, int h);
	int X() { return m_iX; }
	int Y() { return m_iY; }
	int Width() { return m_iWidth; }
	int Height() { return m_iHeight; }

	void Parent(IControl* pParent) { m_pParent = pParent; }
	IControl* Parent() { return m_pParent; }
protected:
	bool m_bIsVisible;
	bool m_bIsEnabled;
	int m_iX;
	int m_iY;
	int m_iWidth;
	int m_iHeight;
	IControl* m_pParent;
	std::vector<IControl*> m_pChildren;

	bool m_bHitcheckForMouseMove;
	bool m_bHitcheckForMouseUp;

	bool m_bMouseOver;
	bool m_bMouseDown;
};

typedef void(*SimpleEventHandler_t)(IControl*);

#endif // __ICONTROL_H__