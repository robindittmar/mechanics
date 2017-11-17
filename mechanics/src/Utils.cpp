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
	/// <param name="l"></param>
	/// <param name="r"></param>
	/// <param name="g"></param>
	/// <param name="b"></param>
	void HslToRgb(int h, float s, float l, int& r, int &g, int& b)
	{
		double      hh, p, q, t, ff;
		long        i;

		if (s <= 0.0) {       // < is bogus, just shuts up warnings
			r = l;
			g = l;
			b = l;
			return;
		}
		hh = h;
		if (hh >= 360.0) hh = 0.0;
		hh /= 60.0;
		i = (long)hh;
		ff = hh - i;
		p = l * (1.0 - s);
		q = l * (1.0 - (s * ff));
		t = l * (1.0 - (s * (1.0 - ff)));

		p *= 255.0f;
		q *= 255.0f;
		t *= 255.0f;

		double ll = l * 255.0f;

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

	void RgbToHsl(int r, int g, int b, float& h, float& s, float& l)
	{
		double r_percent = ((double)r) / 255.0;
		double g_percent = ((double)g) / 255.0;
		double b_percent = ((double)b) / 255.0;

		double max_color = 0;
		if ((r_percent >= g_percent) && (r_percent >= b_percent))
			max_color = r_percent;
		if ((g_percent >= r_percent) && (g_percent >= b_percent))
			max_color = g_percent;
		if ((b_percent >= r_percent) && (b_percent >= g_percent))
			max_color = b_percent;

		double min_color = 0;
		if ((r_percent <= g_percent) && (r_percent <= b_percent))
			min_color = r_percent;
		if ((g_percent <= r_percent) && (g_percent <= b_percent))
			min_color = g_percent;
		if ((b_percent <= r_percent) && (b_percent <= g_percent))
			min_color = b_percent;

		double L = 0;
		double S = 0;
		double H = 0;

		L = (max_color + min_color) / 2;

		if (max_color == min_color)
		{
			S = 0;
			H = 0;
		}
		else
		{
			if (L < .50)
			{
				S = (max_color - min_color) / (max_color + min_color);
			}
			else
			{
				S = (max_color - min_color) / (2 - max_color - min_color);
			}
			if (max_color == r_percent)
			{
				H = (g_percent - b_percent) / (max_color - min_color);
			}
			if (max_color == g_percent)
			{
				H = 2 + (b_percent - r_percent) / (max_color - min_color);
			}
			if (max_color == b_percent)
			{
				H = 4 + (r_percent - g_percent) / (max_color - min_color);
			}
		}
		s = (S * 100);
		l = (L * 100);
		H = H * 60;
		if (H < 0)
			H += 360;
		h = H;
	}
}