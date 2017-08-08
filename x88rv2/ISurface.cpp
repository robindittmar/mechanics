#include "ISurface.h"

#define DRAWSETCOLOR_ALT1_INDEX		14
#define DRAWSETCOLOR_ALT2_INDEX		15
#define DRAWFILLEDRECT_INDEX		16
#define DRAWOUTLINEDRECT_INDEX		18
#define DRAWLINE_INDEX				19
#define DRAWSETTEXTFONT_INDEX		23
#define DRAWSETTEXTCOLOR_INDEX		25
#define DRAWSETTEXTPOS_INDEX		26
#define DRAWPRINTTEXT_INDEX			28
#define SCREATEFONT_INDEX			71
#define SETFONTGLYPHSET_INDEX		72
#define GETTEXTSIZE_INDEX			79

void ISurface::DrawSetColor(Color color)
{
	typedef void(__thiscall *DrawSetColor_t)(void*, Color);
	((DrawSetColor_t)(*(void***)this)[DRAWSETCOLOR_ALT1_INDEX])(this, color);
}

void ISurface::DrawSetColor(int a, int r, int g, int b)
{
	typedef void(__thiscall *DrawSetColor_t)(void*, int, int, int, int);
	((DrawSetColor_t)(*(void***)this)[DRAWSETCOLOR_ALT2_INDEX])(this, r, g, b, a);
}

void ISurface::DrawFilledRect(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall *DrawFilledRect_t)(void*, int, int, int, int);
	((DrawFilledRect_t)(*(void***)this)[DRAWFILLEDRECT_INDEX])(this, x0, y0, x1, y1);
}

void ISurface::DrawOutlinedRect(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall *DrawOutlinedRect_t)(void*, int, int, int, int);
	((DrawOutlinedRect_t)(*(void***)this)[DRAWOUTLINEDRECT_INDEX])(this, x0, y0, x1, y1);
}

void ISurface::DrawLine(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall *DrawLine_t)(void*, int, int, int, int);
	((DrawLine_t)(*(void***)this)[DRAWLINE_INDEX])(this, x0, y0, x1, y1);
}

void ISurface::DrawSetTextColor(int a, int r, int g, int b)
{
	typedef void(__thiscall *DrawSetTextColor_t)(void*, int, int, int, int);
	((DrawSetTextColor_t)(*(void***)this)[DRAWSETTEXTCOLOR_INDEX])(this, r, g, b, a);
}

void ISurface::DrawSetTextPos(int x, int y)
{
	typedef void(__thiscall *DrawSetTextColor_t)(void*, int, int);
	((DrawSetTextColor_t)(*(void***)this)[DRAWSETTEXTPOS_INDEX])(this, x, y);
}

void ISurface::DrawPrintText(const wchar_t *text, int textLen, FontDrawType_t drawType)
{
	typedef void(__thiscall *DrawPrintText_t)(void*, const wchar_t*, int, FontDrawType_t);
	((DrawPrintText_t)(*(void***)this)[DRAWPRINTTEXT_INDEX])(this, text, textLen, drawType);
}

bool ISurface::SetFontGlyphSet(unsigned int font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin, int nRangeMax)
{
	typedef bool(__thiscall *SetFontGlyphSet_t)(void*, unsigned int font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin, int nRangeMax);
	return ((SetFontGlyphSet_t)(*(void***)this)[SETFONTGLYPHSET_INDEX])(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
}

unsigned int ISurface::SCreateFont()
{
	typedef unsigned int(__thiscall *CreateFont_t)(void*);
	return ((CreateFont_t)(*(void***)this)[SCREATEFONT_INDEX])(this);
}

void ISurface::DrawSetTextFont(unsigned int font)
{
	typedef void(__thiscall *CreateFont_t)(void*, unsigned int);
	((CreateFont_t)(*(void***)this)[DRAWSETTEXTFONT_INDEX])(this, font);
}

void ISurface::GetTextSize(HFont font, const wchar_t *text, int &wide, int &tall)
{
	typedef void(__thiscall *TextSize_t)(void*, HFont, const wchar_t*, int&, int&);
	((TextSize_t)(*(void***)this)[GETTEXTSIZE_INDEX])(this, font, text, wide, tall);
}