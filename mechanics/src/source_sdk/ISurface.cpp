#include "ISurface.h"
#include "../utils/XorString.h"
#include "../core/ResourceManager.h"

#define DRAWSETCOLOR_ALT1_INDEX		14
#define DRAWSETCOLOR_ALT2_INDEX		15
#define DRAWFILLEDRECT_INDEX		16
#define DRAWOUTLINEDRECT_INDEX		18
#define DRAWLINE_INDEX				19
#define DRAWSETTEXTFONT_INDEX		23
#define DRAWSETTEXTCOLOR_ALT1_INDEX	24
#define DRAWSETTEXTCOLOR_ALT2_INDEX	25
#define DRAWSETTEXTPOS_INDEX		26
#define DRAWPRINTTEXT_INDEX			28
#define DRAWSETTEXTURERGBA_INDEX	37
#define DRAWSETTEXTURE_INDEX		38
#define DRAWGETTEXTURESIZE_INDEX	40
#define DRAWTEXTUREDRECT_INDEX		41
#define DELETETEXTUREBYID_INDEX		143
#define CREATENEWTEXTUREID_INDEX	43
#define SCREATEFONT_INDEX			71
#define SETFONTGLYPHSET_INDEX		72
#define GETTEXTSIZE_INDEX			79
#define DRAWOUTLINEDCIRCLE_INDEX	103
#define DRAWTEXTUREDPOLYLINE_INDEX	104
#define DRAWTEXTUREDPOLYGON_INDEX	106
#define DRAWTEXTUREDRECTEX_INDEX	148

#define SETCLIPRECT_INDEX			147
#define GETCLIPRECT_INDEX			146

#define UNLOCKCURSOR_INDEX			66
#define LOCKCURSOR_INDEX			67

void ISurface::SetClipRect(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall *SetClipRect_t)(void*, int, int, int, int);
	((SetClipRect_t)(*(void***)this)[SETCLIPRECT_INDEX])(this, x0, y0, x1, y1); // TODO: VTable Index untested
}

void ISurface::GetClipRect(int& x0, int& y0, int& x1, int& y1)
{
	typedef void(__thiscall *GetClipRect_t)(void*, int&, int&, int&, int&);
	((GetClipRect_t)(*(void***)this)[GETCLIPRECT_INDEX])(this, x0, y0, x1, y1); // TODO: VTable Index untested
}

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

void ISurface::DrawLine(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall *DrawLine_t)(void*, int, int, int, int);
	((DrawLine_t)(*(void***)this)[DRAWLINE_INDEX])(this, x0, y0, x1, y1);
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

void ISurface::DrawOutlinedCircle(int x, int y, int radius, int segments)
{
	typedef void(__thiscall* DrawOutlinedCircle_t)(void*, int, int, int, int);
	return ((DrawOutlinedCircle_t)(*(void***)this)[DRAWOUTLINEDCIRCLE_INDEX])(this, x, y, radius, segments);
}

void ISurface::DrawFilledCircle(int x, int y, int radius, int segments)
{
	// negative because of drawing direction (DrawTexturedPolygon kind of works that way)
	float fSegments = (float)-segments;
	float fAngle;
	
	Vertex_t* pVertices = new Vertex_t[segments];
	for (int i = 0; i < segments; i++)
	{
		fAngle = (i / fSegments) * (2 * PI_F);

		pVertices[i].m_Position.x = x + (radius * sinf(fAngle));
		pVertices[i].m_Position.y = y + (radius * cosf(fAngle));
	}
	
	this->DrawSetTexture(g_pResourceManager->GetWhiteTexture());
	this->DrawTexturedPolygon(segments, pVertices, false);

	delete[] pVertices;
}

int ISurface::CreateNewTextureID(bool procedural)
{
	typedef int(__thiscall *CreateNewTextureID_t)(void*, bool);
	return ((CreateNewTextureID_t)(*(void***)this)[CREATENEWTEXTUREID_INDEX])(this, procedural);
}

int ISurface::DrawGetTextureId(char const* filename)
{
	typedef int(__thiscall *DrawGetTextureId_t)(void*, char const*);
	return ((DrawGetTextureId_t)(*(void***)this)[34])(this, filename);
}

void ISurface::DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall)
{
	typedef void(__thiscall *DrawSetTextureRGBA_t)(void*, int, const unsigned char*, int, int);
	((DrawSetTextureRGBA_t)(*(void***)this)[DRAWSETTEXTURERGBA_INDEX])(this, id, rgba, wide, tall);
}

