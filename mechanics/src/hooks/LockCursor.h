#ifndef __LOCKCURSOR_H__
#define __LOCKCURSOR_H__

typedef void(__thiscall* LockCursor_t)(void*);
extern LockCursor_t g_pLockCursor;

void __fastcall hk_LockCursor(void* ecx, void* edx);

#endif // __LOCKCURSOR_H__