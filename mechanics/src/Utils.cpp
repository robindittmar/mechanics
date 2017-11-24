#include "Utils.h"

namespace Utils
{
	/// <summary>
	/// Calcs the angle between 2 points (useful for aimbot)
	/// </summary>
	/// <param name="vStartPos">Vector3 start position</param>
	/// <param name="vEndPos">Vector3 end position</param>
	/// <returns>QAngle from Start->End</returns>
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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="qSource"></param>
	/// <param name="qTarget"></param>
	/// <returns></returns>
	float GetViewangleDist(QAngle& qSource, QAngle& qTarget/*, float fOriginDistance*/)
	{
		QAngle qDist = qTarget - qSource;
		qDist.NormalizeAngles();
		float fAng = qDist.Length();

		// Causes weird angles to have giant "fovs"
		//return (sinf(DEG2RAD(fAng)) * fOriginDistance);
		return fAng;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="hitBox"></param>
	/// <param name="boneMatrix"></param>
	/// <param name="hitBoxVectors"></param>
	/// <param name="fPointScale"></param>
	void GetHitBoxVectors(mstudiobbox_t* hitBox, matrix3x4_t* boneMatrix, Vector* hitBoxVectors, float fPointScale)
	{
		Vector bbmin;
		Vector bbmax;

		// TODO: Try FixHitbox instead (trace from edge points to center and use trace.endpos as new edge)
		float fMod = hitBox->m_flRadius != -1.0f ? hitBox->m_flRadius : 0.0f;
		bbmin = (hitBox->m_vecBBMin - fMod) * fPointScale;
		bbmax = (hitBox->m_vecBBMax + fMod) * fPointScale;

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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="h"></param>
	/// <param name="s"></param>
	/// <param name="v"></param>
	/// <param name="r"></param>
	/// <param name="g"></param>
	/// <param name="b"></param>
	void HsvToRgb(int h, float s, float v, int& r, int &g, int& b)
	{
		double      hh, p, q, t, ff;
		long        i;

		if (s <= 0.0) {       // < is bogus, just shuts up warnings
			r = v;
			g = v;
			b = v;
			return;
		}
		hh = h;
		if (hh >= 360.0) hh = 0.0;
		hh /= 60.0;
		i = (long)hh;
		ff = hh - i;
		p = v * (1.0 - s);
		q = v * (1.0 - (s * ff));
		t = v * (1.0 - (s * (1.0 - ff)));

		p *= 255.0f;
		q *= 255.0f;
		t *= 255.0f;

		double ll = v * 255.0f;

		switch (i)
		{
		case 0:
			r = ll;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = ll;
			b = p;
			break;
		case 2:
			r = p;
			g = ll;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = ll;
			break;
		case 4:
			r = t;
			g = p;
			b = ll;
			break;
		case 5:
		default:
			r = ll;
			g = p;
			b = q;
			break;
		}
	}

	void RgbToHsv(int r, int g, int b, float& h, float& s, float& v)
	{
		unsigned char rgbMin, rgbMax;

		rgbMin = r < g ? (r < b ? r : b) : (g < b ? g : b);
		rgbMax = r > g ? (r > b ? r : b) : (g > b ? g : b);

		v = rgbMax;
		if (v == 0)
		{
			h = 0;
			s = 0;
			return;
		}

		s = 255 * long(rgbMax - rgbMin) / v;
		if (s == 0)
		{
			h = 0;
			v /= 255.0f;
			return;
		}

		if (rgbMax == r)
			h = 0 + 43 * (g - b) / (rgbMax - rgbMin);
		else if (rgbMax == g)
			h = 85 + 43 * (b - r) / (rgbMax - rgbMin);
		else
			h = 171 + 43 * (r - g) / (rgbMax - rgbMin);

		s /= 255.0f;
		v /= 255.0f;
	}
}