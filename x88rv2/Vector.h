/// 
/// Source Engine declarations
///

#include <cmath>
#include <intrin.h>

#ifndef __VECTOR_H__
#define __VECTOR_H__

class matrix3x4_t;

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
		return sqrt(x * x + y * y);
	}
	vec_t	Length2DSqr(void) const;

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
	Vector	operator*(const Vector& v) const;
	Vector	operator/(const Vector& v) const;
	Vector	operator*(float fl) const;
	Vector	operator/(float fl) const {
		Vector n;
		n.x = this->x / fl;
		n.y = this->y / fl;
		n.z = this->z / fl;
		return n;
	}

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

void VectorAngles(const float *forward, float *angles);

#endif // __VECTOR_H__