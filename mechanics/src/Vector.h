/// 
/// Source Engine declarations
///
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cmath>
#include <intrin.h>
#include "MathDefs.h"

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
	Vector(vec_t X, vec_t Y, vec_t Z) {
		this->x = X;
		this->y = Y;
		this->z = Z;
	}
	Vector(vec_t* clr) {
		this->x = clr[0];
		this->y = clr[1];
		this->z = clr[2];
	}

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f) {
		this->x = ix;
		this->x = iy;
		this->z = iz;
	}
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	vec_t operator[](int i) const {
		return ((float*)this)[i];
	}
	vec_t& operator[](int i) {
		return ((float*)this)[i];
	}

	// Base address...
	vec_t* Base();
	vec_t const* Base() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);
	void Zero(); ///< zero out a vector

	// equality
	bool operator==(const Vector& v) const {
		return (
			this->x == v.x &&
			this->y == v.y &&
			this->z == v.z
		);
	}
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
	vec_t	Length() const {
		return sqrtf(LengthSqr());
	}

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

	vec_t	NormalizeInPlace() { // TODO: C+p from aimtux
		Vector& v = *this;

		float iradius = 1.f / (this->Length() + 1.192092896e-07F); //FLT_EPSILON

		v.x *= iradius;
		v.y *= iradius;
		v.z *= iradius;

		return iradius;
	}
	Vector	Normalized() const;
	bool	IsLengthGreaterThan(float val) const;
	bool	IsLengthLessThan(float val) const;

	Vector Direction()
	{
		return Vector(cos(y * PI_F / 180.0f) * cos(x * PI_F / 180.0f), sin(y * PI_F / 180.0f) * cos(x * PI_F / 180.0f), sin(-x * PI_F / 180.0f));
	}

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
	vec_t	Dot(const Vector& vOther) const {
		return (this->x * vOther.x + this->y * vOther.y + this->z * vOther.z);
	}

	// assignment
	Vector& operator=(const Vector &vOther) {
		this->x = vOther.x;
		this->y = vOther.y;
		this->z = vOther.z;
		return *this;
	}

	// 2d
	vec_t	Length2D(void) const {
		return sqrt(Length2DSqr());
	}
	vec_t	Length2DSqr(void) const {
		return x * x + y * y;
	}

	/// get the component of this vector parallel to some other given vector
	Vector  ProjectOnto(const Vector& onto);

	// copy constructors
	//	Vector(const Vector &vOther);

	// arithmetic operations
	Vector	operator-(void) const;

	Vector	operator+(const Vector& v) const {
		Vector n;
		n.x = this->x + v.x;
		n.y = this->y + v.y;
		n.z = this->z + v.z;
		return n;
	}
	Vector	operator-(const Vector& v) const {
		Vector n;
		n.x = this->x - v.x;
		n.y = this->y - v.y;
		n.z = this->z - v.z;
		return n;
	}
	Vector operator*(const Vector &v) const
	{
		return Vector(x * v.x, y * v.y, z * v.z);
	}
	Vector	operator/(const Vector& v) const;
	Vector operator+(float fl) const {
		Vector n;
		n.x = this->x + fl;
		n.y = this->y + fl;
		n.z = this->z + fl;
		return n;
	}
	Vector operator-(float fl) const {
		Vector n;
		n.x = this->x - fl;
		n.y = this->y - fl;
		n.z = this->z - fl;
		return n;
	}
	Vector	operator*(float fl) const {
		Vector n;
		n.x = this->x * fl;
		n.y = this->y * fl;
		n.z = this->z * fl;
		return n;
	}
	Vector	operator/(float fl) const {
		Vector n;
		n.x = this->x / fl;
		n.y = this->y / fl;
		n.z = this->z / fl;
		return n;
	}

	void VectorCrossProduct(const Vector& a, const Vector& b, Vector& result)
	{
		
	}

	// Cross product between two vectors.
	Vector Cross(const Vector &vOther) const
	{
		Vector res;
		// VectorCrossProduct
		res.x = y * vOther.z - z * vOther.y;
		res.y = z * vOther.x - x * vOther.z;
		res.z = x * vOther.y - y * vOther.x;
		return res;
	}

	// Returns a vector with the min or max in X, Y, and Z.
	Vector	Min(const Vector &vOther) const;
	Vector	Max(const Vector &vOther) const;

	void NormalizeAngles();
};

