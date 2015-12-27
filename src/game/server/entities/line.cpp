/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <generated/server_data.h>
#include <game/server/gamecontext.h>
#include <modapi/compatibility.h>
#include <game/modapi-example-def.h>

#include "line.h"

CLine::CLine(CGameWorld *pGameWorld, vec2 StartPos, vec2 EndPos, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_LINE, StartPos)
{
	m_Owner = Owner;
	m_EndPos = EndPos;
	m_StartTick = Server()->Tick();
	m_LifeSpan = Server()->TickSpeed()*10;
	GameWorld()->InsertEntity(this);
}

void CLine::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CLine::Tick()
{
	--m_LifeSpan;
	if(m_LifeSpan <= 0)
	{
		Reset();
	}	
}

void CLine::TickPaused()
{
	++m_StartTick;
}

void CLine::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient) && NetworkClipped(SnappingClient, m_EndPos))
		return;

	if(Server()->GetClientProtocolCompatibility(SnappingClient, MODAPI_COMPATIBILITY_LINE))
	{
		CNetObj_ModAPI_Line* pLine = static_cast<CNetObj_ModAPI_Line*>(Server()->SnapNewItem(NETOBJTYPE_MODAPI_LINE, GetID(), sizeof(CNetObj_ModAPI_Line)));
		if(!pLine)
			return;
			
		pLine->m_LineStyleId = MODAPIEXAMPLE_LINESTYLE_XMAS;
		pLine->m_StartX = (int) m_Pos.x;
		pLine->m_StartY = (int) m_Pos.y;
		pLine->m_EndX = (int)m_EndPos.x;
		pLine->m_EndY = (int)m_EndPos.y;
		pLine->m_StartTick = m_StartTick;
	}
	else
	{
		CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, GetID(), sizeof(CNetObj_Laser)));
		if(!pObj)
			return;

		pObj->m_X = (int)m_EndPos.x;
		pObj->m_Y = (int)m_EndPos.y;
		pObj->m_FromX = (int)m_Pos.x;
		pObj->m_FromY = (int)m_Pos.y;
		pObj->m_StartTick = m_StartTick;
	}
	
}
