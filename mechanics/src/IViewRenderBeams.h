#ifndef __IVIEWRENDERBEAMS_H__
#define __IVIEWRENDERBEAMS_H__

#include "Vector.h"
#include "ClientEntity.h"


#define TE_EXPLFLAG_NONE				0x0	// all flags clear makes default Half-Life explosion
#define TE_EXPLFLAG_NOADDITIVE			0x1	// sprite will be drawn opaque (ensure that the sprite you send is a non-additive sprite)
#define TE_EXPLFLAG_NODLIGHTS			0x2	// do not render dynamic lights
#define TE_EXPLFLAG_NOSOUND				0x4	// do not play client explosion sound
#define TE_EXPLFLAG_NOPARTICLES			0x8	// do not draw particles
#define TE_EXPLFLAG_DRAWALPHA			0x10	// sprite will be drawn alpha
#define TE_EXPLFLAG_ROTATE				0x20	// rotate the sprite randomly
#define TE_EXPLFLAG_NOFIREBALL			0x40	// do not draw a fireball
#define TE_EXPLFLAG_NOFIREBALLSMOKE		0x80	// do not draw smoke with the fireball

#define	TE_BEAMPOINTS					0		// beam effect between two points
#define TE_SPRITE						1	// additive sprite, plays 1 cycle
#define TE_BEAMDISK						2	// disk that expands to max radius over lifetime
#define TE_BEAMCYLINDER					3		// cylinder that expands to max radius over lifetime
#define TE_BEAMFOLLOW					4		// create a line of decaying beam segments until entity stops moving
#define TE_BEAMRING						5		// connect a beam ring to two entities
#define TE_BEAMSPLINE					6		
#define TE_BEAMRINGPOINT				7
#define	TE_BEAMLASER					8		// Fades according to viewpoint
#define TE_BEAMTESLA					9

#define MAX_BEAM_ENTS					10
#define NOISE_DIVISIONS					128

class ITraceFilter;
typedef int pixelvis_handle_t;

struct BeamInfo_t
{
	int			m_nType;

	// Entities
	C_BaseEntity* m_pStartEnt;
	int			m_nStartAttachment;
	C_BaseEntity* m_pEndEnt;
	int			m_nEndAttachment;

	// Points
	Vector		m_vecStart;
	Vector		m_vecEnd;

	int			m_nModelIndex;
	const char	*m_pszModelName;

	int			m_nHaloIndex;
	const char	*m_pszHaloName;
	float		m_flHaloScale;

	float		m_flLife;
	float		m_flWidth;
	float		m_flEndWidth;
	float		m_flFadeLength;
	float		m_flAmplitude;

	float		m_flBrightness;
	float		m_flSpeed;

	int			m_nStartFrame;
	float		m_flFrameRate;

	float		m_flRed;
	float		m_flGreen;
	float		m_flBlue;

	bool		m_bRenderable;

	int			m_nSegments;

	int			m_nFlags;

	// Rings
	Vector		m_vecCenter;
	float		m_flStartRadius;
	float		m_flEndRadius;

	BeamInfo_t()
	{
		m_nType = TE_BEAMPOINTS;
		m_nSegments = -1;
		m_pszModelName = NULL;
		m_pszHaloName = NULL;
		m_nModelIndex = -1;
		m_nHaloIndex = -1;
		m_bRenderable = true;
		m_nFlags = 0;
	}
};

struct BeamTrail_t
{
	// NOTE:  Don't add user defined fields except after these four fields.
	BeamTrail_t*	next;
	float			die;
	Vector			org;
	Vector			vel;
};

// -------------------------------------------------------------------------------------------------- //
// CHandle.
// -------------------------------------------------------------------------------------------------- //
class CBaseHandle
{
	friend class CBaseEntityList;

public:

	CBaseHandle();
	CBaseHandle(const CBaseHandle &other);
	CBaseHandle(unsigned long value);
	CBaseHandle(int iEntry, int iSerialNumber);

	void Init(int iEntry, int iSerialNumber);
	void Term();

	// Even if this returns true, Get() still can return return a non-null value.
	// This just tells if the handle has been initted with any values.
	bool IsValid() const;

