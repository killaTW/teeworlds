/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "mod.h"

#include <game/server/entities/target.h>
#include <game/mapitems.h>
#include <game/server/gamecontext.h>

CGameControllerMOD::CGameControllerMOD(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	// Exchange this to a string that identifies your game mode.
	// DM, TDM and CTF are reserved for teeworlds original modes.
	m_pGameType = "MOD";

	//m_GameFlags = GAMEFLAG_TEAMS; // GAMEFLAG_TEAMS makes it a two-team gamemode
}

void CGameControllerMOD::Tick()
{
	// this is the main part of the gamemode, this function is run every tick

	IGameController::Tick();
}

bool CGameControllerMOD::OnEntity(int Index, vec2 Pos)
{
	if(IGameController::OnEntity(Index, Pos))
		return true;
	
	switch(Index)
	{
	case ENTITY_ARMOR_1:
	case ENTITY_HEALTH_1:
	case ENTITY_WEAPON_SHOTGUN:
	case ENTITY_WEAPON_GRENADE:
	case ENTITY_WEAPON_LASER:
	case ENTITY_POWERUP_NINJA:
		new CTarget(&GameServer()->m_World, Pos);
		return true;
	}

	return false;
}
