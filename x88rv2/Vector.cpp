#include "Vector.h"
#include "Application.h"

void Vector::Normalize()
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