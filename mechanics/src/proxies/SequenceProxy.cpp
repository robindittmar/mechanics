#include "SequenceProxy.h"
#include "../Application.h"

RecvVarProxy_t g_pSequenceProxy;

void __cdecl hk_SetViewModelSequence(const CRecvProxyData* pDataConst, void* pStruct, void* pOut)
{
	CApplication* pApp = CApplication::Instance();

	// Make the incoming data editable.
	CRecvProxyData* pData = (CRecvProxyData*)pDataConst;

	// Confirm that we are replacing our view model and not someone elses.
	CBaseViewModel* pViewModel = (CBaseViewModel*)pStruct;

	if (pViewModel) {
		IClientEntity* pOwner = pApp->EntityList()->GetClientEntityFromHandle(pViewModel->GetOwner());

		// Compare the owner entity of this view model to the local player entity.
		if (pOwner && pOwner->EntIndex() == pApp->EngineClient()->GetLocalPlayer()) {
			// Get the filename of the current view model.
			const model_t* pModel = pApp->ModelInfo()->GetModel(pViewModel->GetModelIndex());
			const char* szModel = pApp->ModelInfo()->GetModelName(pModel);

			// Store the current sequence.
			int m_nSequence = pData->m_Value.m_Int;

			if (!strcmp(szModel, CXorString(/*models/weapons/v_knife_butterfly.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xA0\x62\x7F\xF1\xA7\x65\x6D\xE9\xBB\x39\x66\xE1\xAE").ToCharArray())) {
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomIntDef(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomIntDef(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
				default:
					m_nSequence++;
				}
			}
			else if (!strcmp(szModel, CXorString(/*models/weapons/v_knife_falchion_advanced.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xA4\x76\x67\xE6\xAA\x7E\x64\xEB\x9D\x76\x6F\xF3\xA3\x79\x68\xE0\xA6\x39\x66\xE1\xAE").ToCharArray())) {
				// Fix animations for the Falchion Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomIntDef(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomIntDef(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(szModel, CXorString(/*models/weapons/v_knife_push.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xB2\x62\x78\xED\xEC\x7A\x6F\xE9").ToCharArray())) {
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomIntDef(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomIntDef(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (!strcmp(szModel, CXorString(/*models/weapons/v_knife_survival_bowie.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xB1\x62\x79\xF3\xAB\x61\x6A\xE9\x9D\x75\x64\xF2\xAB\x72\x25\xE8\xA6\x7B").ToCharArray())) {
				// Fix animations for the Bowie Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1; break;
				default:
					m_nSequence--;
				}
			}

			// Set the fixed sequence.
			pData->m_Value.m_Int = m_nSequence;
		}
	}

	// Call original function with the modified data.
	g_pSequenceProxy(pData, pStruct, pOut);
}