void ISurface::DrawSetTexture(int id)
{
	typedef void(__thiscall *DrawSetTexture_t)(void*, int);
	((DrawSetTexture_t)(*(void***)this)[DRAWSETTEXTURE_INDEX])(this, id);
}

void ISurface::DrawGetTextureSize(int id, int& wide, int& tall)
{
	typedef void(__thiscall *DrawGetTextureSize_t)(void*, int, int&, int&);
	((DrawGetTextureSize_t)(*(void***)this)[DRAWGETTEXTURESIZE_INDEX])(this, id, wide, tall);
}

void ISurface::DrawTexturedRect(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall *DrawTexturedRect_t)(void*, int, int, int, int);
	((DrawTexturedRect_t)(*(void***)this)[DRAWTEXTUREDRECT_INDEX])(this, x0, y0, x1, y1);
}

void ISurface::DrawTexturedRectEx(DrawTexturedRectParms_t* pDrawParms)
{
	typedef void(__thiscall *DrawTexturedRectEx_t)(void*, DrawTexturedRectParms_t*);
	((DrawTexturedRectEx_t)(*(void***)this)[DRAWTEXTUREDRECTEX_INDEX])(this, pDrawParms);
}

void ISurface::DrawTexturedPolyLine(const Vertex_t* pVertices, int n)
{
	typedef void(__thiscall *DrawTexturedPolyLine_t)(void*, const Vertex_t*, int);
	((DrawTexturedPolyLine_t)(*(void***)this)[DRAWTEXTUREDPOLYLINE_INDEX])(this, pVertices, n);
}

void ISurface::DrawTexturedPolygon(int n, Vertex_t* pVertices, bool bClipVertices)
{
	typedef void(__thiscall *DrawTexturedPolygon_t)(void*, int, Vertex_t*, bool);
	((DrawTexturedPolygon_t)(*(void***)this)[DRAWTEXTUREDPOLYGON_INDEX])(this, n, pVertices, bClipVertices);
}

bool ISurface::DeleteTextureByID(int id)
{
	typedef bool(__thiscall *DeleteTextureByID_t)(void*, int);
	return ((DeleteTextureByID_t)(*(void***)this)[DELETETEXTUREBYID_INDEX])(this, id);
}

unsigned int ISurface::SCreateFont()
{
	typedef unsigned int(__thiscall *CreateFont_t)(void*);
	return ((CreateFont_t)(*(void***)this)[SCREATEFONT_INDEX])(this);
}

bool ISurface::SetFontGlyphSet(unsigned int font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin, int nRangeMax)
{
	typedef bool(__thiscall *SetFontGlyphSet_t)(void*, unsigned int font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin, int nRangeMax);
	return ((SetFontGlyphSet_t)(*(void***)this)[SETFONTGLYPHSET_INDEX])(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
}

void ISurface::DrawSetTextFont(unsigned int font)
{
	typedef void(__thiscall *CreateFont_t)(void*, unsigned int);
	((CreateFont_t)(*(void***)this)[DRAWSETTEXTFONT_INDEX])(this, font);
}

void ISurface::DrawSetTextColor(Color color)
{
	typedef void(__thiscall *DrawSetTextColor_t)(void*, Color);
	((DrawSetTextColor_t)(*(void***)this)[DRAWSETTEXTCOLOR_ALT1_INDEX])(this, color);
}

void ISurface::DrawSetTextColor(int a, int r, int g, int b)
{
	typedef void(__thiscall *DrawSetTextColor_t)(void*, int, int, int, int);
	((DrawSetTextColor_t)(*(void***)this)[DRAWSETTEXTCOLOR_ALT2_INDEX])(this, r, g, b, a);
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


void ISurface::GetTextSize(HFont font, const wchar_t *text, int &wide, int &tall)
{
	typedef void(__thiscall *TextSize_t)(void*, HFont, const wchar_t*, int&, int&);
	((TextSize_t)(*(void***)this)[GETTEXTSIZE_INDEX])(this, font, text, wide, tall);
}

void ISurface::UnlockCursor()
{
	typedef void(__thiscall *UnlockCursor_t)(void*);
	((UnlockCursor_t)(*(void***)this)[UNLOCKCURSOR_INDEX])(this);
}

void ISurface::LockCursor()
{
	typedef void(__thiscall *LockCursor_t)(void*);
	((LockCursor_t)(*(void***)this)[LOCKCURSOR_INDEX])(this);
}