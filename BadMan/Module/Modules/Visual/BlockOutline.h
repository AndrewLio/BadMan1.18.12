#pragma once
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class BlockOutline : public IModule {
private:
public:
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	virtual const char* getModuleName();
	BlockOutline();
};