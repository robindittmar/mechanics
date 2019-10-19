#ifndef __ICONTROL_H__
#define __ICONTROL_H__

// Std Lib
#include <assert.h>
#include <vector>
#include <unordered_set>

// Source SDK

// Custom
#include "../core/ResourceManager.h"
#include "InputEvent.h"
#include "Gui.h"
#include "IDependable.h"

const Color g_clrControlDisabled(255, 100, 100, 100);

class CWindow;

class IControl : public IDependable
{
public:
	IControl(int x = 0, int y = 0, int w = 0, int h = 0);
	virtual ~IControl();

	/// <summary>
	/// Adds child to container,
	/// the container will take care of cleaning up the child controls
	/// </summary>
	virtual void AddChild(IControl* pControl);

	/// <summary>
	/// Recursive function to retrieve the topmost IControl (which should be a CWindow)
	/// </summary>
	/// <returns>Pointer to the topmost parent</returns>
	CWindow* GetParentWindow();

	/// <summary>
	/// Returns the absolute position of the control
	/// (x and y MUST be initialized with 0 before invoking this method)
	/// </summary>
	/// <param name="pX">ptr to final x position (must contain 0)</param>
	/// <param name="pY">ptr to final y position (must contain 0)</param>
	virtual void GetAbsolutePosition(int* pX, int* pY);

	/// <summary>
	/// This is called 'n' times per tick (for 'n' Events)
	/// 
	/// Should process the event & pass it to all its childs
	/// (The implementation of IControl::ProcesEvents is to call ProcessEvent on all children)
	/// </summary>
	/// <param name="pEvent">Event to be processed</param>
	virtual void ProcessEvent(CInputEvent* pEvent);

	/// <summary>
	/// This is called once per frame
	/// 
	/// Should draw the control
	/// (The implementation of IControl::Draw calls all children's Draw(...) function)
	/// </summary>
	/// <param name="pSurface">The surface to render on</param>
	virtual void Draw(ISurface* pSurface);

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
};

#endif // __ICONTROL_H__