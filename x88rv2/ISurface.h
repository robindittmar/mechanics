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

class Color
{
public:
	// constructors
	Color()
	{
		*((int *)this) = 0;
	}
	Color(int _r, int _g, int _b)
	{
		SetColor(255, _r, _g, _b);
	}
	Color(int _a, int _r, int _g, int _b)
	{
		SetColor(_a, _r, _g, _b);
	}

	// set the color
	// r - red component (0-255)
	// g - green component (0-255)
	// b - blue component (0-255)
	// a - alpha component, controls transparency (0 - transparent, 255 - opaque);
	void SetColor(int _a, int _r, int _g, int _b)
	{
		_color[0] = (unsigned char)_r;
		_color[1] = (unsigned char)_g;
		_color[2] = (unsigned char)_b;
		_color[3] = (unsigned char)_a;
	}

	void GetColor(int &_a, int &_r, int &_g, int &_b) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}

	void SetRawColor(int color32)
	{
		*((int *)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int *)this);
	}

	inline int r() const { return _color[0]; }
	inline int g() const { return _color[1]; }
	inline int b() const { return _color[2]; }
	inline int a() const { return _color[3]; }

	unsigned char &operator[](int index)
	{
		return _color[index];
	}

	const unsigned char &operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const Color &rhs) const
	{
		return (*((int *)this) == *((int *)&rhs));
	}

	bool operator != (const Color &rhs) const
	{
		return !(operator==(rhs));
	}

	Color &operator=(const Color &rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}

private:
	unsigned char _color[4];
};
//-----------------------------------------------------------------------------
// Purpose: Wraps contextless windows system functions
//-----------------------------------------------------------------------------
class ISurface// : public IAppSystem
{
public:
	void DrawSetColor(Color color);
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