	int GetEntryIndex() const;
	int GetSerialNumber() const;

	int ToInt() const;
	bool operator !=(const CBaseHandle &other) const;
	bool operator ==(const CBaseHandle &other) const;
	bool operator ==(const IHandleEntity* pEnt) const;
	bool operator !=(const IHandleEntity* pEnt) const;
	bool operator <(const CBaseHandle &other) const;
	bool operator <(const IHandleEntity* pEnt) const;

	// Assign a value to the handle.
	const CBaseHandle& operator=(const IHandleEntity *pEntity);
	const CBaseHandle& Set(const IHandleEntity *pEntity);

	// Use this to dereference the handle.
	// Note: this is implemented in game code (ehandle.h)
	IHandleEntity* Get() const;


protected:
	// The low NUM_SERIAL_BITS hold the index. If this value is less than MAX_EDICTS, then the entity is networkable.
	// The high NUM_SERIAL_NUM_BITS bits are the serial number.
	unsigned long	m_Index;
};

template< class T >
class CHandle : public CBaseHandle
{
public:
	CHandle();
	CHandle(int iEntry, int iSerialNumber);
	CHandle(const CBaseHandle &handle);
	CHandle(T *pVal);

	// The index should have come from a call to ToInt(). If it hasn't, you're in trouble.
	static CHandle<T> FromIndex(int index);

	T*		Get() const;
	void	Set(const T* pVal);

	operator T*();
	operator T*() const;

	bool	operator !() const;
	bool	operator==(T *val) const;
	bool	operator!=(T *val) const;
	const CBaseHandle& operator=(const T *val);

	T*		operator->() const;
};

typedef CHandle<IClientEntity> EHANDLE; // The client's version of EHANDLE



										//-----------------------------------------------------------------------------
										// What kind of shadows to render?
										//-----------------------------------------------------------------------------
enum ShadowType_t
{
	SHADOWS_NONE = 0,
	SHADOWS_SIMPLE,
	SHADOWS_RENDER_TO_TEXTURE,
	SHADOWS_RENDER_TO_TEXTURE_DYNAMIC,	// the shadow is always changing state
	SHADOWS_RENDER_TO_DEPTH_TEXTURE,
};

typedef unsigned short ClientRenderHandle_t;
enum
{
	INVALID_CLIENT_RENDER_HANDLE = (ClientRenderHandle_t)0xffff,
};

//-----------------------------------------------------------------------------
// Handles to a client shadow
//-----------------------------------------------------------------------------
typedef unsigned short ClientShadowHandle_t;
enum
{
	CLIENTSHADOW_INVALID_HANDLE = (ClientShadowHandle_t)~0
};

class CDefaultClientRenderable : public IClientUnknown, public IClientRenderable
{
public:
	CDefaultClientRenderable()
	{
		m_hRenderHandle = INVALID_CLIENT_RENDER_HANDLE;
	}

	virtual const Vector &			GetRenderOrigin(void) = 0;
	virtual const QAngle &			GetRenderAngles(void) = 0;
	virtual const matrix3x4_t &		RenderableToWorldTransform() = 0;
	virtual bool					ShouldDraw(void) = 0;
	virtual bool					IsTransparent(void) = 0;
	virtual bool					IsTwoPass(void) { return false; }
	virtual void					OnThreadedDrawSetup() {}
	virtual bool					UsesPowerOfTwoFrameBufferTexture(void) { return false; }
	virtual bool					UsesFullFrameBufferTexture(void) { return false; }

	virtual ClientShadowHandle_t	GetShadowHandle() const
	{
		return CLIENTSHADOW_INVALID_HANDLE;
	}

	virtual ClientRenderHandle_t&	RenderHandle()
	{
		return m_hRenderHandle;
	}

	virtual int						GetBody() { return 0; }
	virtual int						GetSkin() { return 0; }
	virtual bool					UsesFlexDelayedWeights() { return false; }

