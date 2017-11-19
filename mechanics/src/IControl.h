#ifndef __ICONTROL_H__
#define __ICONTROL_H__

// Std libs
#include <assert.h>
#include <vector>
#include <unordered_set>

// Resources
#include "ResourceManager.h"

// Event
#include "InputEvent.h"

// GUI Stuff (rendering etc)
#include "Gui.h"
#include "Tooltip.h"

#define TOOLTIP_DISTANCE_TO_MOUSE_X		15
#define TOOLTIP_DISTANCE_TO_MOUSE_Y		20

#define TOOLTIP_TIME_TO_DISPLAY			0.5f

const Color g_clrControlDisabled(255, 100, 100, 100);

class IControl;

struct ControlDependency_t
{
	IControl* control;
	void* param;
	bool reverse;
};

class IControl
{
public:
	IControl(int x = 0, int y = 0, int w = 0, int h = 0);
	~IControl();

	// Internal control events (may easily be overriden
	// will get called by IControl::ProcessEvent, when
	// appropiate)
	virtual void OnMouseMove(int mx, int my) {}
	virtual void OnMouseDown(int mx, int my) {}
	virtual void OnMouseUp(int mx, int my) {}
	virtual void OnClicked() {}

	/// <summary>
	/// Adds child to container,
	/// the container will take care of cleaning up the child controls
	/// </summary>
	virtual void AddChild(IControl* pControl);

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

	/// <summary>
	/// Sets the tooltip text (creates CTooltip instance & takes care of displaying)
	/// ==> Only invoke this method to set the text, everything else is done by IControl
	/// </summary>
	/// <param name="p">The text to display inside the Tooltip</param>
	void SetTooltipText(const char* p);

	/// <summary>
	/// Recursive function to retrieve the topmost IControl (which should be a CWindow)
	/// </summary>
	/// <returns>Pointer to the topmost parent</returns>
	IControl* GetParentWindow();

	/// <summary>
	/// Adds dependency of another control
	/// (For example: ChildCheckbox->AddDependency(ParentCheckbox) will result
	/// in ChildCheckbox being disabled, when ParentCheckbox is disabled aswell [and vice versa ofc])
	/// 
	/// The full dependency list is checked, using && on multiple controls,
	/// but || when having multiple conditions for a single control
	/// </summary>
	/// <param name="pDependency">Control to depend on</param>
	/// <param name="bReverse">Reverse the logic if this control will be enabled/disabled for this one dependency</param>
	void AddDependency(IControl* pDependency, void* pParam = nullptr, bool bReverse = false);

	/// <summary>
	/// Honestly you shouldn't invoke this, unless you explicitly want to change the Relationship between controls
	/// 
	/// this will be invoked by AddDependency, so I highly recommend you to check out AddDependency instead of this
	/// </summary>
	/// <param name="pDependentOne"></param>
	void AddDependentOne(IControl* pDependentOne);

	/// <summary>
	/// Evaluates if we want to be enabled or disabled (usually gets called from depenencies itself)
	/// </summary>
	void EvaluateDependencies();

	/// <summary>
	/// Will notify dependent ones about any changes so they'll evaluate if
	/// they need to be enabled or disabled
	/// </summary>
	void NotifyDependentOnes();

	/// <summary>
	/// Evaluates if we want our dependent ones to be enabled
	/// </summary>
	/// <returns>Wether or not the dependent ones should be enabled</returns>
	virtual bool ShouldDependentOnesBeEnabled(void* pParam) = 0;

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

	std::vector<ControlDependency_t> m_vDependencies;
	std::vector<IControl*> m_vDependentOnes;

	float m_fTimeLastMouseMovement;
	CTooltip* m_pTooltip;

	bool m_bHitcheckForMouseMove;
	bool m_bHitcheckForMouseUp;
	bool m_bCanHaveFocus;

	bool m_bMouseOver;
	bool m_bMouseDown;
};

#endif // __ICONTROL_H__