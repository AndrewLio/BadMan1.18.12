#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class AntiKAFag : public IModule {
private:
	bool obsidian = true;
	bool bedrock = true;
	bool outline = true;

public:
	int range = 5;
	float LineSize = 0.4f;

	AntiKAFag();
	~AntiKAFag();
	// Inherited via IModule
	virtual const char* getRawModuleName() override;
	virtual const char* getModuleName() override;
	//virtual const char* getModName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	//virtual void onTick(C_GameMode* gm) override;
	//virtual const char* getRawModuleName() override;
};