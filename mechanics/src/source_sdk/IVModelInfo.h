//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#ifndef __IVMODELINFO_H__
#define __IVMODELINFO_H__

#include <cstdint>
#include <cstring>
#include "Vector.h"

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class IMaterial;
class KeyValues;
struct vcollide_t;
struct model_t;
class CGameTrace;
struct cplane_t;
typedef CGameTrace trace_t;
struct virtualmodel_t;
typedef unsigned char byte;
struct virtualterrainparams_t;
class CPhysCollide;
typedef unsigned short MDLHandle_t;
class CUtlBuffer;
class IClientRenderable;


// Copy pasta test
// intersection boxes
struct mstudiobbox_t
{
	int32_t	m_iBone;
	int32_t	m_iGroup;
	Vector	m_vecBBMin;
	Vector	m_vecBBMax;
	int32_t	m_iHitboxNameIndex;
	int32_t	m_iPad01[3];
	float	m_flRadius;
	int32_t	m_iPad02[4];

	const char* pszHitboxName(int index) // 0-whatever (usually 20)
	{
		if (m_iHitboxNameIndex == 0)
			return "";

		char* name = ((char*)this) + m_iHitboxNameIndex + 1; // full name
		for (int i = 0; i < index + 1; i++) { // +1 so that if you pass 0 on the first iteration, it doesn't skip it.
			name = (name + (strlen(name) + 1));
		}
		return name;
	}

	mstudiobbox_t() {}

private:
	// No copy constructors allowed
	mstudiobbox_t(const mstudiobbox_t& vOther);
};

struct mstudiohitboxset_t
{
	int     sznameindex;

	inline const char* pszName(void)
	{
		return ((char*)this) + sznameindex;
	}

	int numhitboxes;
	int hitboxindex;

	inline mstudiobbox_t* pHitbox(int i) const
	{
		return (mstudiobbox_t*)(((byte*)this) + hitboxindex) + i;
	}
};

struct mstudiobone_t
{
	int sznameindex;

	inline char *const pszName(void) const
	{
		return ((char *) this) + sznameindex;
	}

	int parent;        // parent bone
	int bonecontroller[6];    // bone controller index, -1 == none

							  // default values
	Vector pos;
	Quaternion quat;
	RadianEuler rot;
	// compression scale
	Vector posscale;
	Vector rotscale;

	matrix3x4_t poseToBone;
	Quaternion qAlignment;
	int flags;
	int proctype;
	int procindex;        // procedural rule
	mutable int physicsbone;    // index into physically simulated bone

	inline void *pProcedure() const
	{
		if (procindex == 0)
			return NULL;
		else
			return (void *)(((unsigned char *) this) + procindex);
	};

	int surfacepropidx;    // index into string tablefor property name

	inline char *const pszSurfaceProp(void) const
	{
		return ((char *) this) + surfacepropidx;
	}

	inline int GetSurfaceProp(void) const
	{
		return surfacepropLookup;
	}

	int contents;        // See BSPFlags.h for the contents flags
	int surfacepropLookup;    // this index must be cached by the loader, not saved in the file
	int unused[7];        // remove as appropriate
};


// sequence descriptions
struct mstudioseqdesc_t
{
	int	szlabelindex;
	inline char * const pszLabel(void) const { return ((char *)this) + szlabelindex; }

	int	szactivitynameindex;
	inline char * const pszActivityName(void) const { return ((char *)this) + szactivitynameindex; }

	int	flags;		// looping/non-looping flags

	int	activity;	// initialized at loadtime to game DLL values
	int	actweight;

	int	numevents;
	int	eventindex;
	inline void *pEvent(int i) const { return (((byte *)this) + eventindex) + i; };

	Vector	bbmin;		// per sequence bounding box
	Vector	bbmax;

	//-------------------------------------------------------------------------
	// Purpose: returns a model animation from the sequence group size and
	//          blend index
	// Note: this replaces GetAnimValue() that was previously in bone_setup
	// Note: this also acts as a SetAnimValue() as it returns a reference to
	//       the anim value in question
	//-------------------------------------------------------------------------
	inline unsigned short& pAnimValue(int nIndex0, int nIndex1) const
	{
		// Clamp indexes
		if (nIndex0 >= groupsize[0])
			nIndex0 = groupsize[0] - 1;

		if (nIndex1 >= groupsize[1])
			nIndex1 = groupsize[1] - 1;

		return *pBlend(nIndex1 * groupsize[0] + nIndex0);
	}

	int	numblends;

	int blendindex;
	inline unsigned short *pBlend(int i) const { return (unsigned short *)(((byte *)this) + blendindex) + i; };

