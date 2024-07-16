#include "Timer1.h"
#include "../pch.h"

Timer1::Timer1() : IModule(0, Category::PLAYER, "Modifies the games speed") {
	registerIntSetting("TPS", &timer2, timer2, 1, 500);
}

const char* Timer1::getModuleName() {
	return ("Timer2");
}

void Timer1::onTick(C_GameMode* gm) {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(timer2);
}

void Timer1::onDisable() {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
}