/// 
/// Source Engine declarations
///


#ifndef __CLIENTENTITY_H__
#define __CLIENTENTITY_H__

struct SpatializationInfo_t;
class ICollideable;
class C_BaseEntity;
class IClientAlphaProperty;

class bf_read;
class ClientClass;

class CClientThinkHandlePtr;
typedef CClientThinkHandlePtr* ClientThinkHandle_t;

class IClientUnknown;

class IClientThinkable {
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual ClientThinkHandle_t	GetThinkHandle() = 0;
	virtual void				SetThinkHandle(ClientThinkHandle_t hThink) = 0;
	virtual void				Release() = 0;
};

class IClientNetworkable {
public:
	virtual IClientUnknown*  GetIClientUnknown() = 0;
	virtual void             Release() = 0;
	virtual ClientClass*     GetClientClass() = 0;
	virtual void             NotifyShouldTransmit(int state) = 0;
	virtual void             OnPreDataChanged(int updateType) = 0;
	virtual void             OnDataChanged(int updateType) = 0;
	virtual void             PreDataUpdate(int updateType) = 0;
	virtual void             PostDataUpdate(int updateType) = 0;
	virtual void             __unkn(void) = 0;
	virtual bool             IsDormant(void) = 0;
	virtual int              EntIndex(void) const = 0;
	virtual void             ReceiveMessage(int classID, bf_read& msg) = 0;
	virtual void*            GetDataTableBasePtr() = 0;
	virtual void             SetDestroyedOnRecreateEntities(void) = 0;
};

typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;
typedef unsigned char uint8;

class Vector;
typedef Vector QAngle;
class model_t;
class matrix3x4_t;

class IClientRenderable {
public:
	virtual IClientUnknown*            GetIClientUnknown() = 0;
	virtual Vector const&              GetRenderOrigin(void) = 0;
	virtual QAngle const&              GetRenderAngles(void) = 0;
	virtual bool                       ShouldDraw(void) = 0;
	virtual int                        GetRenderFlags(void) = 0; // ERENDERFLAGS_xxx
	virtual void                       Unused(void) const {}
	virtual ClientShadowHandle_t       GetShadowHandle() const = 0;
	virtual ClientRenderHandle_t&      RenderHandle() = 0;
	virtual const model_t*             GetModel() const = 0;
	virtual int                        DrawModel(int flags, const int /*RenderableInstance_t*/ &instance) = 0;
	virtual int                        GetBody() = 0;
	virtual void                       GetColorModulation(float* color) = 0;
	virtual bool                       LODTest() = 0;
	virtual bool                       SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
	virtual void                       SetupWeights(const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights) = 0;
	virtual void                       DoAnimationEvents(void) = 0;
	virtual void* /*IPVSNotify*/       GetPVSNotifyInterface() = 0;
	virtual void                       GetRenderBounds(Vector& mins, Vector& maxs) = 0;
	virtual void                       GetRenderBoundsWorldspace(Vector& mins, Vector& maxs) = 0;
	virtual void                       GetShadowRenderBounds(Vector &mins, Vector &maxs, int /*ShadowType_t*/ shadowType) = 0;
	virtual bool                       ShouldReceiveProjectedTextures(int flags) = 0;
	virtual bool                       GetShadowCastDistance(float *pDist, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool                       GetShadowCastDirection(Vector *pDirection, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool                       IsShadowDirty() = 0;
	virtual void                       MarkShadowDirty(bool bDirty) = 0;
	virtual IClientRenderable*         GetShadowParent() = 0;
	virtual IClientRenderable*         FirstShadowChild() = 0;
	virtual IClientRenderable*         NextShadowPeer() = 0;
	virtual int /*ShadowType_t*/       ShadowCastType() = 0;
	virtual void                       CreateModelInstance() = 0;
	virtual ModelInstanceHandle_t      GetModelInstance() = 0;
	virtual const matrix3x4_t&         RenderableToWorldTransform() = 0;
	virtual int                        LookupAttachment(const char *pAttachmentName) = 0;
	virtual   bool                     GetAttachment(int number, Vector &origin, QAngle &angles) = 0;
	virtual bool                       GetAttachment(int number, matrix3x4_t &matrix) = 0;
	virtual float*                     GetRenderClipPlane(void) = 0;
	virtual int                        GetSkin() = 0;
	virtual void                       OnThreadedDrawSetup() = 0;
	virtual bool                       UsesFlexDelayedWeights() = 0;
	virtual void                       RecordToolMessage() = 0;
	virtual bool                       ShouldDrawForSplitScreenUser(int nSlot) = 0;
	virtual uint8                      OverrideAlphaModulation(uint8 nAlpha) = 0;
	virtual uint8                      OverrideShadowAlphaModulation(uint8 nAlpha) = 0;
};
class CBaseHandle;

class IHandleEntity {
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle(const CBaseHandle &handle) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};


class IClientUnknown : public IHandleEntity {
public:
	virtual ICollideable*              GetCollideable() = 0;
	virtual IClientNetworkable*        GetClientNetworkable() = 0;
	virtual IClientRenderable*         GetClientRenderable() = 0;
	virtual IClientEntity*             GetIClientEntity() = 0;
	virtual C_BaseEntity*              GetBaseEntity() = 0;
	virtual IClientThinkable*          GetClientThinkable() = 0;
	//virtual IClientModelRenderable*  GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty*      GetClientAlphaProperty() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual void             Release(void) = 0;
	virtual const Vector     GetAbsOrigin(void) const = 0;
	virtual const QAngle     GetAbsAngles(void) const = 0;
	virtual void*            GetMouth(void) = 0;
	virtual bool             GetSoundSpatialization(SpatializationInfo_t info) = 0;
	virtual bool             IsBlurred(void) = 0;
};

#endif // __CLIENTENTITY_H__