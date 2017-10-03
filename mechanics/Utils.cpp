#include "Utils.h"

namespace Utils
{
	QAngle CalcAngle(Vector& vStartPos, Vector& vEndPos)
	{
		Vector vRelativeDist = vEndPos - vStartPos;
		QAngle qAngle(
			RAD2DEG(-asinf(vRelativeDist.z / vRelativeDist.Length())),
			RAD2DEG(atan2f(vRelativeDist.y, vRelativeDist.x)),
			0.0f
		);

		return qAngle;
	}

	float GetViewangleDist(QAngle& qSource, QAngle& qTarget/*, float fOriginDistance*/)
	{
		QAngle qDist = qTarget - qSource;
		qDist.NormalizeAngles();
		float fAng = qDist.Length();

		// Causes weird angles to have giant "fovs"
		//return (sinf(DEG2RAD(fAng)) * fOriginDistance);
		return fAng;
	}

	void GetHitBoxVectors(mstudiobbox_t* hitBox, matrix3x4_t* boneMatrix, Vector* hitBoxVectors)
	{
		Vector bbmin;
		Vector bbmax;

		// TODO: Try FixHitbox instead (trace from edge points to center and use trace.endpos as new edge)
		float fMod = hitBox->m_flRadius != -1.0f ? hitBox->m_flRadius : 0.0f;
		bbmin = (hitBox->m_vecBBMin - fMod) * 0.65f;
		bbmax = (hitBox->m_vecBBMax + fMod) * 0.65f;

		Vector vPoints[] = {
			(bbmin + bbmax) * 0.5f,
			Vector(bbmin.x, bbmin.y, bbmin.z),
			Vector(bbmin.x, bbmax.y, bbmin.z),
			Vector(bbmax.x, bbmax.y, bbmin.z),
			Vector(bbmax.x, bbmin.y, bbmin.z),
			Vector(bbmax.x, bbmax.y, bbmax.z),
			Vector(bbmin.x, bbmax.y, bbmax.z),
			Vector(bbmin.x, bbmin.y, bbmax.z),
			Vector(bbmax.x, bbmin.y, bbmax.z)
		};

		for (int i = 0; i < sizeof(vPoints) / sizeof(Vector); i++)
		{
			/*if (i != 0)
			vPoints[i] = ((((vPoints[i] + vPoints[0]) * 0.5f) + vPoints[i]) * 0.5f);*/

			VectorTransform(vPoints[i], boneMatrix[hitBox->m_iBone], hitBoxVectors[i]);
		}
	}
}