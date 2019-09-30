#include "IVEngineClient.h"

INetChannel* IVEngineClient::GetNetChannel()
{
    typedef INetChannel* (__thiscall* INetChannel_t)(void*);
    return ((INetChannel_t)(*(void***)this)[78])(this);
}