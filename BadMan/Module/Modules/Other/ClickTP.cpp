#include "ClickTP.h"
#include "../pch.h"
int x;
int y;
int z;
ClickTP::ClickTP() : IModule(0, Category::OTHER, "Teleports to where you are looking") {
}

const char* ClickTP::getModuleName() {
	return "Teleport";
}

void ClickTP::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	vec3_t pos;
	pos.z + 1;

	g_Data.getLocalPlayer()->setPos(pos);
}

void ClickTP::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}