	int seqgroup; // sequence group for demand loading

	int	groupsize[2];
	int	paramindex[2];	// X, Y, Z, XR, YR, ZR
	float	paramstart[2];	// local (0..1) starting value
	float	paramend[2];	// local (0..1) ending value
	int	paramparent;

	float	fadeintime;	// ideal cross fate in time (0.2 default)
	float	fadeouttime;	// ideal cross fade out time (0.2 default)

	int	entrynode;	// transition node at entry
	int	exitnode;	// transition node at exit
	int	nodeflags;	// transition rules

	float	entryphase;	// used to match entry gait
	float	exitphase;	// used to match exit gait

	float	lastframe;	// frame that should generation EndOfSequence

	int	nextseq;	// auto advancing sequences
	int	pose;		// index of delta animation between end and nextseq

	int	numikrules;

	int	numautolayers;
	int	autolayerindex;
	inline void *pAutolayer(int i) const { return (((byte *)this) + autolayerindex) + i; };

	int	weightlistindex;
	float	*pBoneweight(int i) const { return ((float *)(((byte *)this) + weightlistindex) + i); };
	float	weight(int i) const { return *(pBoneweight(i)); };

	int	posekeyindex;
	float				*pPoseKey(int iParam, int iAnim) const { return (float *)(((byte *)this) + posekeyindex) + iParam * groupsize[0] + iAnim; }
	float				poseKey(int iParam, int iAnim) const { return *(pPoseKey(iParam, iAnim)); }

	int	numiklocks;
	int	iklockindex;
	inline void *pIKLock(int i) const { return (((byte *)this) + iklockindex) + i; };

	// Key values
	int	keyvalueindex;
	int	keyvaluesize;
	inline const char * KeyValueText(void) const { return keyvaluesize != 0 ? ((char *)this) + keyvalueindex : NULL; }

	int	unused[3];		// remove/add as appropriate
};


struct studiohdr_t
{
	int	id;
	int	version;

	long checksum;	// this has to be the same in the phy and vtx files to load!

	char name[64];
	int	length;

	Vector	eyeposition;	// ideal eye position

	Vector	illumposition;	// illumination center

	Vector	hull_min;	// ideal movement hull size
	Vector	hull_max;

	Vector	view_bbmin;	// clipping bounding box
	Vector	view_bbmax;

	int	flags;

	int	numbones;	// bones
	int	boneindex;
	inline mstudiobone_t *pBone(int i) const { return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };

	int	numbonecontrollers;	// bone controllers
	int	bonecontrollerindex;
	inline void *pBonecontroller(int i) const { return (((byte *)this) + bonecontrollerindex) + i; };

	int	numhitboxsets;
	int	hitboxsetindex;

