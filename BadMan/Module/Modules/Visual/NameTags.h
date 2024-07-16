#pragma once

#include "../../../../Utils/TargetUtil.h"
#include "../../../../SDK/CCamera.h"
#include "../../ModuleManager.h"
#include "../../../../Utils/DrawUtils.h"
#include "../Module.h"
#include <set>

class NameTags : public IModule {
public:
	std::set<std::string> nameTags;
	bool displayArmor = true;
	NameTags();
	~NameTags();
	bool underline = true;
	bool inview = true;
	float opacity = 1.f;
	bool ingameNametagSetting = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};
