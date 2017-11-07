#include "glow.h"

#include "esp.h"
#include "settings.h"
#include "globals.h"

std::vector<std::pair<int, int>> custom_glow_entities;

void Glow::FrameStageNotify()
{
	for (int i = 0; i < I::GlowManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &I::GlowManager->m_GlowObjectDefinitions[i];
		C_BaseEntity* Entity = glowEntity->getEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID)
		{
		case EClassIds::CC4:
		case EClassIds::CPlantedC4:
			glowEntity->set(Colors::ESP::ESPBOMBTEXT);
			break;

		case EClassIds::CCSPlayer:
			glowEntity->set(Entity->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST ? Colors::ESP::CTCHAMS : Colors::ESP::TCHAMS);
			break;

		case EClassIds::CMolotovGrenade:
		case EClassIds::CIncendiaryGrenade:
		case EClassIds::CHEGrenade:
		case EClassIds::CFlashbang:
		case EClassIds::CSmokeGrenade:
		case EClassIds::CDecoyGrenade:
			glowEntity->set(ESP::GetNadeInfo(Entity).color);

			break;

		case EClassIds::CAK47:
		case EClassIds::CDEagle:
			glowEntity->set(Colors::ESP::DROPS);
			break;

		default:
			if (strstr(Entity->GetClientClass()->m_pNetworkName, "CWeapon"))
				glowEntity->set(Colors::ESP::DROPS);
		break;
		}
	}
}


//{
//	if (!I::Engine->IsInGame())
//		return;
//
//	for (int i = 0; i < I::EntList->GetHighestEntityIndex(); ++i)
//	{
//		C_BaseEntity* entity = I::EntList->GetClientEntity(i);
//
//		if (!entity) continue;
//
//		ClientClass* cclass = entity->GetClientClass();
//
//		if (!cclass) return;
//
//		if (cclass->m_ClassID == EClassIds::CCSPlayer || strstr(cclass->m_pNetworkName, "CWeapon") || strstr(cclass->m_pNetworkName, "dropped"))
//		{
//			if (!I::GlowManager->HasGlowEffect(entity))
//				I::GlowManager->RegisterGlowObject(entity);
//		}
//	}
//
//	for (int i = 0; i < I::GlowManager->m_nFirstFreeSlot; ++i)
//	{
//		GlowObjectDefinition_t* glowObject = &I::GlowManager->m_GlowObjectDefinitions[i];
//
//		C_BaseEntity* entity = glowObject->getEnt();
//
//		if (!entity)
//			continue;
//
//		ClientClass* cclass = entity->GetClientClass();
//
//		if (!cclass)
//			continue;
//
//		if (cclass->m_ClassID == EClassIds::CCSPlayer)
//			glowObject->set(entity->GetTeam() == pLocal->GetTeam() ? Colors::ESP::WEAPONCHAMS : Colors::ESP::ARMSCHAMS);
//		else if (cclass->m_ClassID == EClassIds::CC4 || cclass->m_ClassID == EClassIds::CPlantedC4)
//			glowObject->set(Color(255, 0, 0, 255));
//		else if (strstr(cclass->m_pNetworkName, "CWeapon") || strstr(cclass->m_pNetworkName, "dropped"))
//			glowObject->set(Color(255, 15, 255));
//	}
//}
