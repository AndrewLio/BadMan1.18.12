#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"
class FastEat : public IModule {
private:
	bool TimerWasEnabled = false;
	bool _2espeedWasEnabled = false;
public:

	FastEat();
	~FastEat();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual const char* getRawModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
