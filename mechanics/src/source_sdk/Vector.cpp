#include "Vector.h"
#include "../Application.h"

void Vector::NormalizeAngles()
{
	// Normalize pitch
	while (this->x > 89.0f)
	{
		this->x -= 178.0f;
	}
	while (this->x < -89.0f)
	{
		this->x += 178.0f;
	}

	// Normalize yaw
	while (this->y > 179.9999f)
	{
		this->y -= 360.0f;
	}
	while (this->y < -179.9999f)
	{
		this->y += 360.0f;
	}

	// Roll is always zero (untrusted)
	this->z = 0.0f;
}

void inline SinCos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

void VectorAngles(const float *forward, float *angles)
{
	//Assert(s_bMathlibInitialized);
	float	tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180.0f / PI_F);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180.0f / PI_F);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void VectorAngles(const Vector& forward, QAngle &angles)
{
	float	tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / PI_F);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / PI_F);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void AngleVectors(const QAngle &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;
	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}

float DotProduct(Vector& v1, const float* v2)
{
	return v1.x*v2[0] + v1.y*v2[1] + v1.z*v2[2];
}

void VectorTransform(Vector& vIn, const matrix3x4_t& mIn, Vector& out)
{
	out.x = DotProduct(vIn, mIn[0]) + mIn[0][3];
	out.y = DotProduct(vIn, mIn[1]) + mIn[1][3];
	out.z = DotProduct(vIn, mIn[2]) + mIn[2][3];
}

// assume in2 is a rotation and rotate the input vector
void VectorRotate(Vector& in1, const matrix3x4_t& in2, Vector* out)
{
	out->x = DotProduct(in1, in2[0]);
	out->y = DotProduct(in1, in2[1]);
	out->z = DotProduct(in1, in2[2]);
}

void MatrixCopy(const matrix3x4_t& in, matrix3x4_t& out)
{
	memcpy(out.Base(), in.Base(), sizeof(float) * 3 * 4);
}

void MatrixSetColumn(const Vector &in, int column, matrix3x4_t& out)
{
	out[0][column] = in.x;
	out[1][column] = in.y;
	out[2][column] = in.z;
}

void MatrixGetColumn(const matrix3x4_t& in, int column, Vector &out)
{
	out.x = in[0][column];
	out.y = in[1][column];
	out.z = in[2][column];
}

void AngleMatrix(const QAngle &angles, const Vector &position, matrix3x4_t& matrix)
{
	AngleMatrix(angles, matrix);
	MatrixSetColumn(position, 3, matrix);
}

void AngleMatrix(const QAngle &angles, matrix3x4_t& matrix)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;
	matrix[2][0] = -sp;

	float crcy = cr*cy;
	float crsy = cr*sy;
	float srcy = sr*cy;
	float srsy = sr*sy;
	matrix[0][1] = sp*srcy - crsy;
	matrix[1][1] = sp*srsy + crcy;
	matrix[2][1] = sr*cp;

	matrix[0][2] = (sp*crcy + srsy);
	matrix[1][2] = (sp*crsy - srcy);
	matrix[2][2] = cr*cp;

	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
}

fltx4 LoadUnalignedSIMD(const void *pSIMD)
{
	return _mm_loadu_ps(reinterpret_cast<const float *>(pSIMD));
}

fltx4 SplatXSIMD(fltx4 const & a)
{
	return _mm_shuffle_ps(a, a, MM_SHUFFLE_REV(0, 0, 0, 0));
}

fltx4 SplatYSIMD(fltx4 const &a)
{
	return _mm_shuffle_ps(a, a, MM_SHUFFLE_REV(1, 1, 1, 1));
}

fltx4 SplatZSIMD(fltx4 const &a)
{
	return _mm_shuffle_ps(a, a, MM_SHUFFLE_REV(2, 2, 2, 2));
}

fltx4 MulSIMD(const fltx4 & a, const fltx4 & b)				// a*b
{
	return _mm_mul_ps(a, b);
}

fltx4 AddSIMD(const fltx4 & a, const fltx4 & b)				// a+b
{
	return _mm_add_ps(a, b);
}

fltx4 AndSIMD(const fltx4 & a, const fltx4 & b)				// a & b
{
	return _mm_and_ps(a, b);
}

