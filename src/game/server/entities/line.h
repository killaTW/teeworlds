/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_LINE_H
#define GAME_SERVER_ENTITIES_LINE_H

#include <game/server/entity.h>

class CLine : public CEntity
{
public:
	CLine(CGameWorld *pGameWorld, vec2 StartPos, vec2 EndPos, int Owner);

	virtual void Reset();
	virtual void Tick();
	virtual void TickPaused();
	virtual void Snap(int SnappingClient);

private:
	vec2 m_EndPos;
	int m_StartTick;
	int m_Owner;
	int m_LifeSpan;
};

#endif
