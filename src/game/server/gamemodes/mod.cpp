/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "mod.h"

#include <game/server/entities/character.h>
#include <game/server/entities/target.h>
#include <game/mapitems.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

CGameControllerMOD::CGameControllerMOD(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	// Exchange this to a string that identifies your game mode.
	// DM, TDM and CTF are reserved for teeworlds original modes.
	m_pGameType = "Target";

	//m_GameFlags = GAMEFLAG_TEAMS; // GAMEFLAG_TEAMS makes it a two-team gamemode
}

void CGameControllerMOD::Tick()
{
	// this is the main part of the gamemode, this function is run every tick

	int NbTarget = 0;
	int NbActiveTarget = 0;

	CTarget *p = (CTarget *)GameServer()->m_World.FindFirst(CGameWorld::ENTTYPE_TARGET);
	for(; p; p = (CTarget *)p->TypeNext())
 	{
		NbTarget++;
		if(!p->IsDisabled())
		{
			NbActiveTarget++;
		}
	}
	
	if(NbTarget > 0 && NbActiveTarget == 0)
	{
		int TargetToActivate = rand()%NbTarget;
		int TargetIter = 0;
		
		CTarget *p = (CTarget *)GameServer()->m_World.FindFirst(CGameWorld::ENTTYPE_TARGET);
		for(; p; p = (CTarget *)p->TypeNext())
		{
			if(TargetIter == TargetToActivate)
			{
				p->Enable();
				break;
			}
			TargetIter++;
		}
	}

	IGameController::Tick();
}

void CGameControllerMOD::OnCharacterSpawn(CCharacter *pChr)
{
	pChr->IncreaseHealth(10);
	pChr->IncreaseArmor(5);

	pChr->RemoveWeapon(WEAPON_HAMMER);
	pChr->RemoveWeapon(WEAPON_GUN);
	pChr->RemoveWeapon(WEAPON_SHOTGUN);
	pChr->RemoveWeapon(WEAPON_GRENADE);
	pChr->GiveWeapon(WEAPON_LASER, -1);
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
	
int CGameControllerMOD::OnTargetShooted(class CTarget *pTarget, int Shooter)
{
	if(pTarget && !pTarget->IsDisabled())
	{
		pTarget->Disable();
		
		CPlayer* pShooter = GameServer()->m_apPlayers[Shooter];
		if(pShooter)
		{
			pShooter->m_Score++;
		}
	}
}
