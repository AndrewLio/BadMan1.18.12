#include "Watermark.h"
#include "../pch.h"
#include <chrono>
using namespace std;
Watermark::Watermark() : IModule(0, Category::VISUAL, "Displays the watermark") {
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	shouldHide = true;
}

const char* Watermark::getModuleName() {
	return ("Watermark");
}

void Watermark::onEnable() {
}

void Watermark::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr || moduleMgr->getModule<DebugMenu>()->isEnabled()) return;

	auto interfaceMod = moduleMgr->getModule<Interface>();
	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();

	//DrawUtils::drawImage("textures/ui/heart_new.png", vec2_t_t(positionX + 5, positionY + 50), vec2_t_t(30, 30), vec2_t_t(0, 0), vec2_t_t(1, 1));

	if (g_Data.canUseMoveKeys() && !clickGUI->hasOpenedGUI) {

		constexpr float nameTextSize = 1.5f;
		constexpr float versionTextSize = 0.7f;
		static const float textHeight = (nameTextSize + versionTextSize * 0.7f /* We don't quite want the version string in its own line, just a bit below the name */) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
		constexpr float borderPadding = 1;
		constexpr float margin = 5;

		static std::string name = "BadMan";
#ifdef _DEBUG
		static std::string version = "dev";
#elif defined _BETA
		static std::string version = "beta";
#else
		static std::string version = "public";
#endif

		float nameLength = DrawUtils::getTextWidth(&name, nameTextSize);
		float fullTextLength = nameLength + DrawUtils::getTextWidth(&version, versionTextSize);
		vec4_t rect = vec4_t(
			windowSize.x - margin - fullTextLength - borderPadding * 2,
			windowSize.y - margin - textHeight,
			windowSize.x - margin + borderPadding,
			windowSize.y - margin);

		DrawUtils::drawRectangle(rect, MC_Color(255, 0, 0, 200), 1.f, 1.f);
		DrawUtils::fillRectangle(rect, MC_Color(30, 30, 30), 0.1f);
		DrawUtils::drawText(vec2_t(rect.x + borderPadding, rect.y), &name, MC_Color(255, 0, 0, 200), nameTextSize);
		DrawUtils::drawText(vec2_t(rect.x + borderPadding + nameLength, rect.w - 7), &version, MC_Color(255, 0, 0, 200), versionTextSize);
	}
}
