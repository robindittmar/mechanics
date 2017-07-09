#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <intrin.h>

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

class IClientNetworkable;
class IClientEntity;
class IMaterial;
class model_t;
class SurfInfo;
class PlayerInfo;
class client_textmessage_t;
class CSentence;
class CAudioSource;
class ISpatialQuery;
class IMaterialSystem;
class Frustum_t;
class AudioState_t;
class INetChannelInfo;
class matrix3x4_t;
class CPhysCollide;
class Color;
class IAchievementMgr;
class ISPSharedMemory;
class pfnDemoCustomDataCallback;
class CGamestatsData;
class CSteamAPIContext;
class KeyValues;
class InputContextHandle_t;

typedef float vec_t;
// 3D Vector
class Vector {
public:
	// Members
	vec_t x, y, z;

	// Construction/destruction:
	Vector(void) {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
	Vector(vec_t X, vec_t Y, vec_t Z);
	Vector(vec_t* clr);

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	vec_t* Base();
	vec_t const* Base() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);
	void Zero(); ///< zero out a vector

				 // equality
	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;

	// arithmetic operations
	Vector& operator+=(const Vector& v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Vector& operator-=(const Vector& v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	Vector& operator*=(float fl) {
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	Vector& operator*=(const Vector& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vector& operator/=(const Vector& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	// this ought to be an opcode.
	Vector&	operator+=(float fl) {
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	// this ought to be an opcode.
	Vector&	operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	Vector&	operator-=(float fl) {
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	// negate the vector components
	void	Negate();

	// Get the vector's magnitude.
	vec_t	Length() const;

	// Get the vector's magnitude squared.
	vec_t LengthSqr(void) const {
		return (x*x + y*y + z*z);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const {
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance);
	}

	vec_t	NormalizeInPlace();
	Vector	Normalized() const;
	bool	IsLengthGreaterThan(float val) const;
	bool	IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(Vector const &boxmin, Vector const &boxmax);

	// Get the distance from this vector to the other one.
	vec_t	DistTo(const Vector &vOther) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
	// may be able to tidy this up after switching to VC7
	vec_t DistToSqr(const Vector &vOther) const {
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void	MulAdd(const Vector& a, const Vector& b, float scalar);

	// Dot product.
	vec_t	Dot(const Vector& vOther) const;

	// assignment
	Vector& operator=(const Vector &vOther);

	// 2d
	vec_t	Length2D(void) const {
		return sqrt(x * x + y * y);
	}
	vec_t	Length2DSqr(void) const;

	/// get the component of this vector parallel to some other given vector
	Vector  ProjectOnto(const Vector& onto);

	// copy constructors
	//	Vector(const Vector &vOther);

	// arithmetic operations
	Vector	operator-(void) const;

	Vector	operator+(const Vector& v) const;
	Vector	operator-(const Vector& v) const;
	Vector	operator*(const Vector& v) const;
	Vector	operator/(const Vector& v) const;
	Vector	operator*(float fl) const;
	Vector	operator/(float fl) const;

	// Cross product between two vectors.
	Vector	Cross(const Vector &vOther) const;

	// Returns a vector with the min or max in X, Y, and Z.
	Vector	Min(const Vector &vOther) const;
	Vector	Max(const Vector &vOther) const;
};

class __declspec(align(16)) VectorAligned : public Vector {
public:
	inline VectorAligned(void) {};
	inline VectorAligned(vec_t X, vec_t Y, vec_t Z) {
		Init(X, Y, Z);
	}

public:
	explicit VectorAligned(const Vector &vOther) {
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector &vOther) {
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	VectorAligned& operator=(const VectorAligned &vOther) {
		_mm_store_ps(Base(), _mm_load_ps(vOther.Base()));
		return *this;
	}

	float w;
};

typedef Vector QAngle;
class VMatrix {
public:

	VMatrix();
	VMatrix(
		vec_t m00, vec_t m01, vec_t m02, vec_t m03,
		vec_t m10, vec_t m11, vec_t m12, vec_t m13,
		vec_t m20, vec_t m21, vec_t m22, vec_t m23,
		vec_t m30, vec_t m31, vec_t m32, vec_t m33
	);

	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	VMatrix(const Vector& forward, const Vector& left, const Vector& up);

	// Construct from a 3x4 matrix
	VMatrix(const matrix3x4_t& matrix3x4);

	// Set the values in the matrix.
	void		Init(
		vec_t m00, vec_t m01, vec_t m02, vec_t m03,
		vec_t m10, vec_t m11, vec_t m12, vec_t m13,
		vec_t m20, vec_t m21, vec_t m22, vec_t m23,
		vec_t m30, vec_t m31, vec_t m32, vec_t m33
	);


	// Initialize from a 3x4
	void		Init(const matrix3x4_t& matrix3x4);

	// array access
	inline float* operator[](int i) {
		return m[i];
	}

	inline const float* operator[](int i) const {
		return m[i];
	}

	// Get a pointer to m[0][0]
	inline float *Base() {
		return &m[0][0];
	}

	inline const float *Base() const {
		return &m[0][0];
	}

	void		SetLeft(const Vector &vLeft);
	void		SetUp(const Vector &vUp);
	void		SetForward(const Vector &vForward);

	void		GetBasisVectors(Vector &vForward, Vector &vLeft, Vector &vUp) const;
	void		SetBasisVectors(const Vector &vForward, const Vector &vLeft, const Vector &vUp);

	// Get/set the translation.
	Vector &	GetTranslation(Vector &vTrans) const;
	void		SetTranslation(const Vector &vTrans);

	void		PreTranslate(const Vector &vTrans);
	void		PostTranslate(const Vector &vTrans);

	matrix3x4_t& As3x4();
	const matrix3x4_t& As3x4() const;
	void		CopyFrom3x4(const matrix3x4_t &m3x4);
	void		Set3x4(matrix3x4_t& matrix3x4) const;

	bool		operator==(const VMatrix& src) const;
	bool		operator!=(const VMatrix& src) const { return !(*this == src); }

	// Access the basis vectors.
	Vector		GetLeft() const;
	Vector		GetUp() const;
	Vector		GetForward() const;
	Vector		GetTranslation() const;


	// Matrix->vector operations.
public:
	// Multiply by a 3D vector (same as operator*).
	void		V3Mul(const Vector &vIn, Vector &vOut) const;

	// Multiply by a 4D vector.
	//void		V4Mul( const Vector4D &vIn, Vector4D &vOut ) const;

	// Applies the rotation (ignores translation in the matrix). (This just calls VMul3x3).
	Vector		ApplyRotation(const Vector &vVec) const;

	// Multiply by a vector (divides by w, assumes input w is 1).
	Vector		operator*(const Vector &vVec) const;

	// Multiply by the upper 3x3 part of the matrix (ie: only apply rotation).
	Vector		VMul3x3(const Vector &vVec) const;

	// Apply the inverse (transposed) rotation (only works on pure rotation matrix)
	Vector		VMul3x3Transpose(const Vector &vVec) const;

	// Multiply by the upper 3 rows.
	Vector		VMul4x3(const Vector &vVec) const;

	// Apply the inverse (transposed) transformation (only works on pure rotation/translation)
	Vector		VMul4x3Transpose(const Vector &vVec) const;


	// Matrix->plane operations.
	//public:
	// Transform the plane. The matrix can only contain translation and rotation.
	//void		TransformPlane( const VPlane &inPlane, VPlane &outPlane ) const;

	// Just calls TransformPlane and returns the result.
	//VPlane		operator*(const VPlane &thePlane) const;

	// Matrix->matrix operations.
public:

	VMatrix&	operator=(const VMatrix &mOther);

	// Multiply two matrices (out = this * vm).
	void		MatrixMul(const VMatrix &vm, VMatrix &out) const;

	// Add two matrices.
	const VMatrix& operator+=(const VMatrix &other);

	// Just calls MatrixMul and returns the result.	
	VMatrix		operator*(const VMatrix &mOther) const;

	// Add/Subtract two matrices.
	VMatrix		operator+(const VMatrix &other) const;
	VMatrix		operator-(const VMatrix &other) const;

	// Negation.
	VMatrix		operator-() const;

	// Return inverse matrix. Be careful because the results are undefined 
	// if the matrix doesn't have an inverse (ie: InverseGeneral returns false).
	VMatrix		operator~() const;

	// Matrix operations.
public:
	// Set to identity.
	void		Identity();

	bool		IsIdentity() const;

	// Setup a matrix for origin and angles.
	void		SetupMatrixOrgAngles(const Vector &origin, const QAngle &vAngles);

	// General inverse. This may fail so check the return!
	bool		InverseGeneral(VMatrix &vInverse) const;

	// Does a fast inverse, assuming the matrix only contains translation and rotation.
	void		InverseTR(VMatrix &mRet) const;

	// Usually used for debug checks. Returns true if the upper 3x3 contains
	// unit vectors and they are all orthogonal.
	bool		IsRotationMatrix() const;

	// This calls the other InverseTR and returns the result.
	VMatrix		InverseTR() const;

	// Get the scale of the matrix's basis vectors.
	Vector		GetScale() const;

	// (Fast) multiply by a scaling matrix setup from vScale.
	VMatrix		Scale(const Vector &vScale);

	// Normalize the basis vectors.
	VMatrix		NormalizeBasisVectors() const;

	// Transpose.
	VMatrix		Transpose() const;

	// Transpose upper-left 3x3.
	VMatrix		Transpose3x3() const;

public:
	// The matrix.
	vec_t		m[4][4];
};

enum class SendPropType {
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
	DPT_String,
	DPT_Array,     // An array of the base types (can't be of datatables).
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes
};

class RecvTable;

class RecvProp {
public:
	char *m_pVarName;
	SendPropType m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	const void *m_pExtraData;
	RecvProp *m_pArrayProp;
	void *m_ArrayLengthProxy;
	void *m_ProxyFn;
	void *m_DataTableProxyFn;
	RecvTable *m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char *m_pParentArrayPropName;
};

class RecvTable {
public:
	RecvProp *m_pProps;
	int m_nProps;
	void *m_pDecoder;
	char *m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

class ClientClass {
public:
	void *m_pCreateFn;
	void *m_pCreateEventFn;
	char *m_pNetworkName;
	RecvTable *m_pRecvTable;
	ClientClass *m_pNext;
	int m_ClassID;
};

class IBaseClientDLL {
public:
	// Connect appsystem components, get global interfaces, don't run any other init code
	virtual int              Connect(CreateInterfaceFn appSystemFactory, void *pGlobals) = 0;
	virtual int              Disconnect(void) = 0;
	virtual int              Init(CreateInterfaceFn appSystemFactory, void *pGlobals) = 0;
	virtual void             PostInit() = 0;
	virtual void             Shutdown(void) = 0;
	virtual void             LevelInitPreEntity(char const* pMapName) = 0;
	virtual void             LevelInitPostEntity() = 0;
	virtual void             LevelShutdown(void) = 0;
	virtual ClientClass*     GetAllClasses(void) = 0;
};

class IClientEntityList {
public:
	virtual IClientNetworkable*   GetClientNetworkable(int entnum) = 0;
	virtual void*                 vtablepad0x1(void) = 0;
	virtual void*                 vtablepad0x2(void) = 0;
	virtual IClientEntity*        GetClientEntity(int entNum) = 0;
	virtual IClientEntity*        GetClientEntityFromHandle(void* hEnt) = 0;
	virtual int                   NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int                   GetHighestEntityIndex(void) = 0;
	virtual void                  SetMaxEntities(int maxEnts) = 0;
	virtual int                   GetMaxEntities() = 0;
};

class IVEngineClient {
public:
	virtual int                   GetIntersectingSurfaces(const model_t *model, const Vector &vCenter, const float radius, const bool bOnlyVisibleSurfaces, SurfInfo *pInfos, const int nMaxInfos) = 0;
	virtual Vector                GetLightForPoint(const Vector &pos, bool bClamp) = 0;
	virtual IMaterial*            TraceLineMaterialAndLighting(const Vector &start, const Vector &end, Vector &diffuseLightColor, Vector& baseColor) = 0;
	virtual const char*           ParseFile(const char *data, char *token, int maxlen) = 0;
	virtual bool                  CopyFile(const char *source, const char *destination) = 0;
	virtual void                  GetScreenSize(int& width, int& height) = 0;
	virtual void                  ServerCmd(const char *szCmdString, bool bReliable = true) = 0;
	virtual void                  ClientCmd(const char *szCmdString) = 0;
	virtual bool                  GetPlayerInfo(int ent_num, PlayerInfo *pinfo) = 0;
	virtual int                   GetPlayerForUserID(int userID) = 0;
	virtual client_textmessage_t* TextMessageGet(const char *pName) = 0; // 10
	virtual bool                  Con_IsVisible(void) = 0;
	virtual int                   GetLocalPlayer(void) = 0;
	virtual const model_t*        LoadModel(const char *pName, bool bProp = false) = 0;
	virtual float                 GetLastTimeStamp(void) = 0;
	virtual CSentence*            GetSentence(CAudioSource *pAudioSource) = 0; // 15
	virtual float                 GetSentenceLength(CAudioSource *pAudioSource) = 0;
	virtual bool                  IsStreaming(CAudioSource *pAudioSource) const = 0;
	virtual void                  GetViewAngles(QAngle& va) = 0;
	virtual void                  SetViewAngles(QAngle& va) = 0;
	virtual int                   GetMaxClients(void) = 0; // 20
	virtual   const char*         Key_LookupBinding(const char *pBinding) = 0;
	virtual const char*           Key_BindingForKey(int &code) = 0;
	virtual void                  Key_SetBinding(int, char const*) = 0;
	virtual void                  StartKeyTrapMode(void) = 0;
	virtual bool                  CheckDoneKeyTrapping(int &code) = 0;
	virtual bool                  IsInGame(void) = 0;
	virtual bool                  IsConnected(void) = 0;
	virtual bool                  IsDrawingLoadingImage(void) = 0;
	virtual void                  HideLoadingPlaque(void) = 0;
	virtual void                  Con_NPrintf(int pos, const char *fmt, ...) = 0; // 30
	virtual void                  Con_NXPrintf(const struct con_nprint_s *info, const char *fmt, ...) = 0;
	virtual int                   IsBoxVisible(const Vector& mins, const Vector& maxs) = 0;
	virtual int                   IsBoxInViewCluster(const Vector& mins, const Vector& maxs) = 0;
	virtual bool                  CullBox(const Vector& mins, const Vector& maxs) = 0;
	virtual void                  Sound_ExtraUpdate(void) = 0;
	virtual const char*           GetGameDirectory(void) = 0;
	virtual const VMatrix&        WorldToScreenMatrix() = 0;
	virtual const VMatrix&        WorldToViewMatrix() = 0;
	virtual int                   GameLumpVersion(int lumpId) const = 0;
	virtual int                   GameLumpSize(int lumpId) const = 0; // 40
	virtual bool                  LoadGameLump(int lumpId, void* pBuffer, int size) = 0;
	virtual int                   LevelLeafCount() const = 0;
	virtual ISpatialQuery*        GetBSPTreeQuery() = 0;
	virtual void                  LinearToGamma(float* linear, float* gamma) = 0;
	virtual float                 LightStyleValue(int style) = 0; // 45
	virtual void                  ComputeDynamicLighting(const Vector& pt, const Vector* pNormal, Vector& color) = 0;
	virtual void                  GetAmbientLightColor(Vector& color) = 0;
	virtual int                   GetDXSupportLevel() = 0;
	virtual bool                  SupportsHDR() = 0;
	virtual void                  Mat_Stub(IMaterialSystem *pMatSys) = 0; // 50
	virtual void                  GetChapterName(char *pchBuff, int iMaxLength) = 0;
	virtual char const*           GetLevelName(void) = 0;
	virtual char const*           GetLevelNameShort(void) = 0;
	virtual char const*           GetMapGroupName(void) = 0;
	virtual struct IVoiceTweak_s* GetVoiceTweakAPI(void) = 0;
	virtual void                  SetVoiceCasterID(unsigned int someint) = 0; // 56
	virtual void                  EngineStats_BeginFrame(void) = 0;
	virtual void                  EngineStats_EndFrame(void) = 0;
	virtual void                  FireEvents() = 0;
	virtual int                   GetLeavesArea(unsigned short *pLeaves, int nLeaves) = 0;
	virtual bool                  DoesBoxTouchAreaFrustum(const Vector &mins, const Vector &maxs, int iArea) = 0; // 60
	virtual int                   GetFrustumList(Frustum_t **pList, int listMax) = 0;
	virtual bool                  ShouldUseAreaFrustum(int i) = 0;
	virtual void                  SetAudioState(const AudioState_t& state) = 0;
	virtual int                   SentenceGroupPick(int groupIndex, char *name, int nameBufLen) = 0;
	virtual int                   SentenceGroupPickSequential(int groupIndex, char *name, int nameBufLen, int sentenceIndex, int reset) = 0;
	virtual int                   SentenceIndexFromName(const char *pSentenceName) = 0;
	virtual const char*           SentenceNameFromIndex(int sentenceIndex) = 0;
	virtual int                   SentenceGroupIndexFromName(const char *pGroupName) = 0;
	virtual const char*           SentenceGroupNameFromIndex(int groupIndex) = 0;
	virtual float                 SentenceLength(int sentenceIndex) = 0;
	virtual void                  ComputeLighting(const Vector& pt, const Vector* pNormal, bool bClamp, Vector& color, Vector *pBoxColors = NULL) = 0;
	virtual void                  ActivateOccluder(int nOccluderIndex, bool bActive) = 0;
	virtual bool                  IsOccluded(const Vector &vecAbsMins, const Vector &vecAbsMaxs) = 0; // 74
	virtual int                   GetOcclusionViewId(void) = 0;
	virtual void*                 SaveAllocMemory(size_t num, size_t size) = 0;
	virtual void                  SaveFreeMemory(void *pSaveMem) = 0;
	virtual INetChannelInfo*      GetNetChannelInfo(void) = 0;
	virtual void                  DebugDrawPhysCollide(const CPhysCollide *pCollide, IMaterial *pMaterial, const matrix3x4_t& transform, const Color &color) = 0; //79
	virtual void                  CheckPoint(const char *pName) = 0; // 80
	virtual void                  DrawPortals() = 0;
	virtual bool                  IsPlayingDemo(void) = 0;
	virtual bool                  IsRecordingDemo(void) = 0;
	virtual bool                  IsPlayingTimeDemo(void) = 0;
	virtual int                   GetDemoRecordingTick(void) = 0;
	virtual int                   GetDemoPlaybackTick(void) = 0;
	virtual int                   GetDemoPlaybackStartTick(void) = 0;
	virtual float                 GetDemoPlaybackTimeScale(void) = 0;
	virtual int                   GetDemoPlaybackTotalTicks(void) = 0;
	virtual bool                  IsPaused(void) = 0; // 90
	virtual float                 GetTimescale(void) const = 0;
	virtual bool                  IsTakingScreenshot(void) = 0;
	virtual bool                  IsHLTV(void) = 0;
	virtual bool                  IsLevelMainMenuBackground(void) = 0;
	virtual void                  GetMainMenuBackgroundName(char *dest, int destlen) = 0;
	virtual void                  SetOcclusionParameters(const int /*OcclusionParams_t*/ &params) = 0; // 96
	virtual void                  GetUILanguage(char *dest, int destlen) = 0;
	virtual int                   IsSkyboxVisibleFromPoint(const Vector &vecPoint) = 0;
	virtual const char*           GetMapEntitiesString() = 0;
	virtual bool                  IsInEditMode(void) = 0; // 100
	virtual float                 GetScreenAspectRatio(int viewportWidth, int viewportHeight) = 0;
	virtual bool                  REMOVED_SteamRefreshLogin(const char *password, bool isSecure) = 0; // 100
	virtual bool                  REMOVED_SteamProcessCall(bool & finished) = 0;
	virtual unsigned int          GetEngineBuildNumber() = 0; // engines build
	virtual const char *          GetProductVersionString() = 0; // mods version number (steam.inf)
	virtual void                  GrabPreColorCorrectedFrame(int x, int y, int width, int height) = 0;
	virtual bool                  IsHammerRunning() const = 0;
	virtual void                  ExecuteClientCmd(const char *szCmdString) = 0; //108
	virtual bool                  MapHasHDRLighting(void) = 0;
	virtual bool                  MapHasLightMapAlphaData(void) = 0;
	virtual int                   GetAppID() = 0;
	virtual Vector                GetLightForPointFast(const Vector &pos, bool bClamp) = 0;
	virtual void                  ClientCmd_Unrestricted(char  const*, int, bool);
	virtual void                  ClientCmd_Unrestricted(const char *szCmdString) = 0; // 114
	virtual void                  SetRestrictServerCommands(bool bRestrict) = 0;
	virtual void                  SetRestrictClientCommands(bool bRestrict) = 0;
	virtual void                  SetOverlayBindProxy(int iOverlayID, void *pBindProxy) = 0;
	virtual bool                  CopyFrameBufferToMaterial(const char *pMaterialName) = 0;
	virtual void                  ReadConfiguration(const int iController, const bool readDefault) = 0;
	virtual void                  SetAchievementMgr(IAchievementMgr *pAchievementMgr) = 0;
	virtual IAchievementMgr*      GetAchievementMgr() = 0;
	virtual bool                  MapLoadFailed(void) = 0;
	virtual void                  SetMapLoadFailed(bool bState) = 0;
	virtual bool                  IsLowViolence() = 0;
	virtual const char*           GetMostRecentSaveGame(void) = 0;
	virtual void                  SetMostRecentSaveGame(const char *lpszFilename) = 0;
	virtual void                  StartXboxExitingProcess() = 0;
	virtual bool                  IsSaveInProgress() = 0;
	virtual bool                  IsAutoSaveDangerousInProgress(void) = 0;
	virtual unsigned int          OnStorageDeviceAttached(int iController) = 0;
	virtual void                  OnStorageDeviceDetached(int iController) = 0;
	virtual char* const           GetSaveDirName(void) = 0;
	virtual void                  WriteScreenshot(const char *pFilename) = 0;
	virtual void                  ResetDemoInterpolation(void) = 0;
	virtual int                   GetActiveSplitScreenPlayerSlot() = 0;
	virtual int                   SetActiveSplitScreenPlayerSlot(int slot) = 0;
	virtual bool                  SetLocalPlayerIsResolvable(char const *pchContext, int nLine, bool bResolvable) = 0;
	virtual bool                  IsLocalPlayerResolvable() = 0;
	virtual int                   GetSplitScreenPlayer(int nSlot) = 0;
	virtual bool                  IsSplitScreenActive() = 0;
	virtual bool                  IsValidSplitScreenSlot(int nSlot) = 0;
	virtual int                   FirstValidSplitScreenSlot() = 0; // -1 == invalid
	virtual int                   NextValidSplitScreenSlot(int nPreviousSlot) = 0; // -1 == invalid
	virtual ISPSharedMemory*      GetSinglePlayerSharedMemorySpace(const char *szName, int ent_num = (1 << 11)) = 0;
	virtual void                  ComputeLightingCube(const Vector& pt, bool bClamp, Vector *pBoxColors) = 0;
	virtual void                  RegisterDemoCustomDataCallback(const char* szCallbackSaveID, pfnDemoCustomDataCallback pCallback) = 0;
	virtual void                  RecordDemoCustomData(pfnDemoCustomDataCallback pCallback, const void *pData, size_t iDataLength) = 0;
	virtual void                  SetPitchScale(float flPitchScale) = 0;
	virtual float                 GetPitchScale(void) = 0;
	virtual bool                  LoadFilmmaker() = 0;
	virtual void                  UnloadFilmmaker() = 0;
	virtual void                  SetLeafFlag(int nLeafIndex, int nFlagBits) = 0;
	virtual void                  RecalculateBSPLeafFlags(void) = 0;
	virtual bool                  DSPGetCurrentDASRoomNew(void) = 0;
	virtual bool                  DSPGetCurrentDASRoomChanged(void) = 0;
	virtual bool                  DSPGetCurrentDASRoomSkyAbove(void) = 0;
	virtual float                 DSPGetCurrentDASRoomSkyPercent(void) = 0;
	virtual void                  SetMixGroupOfCurrentMixer(const char *szgroupname, const char *szparam, float val, int setMixerType) = 0;
	virtual int                   GetMixLayerIndex(const char *szmixlayername) = 0;
	virtual void                  SetMixLayerLevel(int index, float level) = 0;
	virtual int                   GetMixGroupIndex(char  const* groupname) = 0;
	virtual void                  SetMixLayerTriggerFactor(int i1, int i2, float fl) = 0;
	virtual void                  SetMixLayerTriggerFactor(char  const* char1, char  const* char2, float fl) = 0;
	virtual bool                  IsCreatingReslist() = 0;
	virtual bool                  IsCreatingXboxReslist() = 0;
	virtual void                  SetTimescale(float flTimescale) = 0;
	virtual void                  SetGamestatsData(CGamestatsData *pGamestatsData) = 0;
	virtual CGamestatsData*       GetGamestatsData() = 0;
	virtual void                  GetMouseDelta(int &dx, int &dy, bool b) = 0; // unknown
	virtual   const char*         Key_LookupBindingEx(const char *pBinding, int iUserId = -1, int iStartCount = 0, int iAllowJoystick = -1) = 0;
	virtual int                   Key_CodeForBinding(char  const*, int, int, int) = 0;
	virtual void                  UpdateDAndELights(void) = 0;
	virtual int                   GetBugSubmissionCount() const = 0;
	virtual void                  ClearBugSubmissionCount() = 0;
	virtual bool                  DoesLevelContainWater() const = 0;
	virtual float                 GetServerSimulationFrameTime() const = 0;
	virtual void                  SolidMoved(class IClientEntity *pSolidEnt, class ICollideable *pSolidCollide, const Vector* pPrevAbsOrigin, bool accurateBboxTriggerChecks) = 0;
	virtual void                  TriggerMoved(class IClientEntity *pTriggerEnt, bool accurateBboxTriggerChecks) = 0;
	virtual void                  ComputeLeavesConnected(const Vector &vecOrigin, int nCount, const int *pLeafIndices, bool *pIsConnected) = 0;
	virtual bool                  IsInCommentaryMode(void) = 0;
	virtual void                  SetBlurFade(float amount) = 0;
	virtual bool                  IsTransitioningToLoad() = 0;
	virtual void                  SearchPathsChangedAfterInstall() = 0;
	virtual void                  ConfigureSystemLevel(int nCPULevel, int nGPULevel) = 0;
	virtual void                  SetConnectionPassword(char const *pchCurrentPW) = 0;
	virtual CSteamAPIContext*     GetSteamAPIContext() = 0;
	virtual void                  SubmitStatRecord(char const *szMapName, unsigned int uiBlobVersion, unsigned int uiBlobSize, const void *pvBlob) = 0;
	virtual void                  ServerCmdKeyValues(KeyValues *pKeyValues) = 0; // 203
	virtual void                  SpherePaintSurface(const model_t* model, const Vector& location, unsigned char chr, float fl1, float fl2) = 0;
	virtual bool                  HasPaintmap(void) = 0;
	virtual void                  EnablePaintmapRender() = 0;
	//virtual void                TracePaintSurface( const model_t *model, const Vector& position, float radius, CUtlVector<Color>& surfColors ) = 0;
	virtual void                  SphereTracePaintSurface(const model_t* model, const Vector& position, const Vector &vec2, float radius, /*CUtlVector<unsigned char, CUtlMemory<unsigned char, int>>*/ int& utilVecShit) = 0;
	virtual void                  RemoveAllPaint() = 0;
	virtual void                  PaintAllSurfaces(unsigned char uchr) = 0;
	virtual void                  RemovePaint(const model_t* model) = 0;
	virtual bool                  IsActiveApp() = 0;
	virtual bool                  IsClientLocalToActiveServer() = 0;
	virtual void                  TickProgressBar() = 0;
	virtual InputContextHandle_t  GetInputContext(int /*EngineInputContextId_t*/ id) = 0;
	virtual void                  GetStartupImage(char* filename, int size) = 0;
	virtual bool                  IsUsingLocalNetworkBackdoor(void) = 0;
	virtual void                  SaveGame(const char*, bool, char*, int, char*, int) = 0;
	virtual void                  GetGenericMemoryStats(/* GenericMemoryStat_t */ void **) = 0;
	virtual bool                  GameHasShutdownAndFlushedMemory(void) = 0;
	virtual int                   GetLastAcknowledgedCommand(void) = 0;
	virtual void                  FinishContainerWrites(int i) = 0;
	virtual void                  FinishAsyncSave(void) = 0;
	virtual int                   GetServerTick(void) = 0;
	virtual const char*           GetModDirectory(void) = 0;
	virtual bool                  AudioLanguageChanged(void) = 0;
	virtual bool                  IsAutoSaveInProgress(void) = 0;
	virtual void                  StartLoadingScreenForCommand(const char* command) = 0;
	virtual void                  StartLoadingScreenForKeyValues(KeyValues* values) = 0;
	virtual void                  SOSSetOpvarFloat(const char*, float) = 0;
	virtual void                  SOSGetOpvarFloat(const char*, float &) = 0;
	virtual bool                  IsSubscribedMap(const char*, bool) = 0;
	virtual bool                  IsFeaturedMap(const char*, bool) = 0;
	virtual void                  GetDemoPlaybackParameters(void) = 0;
	virtual int                   GetClientVersion(void) = 0;
	virtual bool                  IsDemoSkipping(void) = 0;
	virtual void                  SetDemoImportantEventData(const KeyValues* values) = 0;
	virtual void                  ClearEvents(void) = 0;
	virtual int                   GetSafeZoneXMin(void) = 0;
	virtual bool                  IsVoiceRecording(void) = 0;
	virtual void                  ForceVoiceRecordOn(void) = 0;
	virtual bool                  IsReplay(void) = 0;
};

struct SpatializationInfo_t;
class ICollideable;
class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class C_BaseEntity;
class IClientThinkable;
class IClientAlphaProperty;

//class IClientUnknown;
class ClientClass;
class bf_read;

class IClientUnknown;
class CClientThinkHandlePtr;
typedef CClientThinkHandlePtr* ClientThinkHandle_t;

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

class IClientUnknown;
//struct model_t;

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