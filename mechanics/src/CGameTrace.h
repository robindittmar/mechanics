#ifndef __CGAMETRACE_H__
#define __CGAMETRACE_H__

#include "Vector.h"

#define DISPSURF_FLAG_SURFACE		(1<<0)
#define DISPSURF_FLAG_WALKABLE		(1<<1)
#define DISPSURF_FLAG_BUILDABLE		(1<<2)
#define DISPSURF_FLAG_SURFPROP1		(1<<3)
#define DISPSURF_FLAG_SURFPROP2		(1<<4)

class IClientEntity;

struct cplane_t
{
	Vector			normal;
	float			dist;
	unsigned char	type;
	unsigned char	signbits;
	unsigned char	pad[2];
};


struct csurface_t
{
	const char*		name;
	short			surfaceProps;
	unsigned short	flags;         // BUGBUG: These are declared per surface, not per material, but this database is per-material now
};

class CBaseTrace
{
public:

	//// Displacement flags tests.
	//bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	//bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	//bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	//bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	//bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

	// these members are aligned!!
	Vector			startpos;				// start position
	Vector			endpos;					// final position
	cplane_t		plane;					// surface normal at impact

	float			fraction;				// time completed, 1.0 = didn't hit anything

	int				contents;				// contents on other side of surface hit
	unsigned short	dispFlags;				// displacement flags for marking surfaces with data

	bool			allsolid;				// if true, plane is not valid
	bool			startsolid;				// if true, the initial point was in a solid area

	CBaseTrace() {}

private:
	// No copy constructors allowed
	CBaseTrace(const CBaseTrace& vOther);
};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int GetEntityIndex() const;
	bool DidHit() const;
	bool IsVisible() const;
	bool IsEntityVisible(IClientEntity* pTarget = NULL) const;
	bool DidHitEntity(IClientEntity* pTarget) const;

public:

	float               fractionleftsolid;  // time we left a solid, only valid if we started in solid
	csurface_t          surface;            // surface hit (impact surface)
	int                 hitgroup;           // 0 == generic, non-zero is specific body part
	short               physicsbone;        // physics bone hit by trace in studio
	unsigned short      worldSurfaceIndex;  // Index of the msurface2_t, if applicable
	IClientEntity*      hit_entity;
	int                 hitbox;                       // box hit by trace in studio
	char shit[0x24];

	CGameTrace() {}

private:
	// No copy constructors allowed
	CGameTrace(const CGameTrace& other) :
		fractionleftsolid(other.fractionleftsolid),
		surface(other.surface),
		hitgroup(other.hitgroup),
		physicsbone(other.physicsbone),
		worldSurfaceIndex(other.worldSurfaceIndex),
		hit_entity(other.hit_entity),
		hitbox(other.hitbox)
	{
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
	}

	/*CGameTrace& CGameTrace::operator=(const CGameTrace& other)
	{
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
		fractionleftsolid = other.fractionleftsolid;
		surface = other.surface;
		hitgroup = other.hitgroup;
		physicsbone = other.physicsbone;
		worldSurfaceIndex = other.worldSurfaceIndex;
		hit_entity = other.hit_entity;
		hitbox = other.hitbox;
		return *this;
	}*/
};

typedef CGameTrace trace_t;

inline bool CGameTrace::DidHit() const
{
	return fraction < 1 || allsolid || startsolid;
}

inline bool CGameTrace::IsVisible() const
{
	return (fraction > 0.97f);
}

inline bool CGameTrace::IsEntityVisible(IClientEntity* pTarget) const
{
	return ((fraction > 0.97f) || (hit_entity == pTarget));
}

inline bool CGameTrace::DidHitEntity(IClientEntity* pTarget) const
{
	return (hit_entity == pTarget);
}

#endif // __CGAMETRACE_H__