#ifndef __UTILS_H__
#define __UTILS_H__

// Std Lib

// Source SDK
#include "../source_sdk/Vector.h"
#include "../source_sdk/IVModelInfo.h"
#include "../source_sdk/IEngineTrace.h"
#include "../source_sdk/CGameTrace.h"
#include "../source_sdk/ray.h"

// Custom

namespace Utils
{
	QAngle CalcAngle(Vector& vStartPos, Vector& vEndPos);

	float GetOriginDist(Vector& a, Vector& b);
	float GetViewangleDist(QAngle& qSource, QAngle& qTarget/*, float fOriginDistance*/);

	void GetHitBoxCenter(mstudiobbox_t* hitBox, matrix3x4_t* boneMatrix, Vector& hitBoxVector);
	void GetHitBoxVectors(mstudiobbox_t* hitBox, matrix3x4_t* boneMatrix, Vector* hitBoxVectors, float fPointScale);

	float GetTraceFractionWorldProps(Vector start, Vector end);

	/// <summary>
	/// Convert HSL to RGB
	/// </summary>
	void HsvToRgb(int h, float s, float l, int& r, int &g, int& b);

	/// <summary>
	/// Converts RGB to HSL
	/// </summary>
	void RgbToHsv(int r, int g, int b, float& h, float& s, float& v);
}

#endif // __UTILS_H__