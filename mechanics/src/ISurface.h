#ifndef __ISURFACE_H__
#define __ISURFACE_H__

#ifdef CreateFont
#undef CreateFont
#endif

#ifdef PlaySound
#undef PlaySound
#endif

#include "CreateInterface.h"
#include "Vector.h"

class ITexture;

class IImage;
class Image;
class Point;

// handles
typedef unsigned long HCursor;
typedef unsigned long HTexture;
typedef unsigned long HFont;

struct Vertex_t
{
	Vertex_t() {}
	Vertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}

	Vector2D	m_Position;
	Vector2D	m_TexCoord;
};

enum FontDrawType_t
{
	// Use the "additive" value from the scheme file
	FONT_DRAW_DEFAULT = 0,

	// Overrides
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,

	FONT_DRAW_TYPE_COUNT = 2,
};

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
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

	void SetRed(unsigned char r) { _color[0] = r; }
	void SetGreen(unsigned char g) { _color[1] = g; }
	void SetBlue(unsigned char b) { _color[2] = b; }
	void SetAlpha(unsigned char a) { _color[3] = a; }

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
	void DrawLine(int x0, int y0, int x1, int y1);
	void DrawFilledRect(int x0, int y0, int x1, int y1);
	void DrawOutlinedRect(int x0, int y0, int x1, int y1);
	void DrawOutlinedCircle(int x, int y, int radius, int segments);

	int CreateNewTextureID(bool procedural = false);
	int DrawGetTextureId(char const *filename);
	void DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall);
	void DrawSetTexture(int id);
	void DrawTexturedRect(int x0, int y0, int x1, int y1);
	void DrawTexturedPolygon(int n, Vertex_t* pVertices, bool bClipVertices);
	bool DeleteTextureByID(int id);

	unsigned int SCreateFont();
	bool SetFontGlyphSet(unsigned int font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0);
	void DrawSetTextFont(unsigned int font);
	void DrawSetTextColor(Color color);
	void DrawSetTextColor(int a, int r, int g, int b);
	void DrawSetTextPos(int x, int y);
	void DrawPrintText(const wchar_t *text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT);
	void GetTextSize(HFont font, const wchar_t *text, int &wide, int &tall);
};

#endif // __ISURFACE_H__