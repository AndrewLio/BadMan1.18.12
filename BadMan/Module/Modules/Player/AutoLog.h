#pragma once
#pragma once
#include "../Module.h"
class AutoLog : public IModule {
public:
	bool CustomMsg = true;
	std::string custommessage;
	int health = 6;
	SettingEnum ByeByeMessage;
	bool message = true;
	bool OnAnvil = false;
	float height = 1.0f;
	bool caidelljthe = false;
	vec3_ti blockpos3 = { 0, 0, 0 };
	int slottd2 = 0;
	bool silenttd2 = false;
	AutoLog();
	~AutoLog();
	std::string name = "AutoLog";
	bool isFallingAnvilAboveinPlayer(C_Player* plr);
	inline std::string& getCustomMessage() { return custommessage; };
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};