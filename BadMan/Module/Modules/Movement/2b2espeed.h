#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class NukeSpeed : public IModule {
private:
	float speed = 0.325f;
	bool ssp = false;
	float sneakspeed = 1.f;
	float speedreal = 0.5f;
public:
	NukeSpeed();
	~NukeSpeed();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};