	// Look up hitbox set by index
	mstudiohitboxset_t  *pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t *)(((byte *)this) + hitboxsetindex) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t *pHitbox(int i, int set) const
	{
		mstudiohitboxset_t *s = pHitboxSet(set);

		if (!s)
			return NULL;

		return s->pHitbox(i);
	};

	// Calls through to set to get hitbox count for set
	inline int  iHitboxCount(int set) const
	{
		mstudiohitboxset_t const *s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};

	int	numanim;	// animations/poses
	int	animdescindex;	// animation descriptions
	inline void *pAnimdesc(int i) const { return (((byte *)this) + animdescindex) + i; };

	int 	numanimgroup;
	int 	animgroupindex;
	inline  void *pAnimGroup(int i) const { return (((byte *)this) + animgroupindex) + i; };

	int 	numbonedesc;
	int 	bonedescindex;
	inline  void *pBoneDesc(int i) const { return (((byte *)this) + bonedescindex) + i; };

	int	numseq;		// sequences
	int	seqindex;
	inline mstudioseqdesc_t *pSeqdesc(int i) const { if (i < 0 || i >= numseq) i = 0; return (mstudioseqdesc_t *)(((byte *)this) + seqindex) + i; };
	int	sequencesindexed;	// initialization flag - have the sequences been indexed?

	int	numseqgroups;		// demand loaded sequences
	int	seqgroupindex;
	inline  void *pSeqgroup(int i) const { return (((byte *)this) + seqgroupindex) + i; };

	int	numtextures;		// raw textures
	int	textureindex;
	inline void *pTexture(int i) const { return (((byte *)this) + textureindex) + i; };

	int	numcdtextures;		// raw textures search paths
	int	cdtextureindex;
	inline char			*pCdtexture(int i) const { return (((char *)this) + *((int *)(((byte *)this) + cdtextureindex) + i)); };

	int	numskinref;		// replaceable textures tables
	int	numskinfamilies;
	int	skinindex;
	inline short		*pSkinref(int i) const { return (short *)(((byte *)this) + skinindex) + i; };

	int	numbodyparts;
	int	bodypartindex;
	inline void	*pBodypart(int i) const { return (((byte *)this) + bodypartindex) + i; };

	int	numattachments;		// queryable attachable points
	int	attachmentindex;
	inline void	*pAttachment(int i) const { return (((byte *)this) + attachmentindex) + i; };

	int	numtransitions;		// animation node to animation node transition graph
	int	transitionindex;
	inline byte	*pTransition(int i) const { return (byte *)(((byte *)this) + transitionindex) + i; };

	int	numflexdesc;
	int	flexdescindex;
	inline void *pFlexdesc(int i) const { return (((byte *)this) + flexdescindex) + i; };

	int	numflexcontrollers;
	int	flexcontrollerindex;
	inline void *pFlexcontroller(int i) const { return (((byte *)this) + flexcontrollerindex) + i; };

	int	numflexrules;
	int	flexruleindex;
	inline void *pFlexRule(int i) const { return (((byte *)this) + flexruleindex) + i; };

	int	numikchains;
	int	ikchainindex;
	inline void *pIKChain(int i) const { return (((byte *)this) + ikchainindex) + i; };

	int	nummouths;
	int	mouthindex;
	inline void *pMouth(int i) const { return (((byte *)this) + mouthindex) + i; };

	int	numposeparameters;
	int	poseparamindex;
	inline void *pPoseParameter(int i) const { return (((byte *)this) + poseparamindex) + i; };

	int	surfacepropindex;
	inline char * const pszSurfaceProp(void) const { return ((char *)this) + surfacepropindex; }

	// Key values
	int	keyvalueindex;
	int	keyvaluesize;
	inline const char * KeyValueText(void) const { return keyvaluesize != 0 ? ((char *)this) + keyvalueindex : NULL; }

	int	numikautoplaylocks;
	int	ikautoplaylockindex;
	inline void *pIKAutoplayLock(int i) const { return (((byte *)this) + ikautoplaylockindex) + i; };

	float mass;		// The collision model mass that jay wanted
	int	contents;
	int	unused[9];	// remove as appropriate
};

// WORKING ONE !!!
//struct studiohdr_t
//{
//	int id;
//	int version;
//	int checksum;        // this has to be the same in the phy and vtx files to load!
//	char name[64];
//	int length;
//
//	Vector eyeposition;    // ideal eye position
//	Vector illumposition;    // illumination center
//	Vector hull_min;        // ideal movement hull size
//	Vector hull_max;
//	Vector view_bbmin;        // clipping bounding box
//	Vector view_bbmax;
//
//	int flags;
//	int numbones;            // bones
//	int boneindex;
//	inline mstudiobone_t *pBone(int i) const
//	{
//		return (mstudiobone_t *)(((unsigned char *) this) + boneindex) + i;
//	};
//
//	int numbonecontrollers;        // bone controllers
//	int bonecontrollerindex;
//	int numhitboxsets;
//	int hitboxsetindex;
//
//	mstudiohitboxset_t* pHitboxSet(int i) const
//	{
//		return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
//	}
//
//	inline mstudiobbox_t* pHitbox(int i, int set) const
//	{
//		const mstudiohitboxset_t* s = pHitboxSet(set);
//
//		if (!s)
//			return 0;
//
//		return s->pHitbox(i);
//	}
//
//	inline int iHitboxCount(int set) const
//	{
//		const mstudiohitboxset_t* s = pHitboxSet(set);
//
//		if (!s)
//			return 0;
//
//		return s->numhitboxes;
//	}
//};
// Copy pasta test


//-----------------------------------------------------------------------------
// Indicates the type of translucency of an unmodulated renderable
//-----------------------------------------------------------------------------
enum RenderableTranslucencyType_t
{
	RENDERABLE_IS_OPAQUE = 0,
	RENDERABLE_IS_TRANSLUCENT,
	RENDERABLE_IS_TWO_PASS,	// has both translucent and opaque sub-partsa
};


//-----------------------------------------------------------------------------
// Model info interface
//-----------------------------------------------------------------------------

class IVModelInfo
{
public:
	virtual							~IVModelInfo(void) { }

	virtual const model_t			*GetModel(int modelindex) const = 0;
	// Returns index of model by name
	virtual int						GetModelIndex(const char *name) const = 0;

