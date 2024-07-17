#include "Welcome.h"
#include "../../../../Utils/DrawUtils.h"
#include "../../ModuleManager.h"
#include <iomanip>
using namespace std;

Welcome::Welcome() : IModule(0x0, Category::VISUAL, u8"欢迎显示") {
	TextMode.addEntry("Welcome", 1);
	TextMode.addEntry("BadMan", 2);
	TextMode.addEntry("Xin Chao", 3);
	TextMode.addEntry("Ditmemay", 4);
	registerEnumSetting("Text Modes", &TextMode, 1);
	
	registerFloatSetting("Scale", &this->scale, this->scale, 0.5f, 1.5f);
	registerIntSetting("Opacity", &opati, opati, 0, 1000);
	registerBoolSetting("Info", &outline, outline);
}

Welcome::~Welcome() {
}

const char* Welcome::getModuleName() {
	return ("Welcome");
}
void Welcome::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	std::stringstream ss;
	// 可以分别以不同的形式进行显示
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	ss << std::put_time(std::localtime(&t), "%Y-%m-%d- %H:%M:%S");
	//ss << std::put_time(std::localtime(&t), u8"%Y年%m月%d日%H时%M分%S秒");
	//	ss << std::put_time(std::localtime(&t), "%Y%m%d%H%M%S");

	std::string str_time = ss.str();


	static float rcolors[4];

	if (g_Data.isInGame() && GameData::canUseMoveKeys()) {
		float nameTextSize = scale + 0.5f;
		static const float textHeight = (nameTextSize * 0.7f) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
		constexpr float borderPadding = 1;
		constexpr float margin = 5;
		std::string playerName = std::string(g_Data.getLocalPlayer()->getNameTag()->getText());

		switch (TextMode.getSelectedValue()) {
		case 1:
			name = u8"你好！ " + playerName + "   " + str_time;;
			break;

		case 2:
			name = u8"你好，操你妈！ " + playerName + "   " + str_time;
			break;

		case 3:
			name = u8"你好！ " + playerName + "   " + str_time;;
			break;
		case 4:
			name = u8"你好！" + playerName + "   " + str_time;;
			break;
		}
		auto interfaceColor = ColorUtil::interfaceColor(1);
		float nameLength = DrawUtils::getTextWidth(&name, scale);
		float fullTextLength = nameLength;
		float x = windowSize.x - windowSize.x / 2 - nameLength / 2;
		float y = windowSize.y - windowSize.y;
		vec4_t rect = vec4_t(
			windowSize.x - margin - fullTextLength - borderPadding * 415,
			windowSize.y - margin - textHeight,
			windowSize.x - margin + borderPadding,
			windowSize.y - margin);
		//DrawUtils::drawText(vec2_t(x, y + 2), &name, MC_Color(interfaceColor), nameTextSize, 1.f, true);
		float texthai = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * scale;
		DrawUtils::drawCenteredString(vec2_t(windowSize.x / 2, windowSize.y - windowSize.y + 7.5), &name, scale, MC_Color(interfaceColor), true);
			

		//draw Welcome2

		if (outline) {
			string welcome2 = "Welcome to BadMan！";
			if (!isFirstInjected && !StartMove) {

				DrawUtils::fillRectangle(vec4_t(0, 0, MoveX, MoveY), MC_Color(0, 0, 0), opatiF);
				DrawUtils::drawCenteredString(vec2_t(stringPosX, stringPosY), &welcome2, 5.f, MC_Color(255, 0, 0), true);
				if (opati != -50)
					opati--;
				if (opati == -50) {
					opatiF -= 0.0005;
					MoveY -= 0.75f;
					opati = -50;
				}
			}
			//clientMessageF("Op = %f",opatiF);
			//clientMessageF("Y = %f", MoveY);
			//clientMessageF("OPI = %d", opati);
			if (opatiF <= 0 || MoveY <= 0) {
				isFirstInjected = true;
				StartMove = true;
				opatiF == 0;
				MoveY == 0;
			}
		}
	}
}