class __declspec(align(16)) VectorAligned : public Vector {
public:
	inline VectorAligned(void) {};
	inline VectorAligned(vec_t X, vec_t Y, vec_t Z) {
		Init(X, Y, Z);
		w = 0.0f;
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

//=========================================================
// 2D Vector2D
//=========================================================

class Vector2D
{
public:
	// Members
	vec_t x, y;

	// Construction/destruction
	Vector2D(void) {
		x = 0;
		y = 0;
	}

	Vector2D(vec_t X, vec_t Y) {
		x = X;
		y = Y;
	}

	Vector2D(const float *pFloat) {
		x = pFloat[0];
		y = pFloat[1];
	}

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f);

	// Got any nasty NAN's?
	bool IsValid() const;

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	vec_t* Base();
	vec_t const* Base() const;

	// Initialization methods
	void Random(float minVal, float maxVal);

	// equality
	bool operator==(const Vector2D& v) const;
	bool operator!=(const Vector2D& v) const;

	// arithmetic operations
	Vector2D&	operator+=(const Vector2D &v);
	Vector2D&	operator-=(const Vector2D &v);
	Vector2D&	operator*=(const Vector2D &v);
	Vector2D&	operator*=(float s);
	Vector2D&	operator/=(const Vector2D &v);
	Vector2D&	operator/=(float s);

	// negate the Vector2D components
	void	Negate();

	// Get the Vector2D's magnitude.
	vec_t	Length() const;

	// Get the Vector2D's magnitude squared.
	vec_t	LengthSqr(void) const;

	// return true if this vector is (0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance);
	}

	// Normalize in place and return the old length.
	vec_t	NormalizeInPlace();

	// Compare length.
	bool	IsLengthGreaterThan(float val) const;
	bool	IsLengthLessThan(float val) const;

	// Get the distance from this Vector2D to the other one.
	vec_t	DistTo(const Vector2D &vOther) const;

	// Get the distance from this Vector2D to the other one squared.
	vec_t	DistToSqr(const Vector2D &vOther) const;

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual Vector2D equation (because it's done per-component
	// rather than per-Vector2D).
	void	MulAdd(const Vector2D& a, const Vector2D& b, float scalar);

	// Dot product.
	vec_t	Dot(const Vector2D& vOther) const;

	// assignment
	Vector2D& operator=(const Vector2D &vOther) {
		x = vOther.x;
		y = vOther.y;
		return *this;
	}

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// copy constructors
	Vector2D(const Vector2D &vOther);

	// arithmetic operations
	Vector2D	operator-(void) const;

	Vector2D	operator+(const Vector2D& v) const;
	Vector2D	operator-(const Vector2D& v) const;
	Vector2D	operator*(const Vector2D& v) const;
	Vector2D	operator/(const Vector2D& v) const;
	Vector2D	operator*(float fl) const;
	Vector2D	operator/(float fl) const;

	// Cross product between two vectors.
	Vector2D	Cross(const Vector2D &vOther) const;

	// Returns a Vector2D with the min or max in X, Y, and Z.
	Vector2D	Min(const Vector2D &vOther) const;
	Vector2D	Max(const Vector2D &vOther) const;

#else

private:
	// No copy constructors allowed if we're in optimal mode
	Vector2D(const Vector2D& vOther);
#endif
};

//=========================================================
// 4D Vector4D
//=========================================================

class Vector4D
{
public:
	// Members
	vec_t x, y, z, w;

