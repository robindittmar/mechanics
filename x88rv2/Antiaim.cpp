#include "Antiaim.h"
#include "Application.h"

CAntiaim::CAntiaim()
{

}

CAntiaim::~CAntiaim()
{
}

void CAntiaim::Setup()
{
	m_pApp = CApplication::Instance();
}

void CAntiaim::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	Antiaim antiAimInitizalize = *(Antiaim*)pParameters;
	QAngle angles;
	this->m_pApp->EngineClient()->GetViewAngles(angles);

	// Pitch
	switch (antiAimInitizalize.pitch)
	{
	case DOWN:
		antiAimInitizalize.pUserCmd->viewangles[0] = 89;
		break;
	}


	// Yaw
	switch (antiAimInitizalize.yaw)
	{
	case BACKWARDS:
		angles.y -= 180;
		while (angles.y > 180.0f)
		{
			angles.y -= 360.0f;
		}
		while (angles.y < -180.0f)
		{
			angles.y += 360.0f;
		}
		antiAimInitizalize.pUserCmd->viewangles[1] = angles.y;
		break;
	case JITTER_BACKWARDS:
		break;
	}
}
