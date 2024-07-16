#include "ShieldBreaker.h"

ShieldBreak::ShieldBreak() : IModule(0x0, Category::COMBAT, u8"¿ìËÙÆÆ»µ¶ÜÅÆ") {
	this->registerBoolSetting("MultiAura", &this->isMulti, this->isMulti);
	this->registerFloatSetting("range", &this->range, this->range, 2.f, 20.f);
}

ShieldBreak::~ShieldBreak() {
}

const char* ShieldBreak::getModuleName() {
	return ("ShieldBreak");
}

static std::vector<C_Entity*> targetListS;

void findEntityS(C_Entity* currentEntityS, bool isRegularEntityS) {
	static auto killauraMod = moduleMgr->getModule<ShieldBreak>();

	if (currentEntityS == nullptr)
		return;

	if (currentEntityS == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntityS, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntityS->isAlive())
		return;

	if (killauraMod->isMobAura) {
		if (currentEntityS->getNameTag()->getTextLength() <= 1 && currentEntityS->getEntityTypeId() == 63)
			return;
		if (currentEntityS->width <= 0.01f || currentEntityS->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntityS->getEntityTypeId() == 64)  // item
			return;
		if (currentEntityS->getEntityTypeId() == 69)  // xp
			return;
	}
	else {
		if (!TargetUtil::isValidTarget(currentEntityS))
			return;
	}

	float dist = (*currentEntityS->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < killauraMod->range) {
		targetListS.push_back(currentEntityS);
	}
}

void ShieldBreak::onTick(C_GameMode* gm) {
	using getOffhandSlot_t = C_ItemStack * (__fastcall*)(C_Entity*);
	static getOffhandSlot_t getOffhandSlot = (getOffhandSlot_t)FindSignature("48 8B 89 ?? ?? ?? ?? BA 01 00 00 00 48 8B 01 48 FF 60 ??");

	//Loop through all our players and retrieve their information
	targetListS.clear();

	g_Data.forEachEntity(findEntityS);

	Odelay++;
	if (!targetListS.empty() && Odelay >= delay) {
		// Attack all entitys in targetList
		if (isMulti) {
			for (auto& i : targetListS) {
				C_ItemStack* current = getOffhandSlot(i);

				bool run = false;

				if (current->item == NULL && !i->isSneaking())
					run = false;

				else if ((*current->item)->itemId != (355) && i->isSneaking())
					run = true;

				if (!(i->damageTime > 1 && hurttime)) {
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
					g_Data.getCGameMode()->attack(i);
				}
			}
		}
		else {
			C_ItemStack* current = getOffhandSlot(targetListS[0]);

			bool run = false;

			if (current->item == NULL && !targetListS[0]->isSneaking())
				run = false;

			else if ((*current->item)->itemId != (355) && targetListS[0]->isSneaking())
				run = true;

			if (!(targetListS[0]->damageTime > 1 && hurttime)) {
				g_Data.getLocalPlayer()->swing();
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
				g_Data.getCGameMode()->attack(targetListS[0]);
			}
		}
	}
}