	// Construction/destruction
	Vector4D(void);
	Vector4D(vec_t X, vec_t Y, vec_t Z, vec_t W);
	Vector4D(const float *pFloat);

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f, vec_t iw = 0.0f);
	void Init(const Vector& src, vec_t iw = 0.0f);

	// Got any nasty NAN's?
	bool IsValid() const;

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	inline vec_t* Base();
	inline vec_t const* Base() const;

	// Cast to Vector and Vector2D...
	Vector& AsVector3D();
	Vector const& AsVector3D() const;

	Vector2D& AsVector2D();
	Vector2D const& AsVector2D() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);

	// equality
	bool operator==(const Vector4D& v) const;
	bool operator!=(const Vector4D& v) const;

	// arithmetic operations
	Vector4D&	operator+=(const Vector4D &v);
	Vector4D&	operator-=(const Vector4D &v);
	Vector4D&	operator*=(const Vector4D &v);
	Vector4D&	operator*=(float s);
	Vector4D&	operator/=(const Vector4D &v);
	Vector4D&	operator/=(float s);

	Vector4D	operator-(void) const;
	Vector4D	operator*(float fl) const;
	Vector4D	operator/(float fl) const;
	Vector4D	operator*(const Vector4D& v) const;
	Vector4D	operator+(const Vector4D& v) const;
	Vector4D	operator-(const Vector4D& v) const;

	// negate the Vector4D components
	void	Negate();

	// Get the Vector4D's magnitude.
	vec_t	Length() const;

	// Get the Vector4D's magnitude squared.
	vec_t	LengthSqr(void) const;

	// return true if this vector is (0,0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance &&
			w > -tolerance && w < tolerance);
	}

	// Get the distance from this Vector4D to the other one.
	vec_t	DistTo(const Vector4D &vOther) const;

	// Get the distance from this Vector4D to the other one squared.
	vec_t	DistToSqr(const Vector4D &vOther) const;

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual Vector4D equation (because it's done per-component
	// rather than per-Vector4D).
	void	MulAdd(Vector4D const& a, Vector4D const& b, float scalar);

	// Dot product.
	vec_t	Dot(Vector4D const& vOther) const;

	// No copy constructors allowed if we're in optimal mode
#ifdef VECTOR_NO_SLOW_OPERATIONS
private:
#else
public:
#endif
	Vector4D(Vector4D const& vOther);

	// No assignment operators either...
	Vector4D& operator=(Vector4D const& src);
};

struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin)
	{
		m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
		m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
		m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void Invalidate(void)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_flMatVal[i][j] = -1.0f; // VEC_T_NAN is actuall not -1.0f, idk. Shouldn't be used
			}
		}
	}

	float *operator[](int i) { return m_flMatVal[i]; }
	const float *operator[](int i) const { return m_flMatVal[i]; }
	float *Base() { return &m_flMatVal[0][0]; }
	const float *Base() const { return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};

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

class Quaternion;

class RadianEuler
{
public:
	inline RadianEuler(void) { }

	inline RadianEuler(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	inline RadianEuler(Quaternion const &q);    // evil auto type promotion!!!
	inline RadianEuler(QAngle const &angles);    // evil auto type promotion!!!

												 // Initialization
	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix;
		y = iy;
		z = iz;
	}

	//	conversion to qangle
	QAngle ToQAngle(void) const;

	bool IsValid() const;

	void Invalidate();

	inline float *Base()
	{
		return &x;
	}

	inline const float *Base() const
	{
		return &x;
	}

	// array access...
	float operator[](int i) const;

	float &operator[](int i);

	float x, y, z;
};

class Quaternion				// same data-layout as engine's vec4_t,
{                                //		which is a float[4]
public:
	inline Quaternion(void) { }

	inline Quaternion(float ix, float iy, float iz, float iw) : x(ix), y(iy), z(iz), w(iw) { }

