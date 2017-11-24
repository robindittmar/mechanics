#ifndef __UTILS_H__
#define __UTILS_H__

#include "Vector.h"
#include "IVModelInfo.h"

namespace Utils
{
	QAngle CalcAngle(Vector& vStartPos, Vector& vEndPos);
	float GetViewangleDist(QAngle& qSource, QAngle& qTarget/*, float fOriginDistance*/);

	void GetHitBoxVectors(mstudiobbox_t* hitBox, matrix3x4_t* boneMatrix, Vector* hitBoxVectors, float fPointScale);

	/// <summary>
	/// Convert HSL to RGB
	/// </summary>
	void HsvToRgb(int h, float s, float l, int& r, int &g, int& b);

	/// <summary>
	/// Converts RGB to HSl
	/// </summary>
	void RgbToHsv(int r, int g, int b, float& h, float& s, float& v);
}

#endif // __UTILS_H__