	// Returns name of model
	virtual const char				*GetModelName(const model_t *model) const = 0;
	virtual void					UNUSED() {};
	virtual vcollide_t				*GetVCollide(const model_t *model) const = 0;
	virtual vcollide_t				*GetVCollide(int modelindex) const = 0;
	virtual void					GetModelBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual	void					GetModelRenderBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual int						GetModelFrameCount(const model_t *model) const = 0;
	virtual int						GetModelType(const model_t *model) const = 0;
	virtual void					*GetModelExtraData(const model_t *model) = 0;
	virtual bool					ModelHasMaterialProxy(const model_t *model) const = 0;
	virtual bool					IsTranslucent(model_t const* model) const = 0;
	virtual bool					IsTranslucentTwoPass(const model_t *model) const = 0;
	virtual void					Unused0() {};
	virtual RenderableTranslucencyType_t ComputeTranslucencyType(const model_t *model, int nSkin, int nBody) = 0;
	virtual int						GetModelMaterialCount(const model_t* model) const = 0;
	virtual void					GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterial) = 0;
	virtual bool					IsModelVertexLit(const model_t *model) const = 0;
	virtual const char				*GetModelKeyValueText(const model_t *model) = 0;
	virtual bool					GetModelKeyValue(const model_t *model, CUtlBuffer &buf) = 0; // supports keyvalue blocks in submodels
	virtual float					GetModelRadius(const model_t *model) = 0;

	virtual const studiohdr_t		*FindModel(const studiohdr_t *pStudioHdr, void **cache, const char *modelname) const = 0;
	virtual const studiohdr_t		*FindModel(void *cache) const = 0;
	virtual	virtualmodel_t			*GetVirtualModel(const studiohdr_t *pStudioHdr) const = 0;
	virtual byte					*GetAnimBlock(const studiohdr_t *pStudioHdr, int iBlock) const = 0;

	// Available on client only!!!
	virtual void					GetModelMaterialColorAndLighting(const model_t *model, Vector const& origin,
		QAngle const& angles, trace_t* pTrace,
		Vector& lighting, Vector& matColor) = 0;
	virtual void					GetIlluminationPoint(const model_t *model, IClientRenderable *pRenderable, Vector const& origin,
		QAngle const& angles, Vector* pLightingCenter) = 0;

	virtual int						GetModelContents(int modelIndex) const = 0;
	virtual void					UNUSED2() {};
	virtual void					PADDING_001() = 0;
	virtual studiohdr_t				*GetStudiomodel(const model_t *mod) = 0;
	virtual int						GetModelSpriteWidth(const model_t *model) const = 0;
	virtual int						GetModelSpriteHeight(const model_t *model) const = 0;

	// Sets/gets a map-specified fade range (client only)
	virtual void					SetLevelScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual void					GetLevelScreenFadeRange(float *pMinArea, float *pMaxArea) const = 0;

	// Sets/gets a map-specified per-view fade range (client only)
	virtual void					SetViewScreenFadeRange(float flMinSize, float flMaxSize) = 0;

	// Computes fade alpha based on distance fade + screen fade (client only)
	virtual unsigned char			ComputeLevelScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual unsigned char			ComputeViewScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;

	// both client and server
	virtual int						GetAutoplayList(const studiohdr_t *pStudioHdr, unsigned short **pAutoplayList) const = 0;

	// Gets a virtual terrain collision model (creates if necessary)
	// NOTE: This may return NULL if the terrain model cannot be virtualized
	virtual CPhysCollide			*GetCollideForVirtualTerrain(int index) = 0;

	virtual bool					IsUsingFBTexture(const model_t *model, int nSkin, int nBody, void /*IClientRenderable*/ *pClientRenderable) const = 0;

	virtual const model_t			*FindOrLoadModel(const char *name) const = 0;

	virtual MDLHandle_t				GetCacheHandle(const model_t *model) const = 0;

	// Returns planes of non-nodraw brush model surfaces
	virtual int						GetBrushModelPlaneCount(const model_t *model) const = 0;
	virtual void					GetBrushModelPlane(const model_t *model, int nIndex, cplane_t &plane, Vector *pOrigin) const = 0;
	virtual int						GetSurfacepropsForVirtualTerrain(int index) = 0;
	virtual bool					UsesEnvCubemap(const model_t *model) const = 0;
	virtual bool	UsesStaticLighting(const model_t *model) const = 0;
};


class IVModelInfoClient : public IVModelInfo
{
public:
};


struct virtualterrainparams_t
{
	// UNDONE: Add grouping here, specified in BSP file? (test grouping to see if this is necessary)
	int index;
};

#endif // __IVMODELINFO_H__