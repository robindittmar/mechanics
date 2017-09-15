#ifndef __RAY_H__
#define __RAY_H__

#include "Vector.h"

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