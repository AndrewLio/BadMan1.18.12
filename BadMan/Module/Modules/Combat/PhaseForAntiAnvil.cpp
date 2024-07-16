#include "PhaseForAntiAnvil.h"

PhaseForAntiAnvil::PhaseForAntiAnvil() : IModule(0x0, Category::CUSTOM, u8"´©Ç½") {
	shouldHide = true;
}

PhaseForAntiAnvil::~PhaseForAntiAnvil() {
}

const char* PhaseForAntiAnvil::getModuleName() {
	return ("AutoPhase");
}

void PhaseForAntiAnvil::onTick(C_GameMode* gm) {
	gm->player->aabb.upper.y = gm->player->aabb.lower.y;
}

void PhaseForAntiAnvil::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->aabb.upper.y += 1.8f;
}
