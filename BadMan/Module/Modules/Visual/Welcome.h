#pragma once
#include "../Module.h"
class Welcome : public IModule {
public:
	Welcome();
	~Welcome();
	float opatiF = 1.f;
	float scale = 1.f;
	int opati = 250;
	bool outline = true;
	std::string name = "";
	bool isFirstInjected = false;
	bool StartMove = false;
	SettingEnum TextMode = this;
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	float MoveY = windowSize.y;
	float MoveX = windowSize.x;
	float stringPosX = windowSize.x - windowSize.x / 2;
	float stringPosY = windowSize.y - windowSize.y / 2;
	// Inherited via IModule
	virtual const char* getModuleName();
	//virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx);
};
