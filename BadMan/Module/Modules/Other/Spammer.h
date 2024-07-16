#pragma once
#include "../../../../Utils/Utils.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class Spammer : public IModule {
private:
	std::string message = "BadMan!!!!!!!!!!!!!!!!!!";
	bool bypass = true;
	int length = 8;
	int Odelay = 0;
	int delay = 2;

public:
	inline std::string& getMessage() { return message; };
	inline int& getDelay() { return delay; };
	inline bool& getBypass() { return bypass; };

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	Spammer();
};