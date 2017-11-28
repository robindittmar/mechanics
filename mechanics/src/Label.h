#ifndef __LABEL_H__
#define __LABEL_H__

// Std lib
#include <cstring>

// Custom
#include "Gui.h"
#include "IControl.h"

#define LABEL_ORIENTATION_LEFT		0
#define LABEL_ORIENTATION_CENTER	1
#define LABEL_ORIENTATION_RIGHT		2

class CLabel : public IControl
{
public:
	CLabel(
		int x = 0,
		int y = 0,
		int w = 0,
		int h = 0,
		const char* pText = nullptr,
		int font = RM_FONT_NORMAL,
		int orientation = LABEL_ORIENTATION_LEFT,
		Color color = Color(255, 255, 255, 255)
	);
	virtual ~CLabel();

	virtual void Draw(ISurface* pSurface);

	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;

	void GetTextSize(ISurface* pSurface, int& width, int& height);

	void SetTextColor(Color c) { m_cTextColor = c; }
	Color GetTextColor() { return m_cTextColor; }

	void SetOrientation(int iOrientation) { m_iOrientation = iOrientation; }
	int GetOrientation() { return m_iOrientation; }

	int GetContentTextLength() { return m_iContentTextLen; }

	void SetContentText(const char* pText);
	const char* GetContentText() { return (const char*)m_pContentText; }

	void SetContentTextW(const wchar_t* pTextW);
	const wchar_t* GetContentTextW() { return (const wchar_t*)m_pContentTextW; }
private:
	void FreeMemory();

	unsigned int m_iFont;

	Color m_cTextColor;
	int m_iOrientation;

	int m_iContentTextLen;
	char* m_pContentText;
	wchar_t* m_pContentTextW;
};

#endif // __LABEL_H__