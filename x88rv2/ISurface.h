#ifndef __ISURFACE_H__
#define __ISURFACE_H__

#ifdef CreateFont
#undef CreateFont
#endif

#ifdef PlaySound
#undef PlaySound
#endif

#include "CreateInterface.h"

class Color;
class ITexture;

class IImage;
class Image;
class Point;

// handles
typedef unsigned long HCursor;
typedef unsigned long HTexture;
typedef unsigned long HFont;


//SRC only defines

enum FontDrawType_t
{
	// Use the "additive" value from the scheme file
	FONT_DRAW_DEFAULT = 0,

	// Overrides
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,

	FONT_DRAW_TYPE_COUNT = 2,
};

//-----------------------------------------------------------------------------
// Purpose: Wraps contextless windows system functions
//-----------------------------------------------------------------------------
class ISurface// : public IAppSystem
{
public:
	void DrawSetColor(int a, int r, int g, int b);
	void DrawFilledRect(int x0, int y0, int x1, int y1);
	void DrawOutlinedRect(int x0, int y0, int x1, int y1);
	void DrawLine(int x0, int y0, int x1, int y1);
	void DrawSetTextColor(int r, int g, int b, int a);
	void DrawSetTextPos(int x, int y);
	void DrawPrintText(const wchar_t *text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT);
	bool SetFontGlyphSet(unsigned int font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0);
	unsigned int SCreateFont();
	void DrawSetTextFont(unsigned int font);
};

#endif // __ISURFACE_H__