#include "NameTags.h"

#include "../../../../Utils/TargetUtil.h"
#include "../pch.h"

bool itemTags = false;

NameTags::NameTags() : IModule(0, Category::VISUAL, "Shows better nametags above players that can be seen from a lot more far aways") {
	this->registerBoolSetting("Display Armor", &this->displayArmor, this->displayArmor);
	this->registerBoolSetting("Item Tags", &itemTags, itemTags);
	this->registerBoolSetting("Inventory View", &inview, inview);
	this->registerFloatSetting("opacity", &opacity, opacity, 0, 1);
}

NameTags::~NameTags() {
}

const char* NameTags::getModuleName() {
	return ("NameTags");
}

void drawNametags(C_Entity* ent, bool isRegularEntitie) {
	C_LocalPlayer* C_localPlayer = g_Data.getLocalPlayer();
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();

	if (nameTagsMod == nullptr)
		return;

	if (ent != C_localPlayer)
	{
		if (ent->isPlayer() && nameTagsMod != nullptr) {
			if (ent->timeSinceDeath > 0)
				return;

			if (ent->getNameTag()->getTextLength() < 1)
				return;

			nameTagsMod->nameTags.insert(Utils::sanitize(ent->getNameTag()->getText()));
			float dist = ent->getPos()->dist(g_Data.getClientInstance()->levelRenderer->getOrigin());
			DrawUtils::drawNameTags(ent, fmax(0.6f, 3.f / dist), true);
			DrawUtils::flush();
		}

		if (ent->getEntityTypeId() == 64 && itemTags) {
			C_ItemStack* C_stack = (C_ItemStack*)((uintptr_t)ent + 1832);

			std::stringstream textbuild;
			if (C_stack->item != nullptr) {
				if (C_stack->count > 1)
					textbuild << std::to_string(C_stack->count) << "x ";

				bool wasSpace = true;
				std::string name = C_stack->getItem()->name.getText();
				for (auto i = 0; i < name.size(); i++) {
					if (wasSpace) {
						name[i] = toupper(name[i]);
						wasSpace = false;
					}

					if (name[i] == '_') {
						wasSpace = true;
						name[i] = ' ';
					}
				}

				textbuild << name;
			}
			else {
				textbuild << "No item";
			}
			std::string text = textbuild.str();

			vec2_t textPos = DrawUtils::worldToScreen(*ent->getPos());
			float size = fmax(0.6f, 3.f / g_Data.getClientInstance()->levelRenderer->getOrigin().dist(*ent->getPos()));

			float itemHeight = 15.f * size;
			float textHeight = DrawUtils::getFontHeight(size);
			float itemSize = size;

			if (itemHeight > textHeight) {
				itemSize *= textHeight / itemHeight;
				itemHeight = textHeight;
			}

			float textWidth = DrawUtils::getTextWidth(&text, size) + (15.f * itemSize + 3.f * size);

			textPos.y -= textHeight * 1.5f;
			textPos.x -= textWidth / 2.f;

			vec4_t rectPos;
			rectPos.x = textPos.x - 1.f * size;
			rectPos.y = textPos.y - 1.f * size;
			rectPos.z = textPos.x + textWidth + 1.f * size;
			rectPos.w = textPos.y + textHeight + 2.f * size;

			vec2_t itemPos = textPos;
			textPos.x += 15.f * itemSize + 3.f * size;

			DrawUtils::fillRectangle(rectPos, MC_Color(20, 20, 20), moduleMgr->getModule<NameTags>()->opacity);
			DrawUtils::drawItem(C_stack, itemPos, 1.f, itemSize, false);
			DrawUtils::drawText(textPos, &text, MC_Color(255, 255, 255), size);
			DrawUtils::flush();
		}
	}
}
void NameTags::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* C_localPlayer = g_Data.getLocalPlayer();
	if (C_localPlayer == nullptr || !GameData::canUseMoveKeys()) return;

	g_Data.forEachEntity(drawNametags);
}

//rr