	inline Quaternion(RadianEuler const &angle);    // evil auto type promotion!!!

	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f)
	{
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}

	bool IsValid() const;

	void Invalidate();

	bool operator==(const Quaternion &src) const;

	bool operator!=(const Quaternion &src) const;

	float *Base()
	{
		return (float *) this;
	}

	const float *Base() const
	{
		return (float *) this;
	}

	// array access...
	float operator[](int i) const;

	float &operator[](int i);

	float x, y, z, w;
};

void inline SinCos(float radians, float *sine, float *cosine);
void VectorAngles(const float *forward, float *angles);
void VectorAngles(const Vector& forward, QAngle &angles);
void AngleVectors(const QAngle &angles, Vector *forward = NULL, Vector *right = NULL, Vector *up = NULL);
void VectorTransform(Vector& vIn, const matrix3x4_t& mIn, Vector& out);
void VectorRotate(Vector& in1, const matrix3x4_t& in2, Vector* out);

void MatrixCopy(const matrix3x4_t& in, matrix3x4_t& out);
void MatrixSetColumn(const Vector &in, int column, matrix3x4_t& out);
void MatrixGetColumn(const matrix3x4_t& in, int column, Vector &out);
void AngleMatrix(const QAngle &angles, const Vector &position, matrix3x4_t& matrix);
void AngleMatrix(const QAngle &angles, matrix3x4_t& matrix);


typedef Vector QAngle;
inline void VectorRotate(const Vector &i, const matrix3x4_t &matrix, Vector &o)
{
	o.x = i.Dot(Vector(matrix[0][0], matrix[0][1], matrix[0][2]));
	o.y = i.Dot(Vector(matrix[1][0], matrix[1][1], matrix[1][2]));
	o.z = i.Dot(Vector(matrix[2][0], matrix[2][1], matrix[2][2]));
}

inline void VectorRotate(Vector &i, const QAngle &angles, Vector &o)
{
	matrix3x4_t matrix;

	AngleMatrix(angles, matrix);
	VectorRotate(i, matrix, o);
}

inline Vector VectorRotate(Vector &i, const QAngle &angles)
{
	Vector o;
	VectorRotate(i, angles, o);

	return o;
}

template<typename T>
inline T Min(T x, T y)
{
	return x < y ? x : y;
}

template<typename T>
inline T Max(T x, T y)
{
	return x > y ? x : y;
}

template<typename T>
constexpr T minof()
{
	return (T)1 << (sizeof(T) * 8 - 1);
}

template<typename T>
constexpr T maxof()
{
	return ~((T)1 << (sizeof(T) * 8 - 1));
}

typedef __m128 fltx4;

fltx4 LoadUnalignedSIMD(const void *pSIMD);

#define MM_SHUFFLE_REV(a,b,c,d) _MM_SHUFFLE(d,c,b,a)
fltx4 SplatXSIMD(fltx4 const & a);

fltx4 SplatYSIMD(fltx4 const &a);

fltx4 SplatZSIMD(fltx4 const &a);

fltx4 MulSIMD(const fltx4 & a, const fltx4 & b);

fltx4 AddSIMD(const fltx4 & a, const fltx4 & b);

fltx4 AndSIMD(const fltx4 & a, const fltx4 & b);

#define RESTRICT __restrict
void StoreUnalignedSIMD(float * RESTRICT pSIMD, const fltx4 & a);

#define DECL_ALIGN(x) __declspec(align(x))
#define ALIGN16 DECL_ALIGN(16)
#define ALIGN16_POST
const int ALIGN16 g_SIMD_ComponentMask[4][4] ALIGN16_POST =
{
	{ 0xFFFFFFFF, 0, 0, 0 },{ 0, 0xFFFFFFFF, 0, 0 },{ 0, 0, 0xFFFFFFFF, 0 },{ 0, 0, 0, 0xFFFFFFFF }
};

void ConcatTransforms(const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out);

void MatrixAngles(const matrix3x4_t& matrix, float *angles);

#endif // __VECTOR_H__