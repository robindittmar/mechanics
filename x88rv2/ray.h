#ifndef __RAY_H__
#define __RAY_H__

#include "Vector.h"

void VectorITransform(Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	float in1t[3];

	in1t[0] = in1.x - in2.c[0][3];
	in1t[1] = in1.y - in2.c[1][3];
	in1t[2] = in1.z - in2.c[2][3];

	out.x = in1t[0] * in2.c[0][0] + in1t[1] * in2.c[1][0] + in1t[2] * in2.c[2][0];
	out.y = in1t[0] * in2.c[0][1] + in1t[1] * in2.c[1][1] + in1t[2] * in2.c[2][1];
	out.z = in1t[0] * in2.c[0][2] + in1t[1] * in2.c[1][2] + in1t[2] * in2.c[2][2];
}

void VectorIRotate(Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	out.x = in1.x*in2.c[0][0] + in1.y*in2.c[1][0] + in1.z*in2.c[2][0];
	out.y = in1.x*in2.c[0][1] + in1.y*in2.c[1][1] + in1.z*in2.c[2][1];
	out.z = in1.x*in2.c[0][2] + in1.y*in2.c[1][2] + in1.z*in2.c[2][2];
}

struct Ray_t
{
	__declspec(align(16)) Vector  m_Start;
	__declspec(align(16)) Vector  m_Delta;
	__declspec(align(16)) Vector  m_StartOffset;
	__declspec(align(16)) Vector  m_Extents;
	//without your matrix3x4
	bool	m_IsRay;
	bool	m_IsSwept;

	void Init(Vector const& start, Vector const& end)
	{
		/*Assert(&end);
		VectorSubtract(end, start, m_Delta);

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		VectorClear(m_Extents);
		m_IsRay = true;

		VectorClear(m_StartOffset);
		VectorCopy(start, m_Start);*/

		m_Delta.x = end.x;
		m_Delta.y = end.y;
		m_Delta.z = end.z;
		m_Delta -= start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.x = 0;
		m_Extents.y = 0;
		m_Extents.z = 0;

		m_IsRay = true;

		m_StartOffset.x = 0;
		m_StartOffset.y = 0;
		m_StartOffset.z = 0;
		m_Start.x = start.x;
		m_Start.y = start.y;
		m_Start.z = start.z;
	}
};

#endif // __RAY_H__