#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class AutoCrystal : public IModule {
public:
	bool obsidianAC = false;
	bool autoplace = true;
	bool autobreak = true;
	bool crystalCheck = true;
	bool calcpos = true;
	bool calcdmg = true;
	bool yLock = false;
	bool isClick = false;
	bool doMultiple = true;
	int rad = 2;
	int sthealth = 5;
	bool silentAC = false;
	int slotAC = 0;
	bool renderAC = true;
	float mindmg = 6.f;
	float maxdmg = 12.f;
	void findObAC();

	AutoCrystal();
	~AutoCrystal();
	SettingEnum ServerMode = this;
	SettingEnum Calculation = this;
	SettingEnum EnemyType = this;
	std::string targetnameac = "None";
	std::string mode;
	int Odelay;
	bool idpredict = true;
	int placedelay = 1;
	int breakdelay = 0;
	int placerange = 7;
	int breakrange = 7;
	int placepacket = 6;
	int breakpacket = 6;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual const char* getRawModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	void atack(C_Entity* ent);
	void buildBlock(vec3_t pos);
	int range = 7;
	//bool BadMan = true;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	vec3_t espPosUpperAC;
	vec3_t espPosLowerAC;
};