	virtual const model_t*			GetModel() const { return NULL; }
	virtual int						DrawModel(int flags) { return 0; }
	virtual void					ComputeFxBlend() { return; }
	virtual int						GetFxBlend() { return 255; }
	virtual bool					LODTest() { return true; }
	virtual bool					SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) { return true; }
	virtual void					SetupWeights(const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights) {}
	virtual void					DoAnimationEvents(void) {}
	//virtual IPVSNotify*				GetPVSNotifyInterface() { return NULL; }
	virtual void					PlaceHolder();
	//virtual void					GetRenderBoundsWorldspace(Vector& absMins, Vector& absMaxs) { DefaultRenderBoundsWorldspace(this, absMins, absMaxs); }
	virtual void					PlaceHolder2();

	// Determine the color modulation amount
	virtual void	GetColorModulation(float* color)
	{
		color[0] = color[1] = color[2] = 1.0f;
	}

	// Should this object be able to have shadows cast onto it?
	virtual bool	ShouldReceiveProjectedTextures(int flags)
	{
		return false;
	}

	// These methods return true if we want a per-renderable shadow cast direction + distance
	virtual bool	GetShadowCastDistance(float *pDist, ShadowType_t shadowType) const { return false; }
	virtual bool	GetShadowCastDirection(Vector *pDirection, ShadowType_t shadowType) const { return false; }

	virtual void	GetShadowRenderBounds(Vector &mins, Vector &maxs, ShadowType_t shadowType)
	{
		GetRenderBounds(mins, maxs);
	}

	virtual bool IsShadowDirty() { return false; }
	virtual void MarkShadowDirty(bool bDirty) {}
	virtual IClientRenderable *GetShadowParent() { return NULL; }
	virtual IClientRenderable *FirstShadowChild() { return NULL; }
	virtual IClientRenderable *NextShadowPeer() { return NULL; }
	//virtual ShadowType_t ShadowCastType() { return SHADOWS_NONE; }
	virtual void PlaceHolder3();
	virtual void CreateModelInstance() {}
	virtual ModelInstanceHandle_t GetModelInstance() { return MODEL_INSTANCE_INVALID; }

	// Attachments
	virtual int LookupAttachment(const char *pAttachmentName) { return -1; }
	virtual	bool GetAttachment(int number, Vector &origin, QAngle &angles) { return false; }
	virtual bool GetAttachment(int number, matrix3x4_t &matrix) { return false; }

	// Rendering clip plane, should be 4 floats, return value of NULL indicates a disabled render clip plane
	virtual float *GetRenderClipPlane() { return NULL; }

	virtual void RecordToolMessage() {}
	virtual bool IgnoresZBuffer(void) const { return false; }

	// IClientUnknown implementation.
public:
	//virtual void SetRefEHandle(const CBaseHandle &handle) { Assert(false); }
	//virtual const CBaseHandle& GetRefEHandle() const { Assert(false); return *((CBaseHandle*)0); }
	virtual void PlaceHolder4();
	virtual void PlaceHolder5();

	virtual IClientUnknown*		GetIClientUnknown() { return this; }
	virtual ICollideable*		GetCollideable() { return 0; }
	virtual IClientRenderable*	GetClientRenderable() { return this; }
	virtual IClientNetworkable*	GetClientNetworkable() { return 0; }
	//virtual IClientEntity*		GetIClientEntity() { return 0; }
	virtual void PlaceHolder6();
	virtual C_BaseEntity*		GetBaseEntity() { return 0; }
	virtual IClientThinkable*	GetClientThinkable() { return 0; }


public:
	ClientRenderHandle_t m_hRenderHandle;
};

class Beam_t : public CDefaultClientRenderable
{
public:
	Beam_t();

	// Methods of IClientRenderable
	virtual const Vector&	GetRenderOrigin(void);
	virtual const QAngle&	GetRenderAngles(void);
	virtual const matrix3x4_t &RenderableToWorldTransform();
	virtual void			GetRenderBounds(Vector& mins, Vector& maxs);
	virtual bool			ShouldDraw(void);
	virtual bool			IsTransparent(void);
	virtual int				DrawModel(int flags);
	virtual void			ComputeFxBlend();
	virtual int				GetFxBlend();

	// Resets the beam state
	void			Reset();

	// Method to computing the bounding box
	void			ComputeBounds();

