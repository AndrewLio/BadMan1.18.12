#pragma once
#include <ctime>

#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class AutoEZ : public IModule {
private:
	bool sayUserDied = true;
	bool sayEnemDied = true;
public:
	bool foundPlayer = false;
	bool isDead = false;
	bool antiSpamCheck = true;

	AutoEZ();
	~AutoEZ();

	// Inherited via IModule
	virtual const char* getRawModuleName() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	std::string deathMsg = "gg";
};