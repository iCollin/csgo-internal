/*
      ___           ___           ___           ___     
     /\  \         |\__\         /\  \         /\__\    
    /::\  \        |:|  |       /::\  \       /::|  |   
   /:/\:\  \       |:|  |      /:/\:\  \     /:|:|  |   
  /:/  \:\  \      |:|__|__   /::\~\:\  \   /:/|:|  |__ 
 /:/__/ \:\__\     /::::\__\ /:/\:\ \:\__\ /:/ |:| /\__\
 \:\  \  \/__/    /:/~~/~~   \/__\:\/:/  / \/__|:|/:/  /
  \:\  \         /:/  /           \::/  /      |:/:/  / 
   \:\  \        \/__/            /:/  /       |::/  /  
    \:\__\                       /:/  /        /:/  /   
     \/__/                       \/__/         \/__/    

revolt (4/2017)
credit: AimTux (https://github.com/AimTuxOfficial/AimTux)
*/

#pragma once

class CGlowObjectManager
{
public:
	class GlowObjectDefinition_t
	{
	public:
		void set(Color color)
		{
			m_vGlowColor = Vector(color.fr(), color.fg(), color.fb());
			m_flGlowAlpha = color.fa();
			m_bRenderWhenOccluded = true;
			m_bRenderWhenUnoccluded = false;
			m_flBloomAmount = 1.f;
		}

		C_BaseEntity* getEntity()
		{
			return m_hEntity;
		}

		bool IsEmpty() const
		{
			return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE;
		}

	public:
		C_BaseEntity* m_hEntity;
		Vector m_vGlowColor;
		float m_flGlowAlpha;

		char unknown[4];
		float flUnk;
		float m_flBloomAmount;
		float localplayeriszeropoint3;

		bool m_bRenderWhenOccluded;
		bool m_bRenderWhenUnoccluded;
		bool m_bFullBloomRender;
		char unknown1[1];

		int m_nFullBloomStencilTestValue;
		int iUnk;
		int m_nSplitScreenSlot;
		int m_nNextFreeSlot;

		// Special values for GlowObjectDefinition_t::m_nNextFreeSlot
		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};

	GlowObjectDefinition_t* m_GlowObjectDefinitions;
	int max_size;
	int pad;
	int size;
	GlowObjectDefinition_t* m_GlowObjectDefinitions2;
	int currentObjects;
};