	// Bounding box...
	Vector			m_Mins;
	Vector			m_Maxs;
	pixelvis_handle_t *m_queryHandleHalo;
	float			m_haloProxySize;

	// Data is below..

	// Next beam in list
	Beam_t*			next;

	// Type of beam
	int				type;
	int				flags;

	// Control points for the beam
	int				numAttachments;
	Vector			attachment[MAX_BEAM_ENTS];
	Vector			delta;

	// 0 .. 1 over lifetime of beam
	float			t;
	float			freq;

	// Time when beam should die
	float			die;
	float			width;
	float			endWidth;
	float			fadeLength;
	float			amplitude;
	float			life;

	// Color
	float			r, g, b;
	float			brightness;

	// Speed
	float			speed;

	// Animation
	float			frameRate;
	float			frame;
	int				segments;

	// Attachment entities for the beam
	EHANDLE			entity[MAX_BEAM_ENTS];
	int				attachmentIndex[MAX_BEAM_ENTS];

	// Model info
	int				modelIndex;
	int				haloIndex;

	float			haloScale;
	int				frameCount;

	float			rgNoise[NOISE_DIVISIONS + 1];

	// Popcorn trail for beam follows to use
	BeamTrail_t*	trail;

	// for TE_BEAMRINGPOINT
	float			start_radius;
	float			end_radius;

	// for FBEAM_ONLYNOISEONCE
	bool			m_bCalculatedNoise;

	float			m_flHDRColorScale;

#ifdef PORTAL
	bool m_bDrawInMainRender;
	bool m_bDrawInPortalRender;
#endif //#ifdef PORTAL
};

class C_Beam;

class IViewRenderBeams
{
public:
	virtual void	InitBeams(void) = 0;
	virtual void	ShutdownBeams(void) = 0;
	virtual void	ClearBeams(void) = 0;

	// Updates the state of the temp ent beams
	virtual void	UpdateTempEntBeams() = 0;

	virtual void	DrawBeam(C_Beam* pbeam, ITraceFilter *pEntityBeamTraceFilter = NULL) = 0;
	virtual void	DrawBeam(Beam_t *pbeam) = 0;

	virtual void	KillDeadBeams(IClientEntity* pEnt) = 0;

	// New interfaces!
	virtual Beam_t	*CreateBeamEnts(BeamInfo_t &beamInfo) = 0;
	virtual Beam_t	*CreateBeamEntPoint(BeamInfo_t &beamInfo) = 0;
	virtual	Beam_t	*CreateBeamPoints(BeamInfo_t &beamInfo) = 0;
	virtual Beam_t	*CreateBeamRing(BeamInfo_t &beamInfo) = 0;
	virtual Beam_t	*CreateBeamRingPoint(BeamInfo_t &beamInfo) = 0;
	virtual Beam_t	*CreateBeamCirclePoints(BeamInfo_t &beamInfo) = 0;
	virtual Beam_t	*CreateBeamFollow(BeamInfo_t &beamInfo) = 0;

	virtual void	FreeBeam(Beam_t *pBeam) = 0;
	virtual void	UpdateBeamInfo(Beam_t *pBeam, BeamInfo_t &beamInfo) = 0;

	// These will go away!
	virtual void	CreateBeamEnts(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int type = -1) = 0;
	virtual void	CreateBeamEntPoint(int	nStartEntity, const Vector *pStart, int nEndEntity, const Vector* pEnd,
		int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual void	CreateBeamPoints(Vector& start, Vector& end, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual void	CreateBeamRing(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags = 0) = 0;
	virtual void	CreateBeamRingPoint(const Vector& center, float start_radius, float end_radius, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags = 0) = 0;
	virtual void	CreateBeamCirclePoints(int type, Vector& start, Vector& end,
		int modelIndex, int haloIndex, float haloScale, float life, float width,
		float m_nEndWidth, float m_nFadeLength, float amplitude, float brightness, float speed,
		int startFrame, float framerate, float r, float g, float b) = 0;
	virtual void	CreateBeamFollow(int startEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float r, float g, float b,
		float brightness) = 0;
};

extern IViewRenderBeams *beams;

#endif // __IVIEWRENDERBEAMS_H__