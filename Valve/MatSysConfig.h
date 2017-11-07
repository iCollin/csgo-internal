#pragma once

#include "settings.h"

#include "IMaterialSystem.h"

bool Config::Misc::RenderConVars::mat_drawgray = false;
bool Config::Misc::RenderConVars::mat_fullbright = false;
bool Config::Misc::RenderConVars::mat_showlowresimage = false;
bool Config::Misc::RenderConVars::mat_showmiplevels = false;

namespace MatSysConfig
{
	struct MaterialVideoMode_t {
		int m_Width;
		int m_Height;
		int m_Format;
		int m_RefreshRate;
	};

	struct MaterialSystem_Config_t {
		MaterialVideoMode_t m_VideoMode;
		float m_fMonitorGamma;
		float m_fGammaTVRangeMin;
		float m_fGammaTVRangeMax;
		float m_fGammaTVExponent;
		bool m_bGammaTVEnabled;
		bool m_bTripleBuffered;
		int m_nAASamples;
		int m_nForceAnisotropicLevel;
		int m_nSkipMipLevels;
		int m_nDxSupportLevel;
		int m_nFlags;
		bool m_bEditMode;
		char m_nProxiesTestMode;
		bool m_bCompressedTextures;
		bool m_bFilterLightmaps;
		bool m_bFilterTextures;
		bool m_bReverseDepth;
		bool m_bBufferPrimitives;
		bool m_bDrawFlat;
		bool m_bMeasureFillRate;
		bool m_bVisualizeFillRate;
		bool m_bNoTransparency;
		bool m_bSoftwareLighting;
		bool m_bAllowCheats;
		char m_nShowMipLevels;
		bool m_bShowLowResImage;
		bool m_bShowNormalMap;
		bool m_bMipMapTextures;
		char m_nFullbright;
		bool m_bFastNoBump;
		bool m_bSuppressRendering;
		bool m_bDrawGray;
		bool m_bShowSpecular;
		bool m_bShowDiffuse;
		int m_nWindowedSizeLimitWidth;
		int m_nWindowedSizeLimitHeight;
		int m_nAAQuality;
		bool m_bShadowDepthTexture;
		bool m_bMotionBlur;
		bool m_bSupportFlashlight;
		bool m_bPaintEnabled;
		char pad[0xC];
	};

	void Hook(MaterialSystem_Config_t* config)
	{
		config->m_bDrawGray = Config::Misc::RenderConVars::mat_drawgray;
		//config->m_nFullbright = Config::Misc::RenderConVars::mat_fullbright;
		config->m_bShowLowResImage = Config::Misc::RenderConVars::mat_showlowresimage;
		//config->m_nShowMipLevels = Config::Misc::RenderConVars::mat_showmiplevels;
	}
}