/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <generated/server_data.h>
#include <game/server/gamecontext.h>
#include <game/server/gamecontroller.h>
#include <game/server/player.h>
#include <game/modapi-example-def.h>
#include <modapi/compatibility.h>

#include "character.h"
#include "target.h"

CTarget::CTarget(CGameWorld *pGameWorld, vec2 Pos)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_TARGET, Pos)
{
	Reset();

	GameWorld()->InsertEntity(this);
	
	m_Disabled = true;
}

void CTarget::Snap(int SnappingClient)
{
	if(m_Disabled)
		return;
	
	if(NetworkClipped(SnappingClient))
		return;

	if(Server()->GetClientProtocolCompatibility(SnappingClient, MODAPI_COMPATIBILITY_SPRITE))
	{
		CNetObj_ModAPI_Sprite* pSprite = static_cast<CNetObj_ModAPI_Sprite*>(Server()->SnapNewItem(NETOBJTYPE_MODAPI_SPRITE, GetID(), sizeof(CNetObj_ModAPI_Sprite)));
		if(!pSprite)
			return;
			
		pSprite->m_SpriteId = MODAPIEXAMPLE_SPRITE_TARGET;
		pSprite->m_X = (int) m_Pos.x;
		pSprite->m_Y = (int) m_Pos.y;
		pSprite->m_Size = 64;
		pSprite->m_Angle = 0;
	}
	else
	{
		CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, GetID(), sizeof(CNetObj_Pickup)));
		if(!pP)
			return;

		pP->m_X = (int)m_Pos.x;
		pP->m_Y = (int)m_Pos.y;
		pP->m_Type = PICKUP_ARMOR;
	}
}
	
void CTarget::OnHit(int CID)
{
	GameServer()->m_pController->OnTargetShooted(this, CID);
}

bool CTarget::IsDisabled()
{
	return m_Disabled;
}

void CTarget::Disable()
{
	GameServer()->CreateExplosion(m_Pos, -1, -1, 0);
			
	m_Disabled = true;
}

void CTarget::Enable()
{
	GameServer()->CreatePlayerSpawn(m_Pos);
	
	m_Disabled = false;
}