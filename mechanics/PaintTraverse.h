#ifndef __PAINTTRAVERSE_H__
#define __PAINTTRAVERSE_H__

struct PaintTraverseParam
{
	unsigned int vguiPanel;
	bool forceRepaint;
	bool allowForce;
};

typedef void(__thiscall *PaintTraverse_t)(void*, unsigned int, bool, bool);
extern PaintTraverse_t g_pPaintTraverse;

void __fastcall hk_PaintTraverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);

#endif // __PAINTTRAVERSE_H__