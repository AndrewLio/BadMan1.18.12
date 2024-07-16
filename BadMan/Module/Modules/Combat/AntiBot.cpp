#include "AntiBot.h"
#include "../pch.h"

using namespace std;
AntiBot::AntiBot() : IModule(0, Category::COMBAT, u8"屏蔽反作弊用的假人") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Basic", 0);
	mode.addEntry("Advanced", 1);
	registerBoolSetting("Invisible Check", &invisibleCheck, invisibleCheck);
	registerBoolSetting("EntityID Check", &entityIdCheck, entityIdCheck);
	registerBoolSetting("Hitbox Check", &hitboxCheck, hitboxCheck);
	registerBoolSetting("Other Check", &otherCheck, otherCheck);
	registerBoolSetting("Extra Check", &extraCheck, extraCheck);
}

const char* AntiBot::getRawModuleName() {
	return "AntiBot";
}

const char* AntiBot::getModuleName() {
	name = string("AntiBot ") + string(GRAY) + string("[") + string(WHITE) + string("Advanced") + string(GRAY) + string("]");
	return name.c_str();
}
