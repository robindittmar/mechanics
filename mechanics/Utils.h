#ifndef __UTILS_H__
#define __UTILS_H__

#include "Vector.h"
#include "IVModelInfo.h"

namespace Utils
{
	QAngle CalcAngle(Vector& vStartPos, Vector& vEndPos);
	float GetViewangleDist(QAngle& qSource, QAngle& qTarget/*, float fOriginDistance*/);

	void GetHitBoxVectors(mstudiobbox_t* hitBox, matrix3x4_t* boneMatrix, Vector* hitBoxVectors);
}

#endif // __UTILS_H__