void StoreUnalignedSIMD(float * RESTRICT pSIMD, const fltx4 & a)
{
	_mm_storeu_ps(pSIMD, a);
}

void ConcatTransforms(const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out)
{
	fltx4 lastMask = *(fltx4 *)(&g_SIMD_ComponentMask[3]);
	fltx4 rowA0 = LoadUnalignedSIMD(in1.m_flMatVal[0]);
	fltx4 rowA1 = LoadUnalignedSIMD(in1.m_flMatVal[1]);
	fltx4 rowA2 = LoadUnalignedSIMD(in1.m_flMatVal[2]);

	fltx4 rowB0 = LoadUnalignedSIMD(in2.m_flMatVal[0]);
	fltx4 rowB1 = LoadUnalignedSIMD(in2.m_flMatVal[1]);
	fltx4 rowB2 = LoadUnalignedSIMD(in2.m_flMatVal[2]);

	// now we have the rows of m0 and the columns of m1
	// first output row
	fltx4 A0 = SplatXSIMD(rowA0);
	fltx4 A1 = SplatYSIMD(rowA0);
	fltx4 A2 = SplatZSIMD(rowA0);
	fltx4 mul00 = MulSIMD(A0, rowB0);
	fltx4 mul01 = MulSIMD(A1, rowB1);
	fltx4 mul02 = MulSIMD(A2, rowB2);
	fltx4 out0 = AddSIMD(mul00, AddSIMD(mul01, mul02));

	// second output row
	A0 = SplatXSIMD(rowA1);
	A1 = SplatYSIMD(rowA1);
	A2 = SplatZSIMD(rowA1);
	fltx4 mul10 = MulSIMD(A0, rowB0);
	fltx4 mul11 = MulSIMD(A1, rowB1);
	fltx4 mul12 = MulSIMD(A2, rowB2);
	fltx4 out1 = AddSIMD(mul10, AddSIMD(mul11, mul12));

	// third output row
	A0 = SplatXSIMD(rowA2);
	A1 = SplatYSIMD(rowA2);
	A2 = SplatZSIMD(rowA2);
	fltx4 mul20 = MulSIMD(A0, rowB0);
	fltx4 mul21 = MulSIMD(A1, rowB1);
	fltx4 mul22 = MulSIMD(A2, rowB2);
	fltx4 out2 = AddSIMD(mul20, AddSIMD(mul21, mul22));

	// add in translation vector
	A0 = AndSIMD(rowA0, lastMask);
	A1 = AndSIMD(rowA1, lastMask);
	A2 = AndSIMD(rowA2, lastMask);
	out0 = AddSIMD(out0, A0);
	out1 = AddSIMD(out1, A1);
	out2 = AddSIMD(out2, A2);

	// write to output
	StoreUnalignedSIMD(out.m_flMatVal[0], out0);
	StoreUnalignedSIMD(out.m_flMatVal[1], out1);
	StoreUnalignedSIMD(out.m_flMatVal[2], out2);
}

void MatrixAngles(const matrix3x4_t& matrix, float *angles)
{
	float forward[3];
	float left[3];
	float up[3];

	//
	// Extract the basis vectors from the matrix. Since we only need the Z
	// component of the up vector, we don't get X and Y.
	//
	forward[0] = matrix[0][0];
	forward[1] = matrix[1][0];
	forward[2] = matrix[2][0];
	left[0] = matrix[0][1];
	left[1] = matrix[1][1];
	left[2] = matrix[2][1];
	up[2] = matrix[2][2];

	float xyDist = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);

	// enough here to get angles?
	if (xyDist > 0.001f)
	{
		// (yaw)	y = ATAN( forward.y, forward.x );		-- in our space, forward is the X axis
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

		// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

		// (roll)	z = ATAN( left.z, up.z );
		angles[2] = RAD2DEG(atan2f(left[2], up[2]));
	}
	else	// forward is mostly Z, gimbal lock-
	{
		// (yaw)	y = ATAN( -left.x, left.y );			-- forward is mostly z, so use right for yaw
		angles[1] = RAD2DEG(atan2f(-left[0], left[1]));

		// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

		// Assume no roll in this case as one degree of freedom has been lost (i.e. yaw == roll)
		angles[2] = 0;
	}
}
