/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_TARGET_H
#define GAME_SERVER_ENTITIES_TARGET_H

#include <game/server/entity.h>

class CTarget : public CEntity
{
private:
	bool m_Disabled;

public:
	CTarget(CGameWorld *pGameWorld, vec2 Pos);

	virtual void Snap(int SnappingClient);
	
	void OnHit(int CID);
	bool IsDisabled();
	void Disable();
	void Enable();
};

#endif
