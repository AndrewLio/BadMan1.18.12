#include "Crystalbreak.h"

Crystalbreak::Crystalbreak() : IModule(0x0, Category::COMBAT, u8"ÆÆ»µÖÜÎ§µÄË®¾§") {
	registerIntSetting("range", &eRange, eRange, 1, 6);
	registerBoolSetting("notarget", &notarget, notarget);
}
Crystalbreak::~Crystalbreak() {
}
const char* Crystalbreak::getModuleName() {
	return ("CrystalAuraX");
}

static std::vector<C_Entity*> targetList20;

void findEntity20(C_Entity* currentEntity20, bool isRegularEntity) {
	static auto CrystalbreakMod = moduleMgr->getModule<Crystalbreak>();

	if (currentEntity20 == nullptr)
		return;

	if (currentEntity20 == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity20, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity20->isAlive())
		return;


	if (!TargetUtil::isValidTarget(currentEntity20))
		return;

	float dist = (*currentEntity20->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < CrystalbreakMod->eRange) {
		targetList20.push_back(currentEntity20);

		float dist = (*currentEntity20->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < CrystalbreakMod->eRange) {
			targetList20.push_back(currentEntity20);
		}
	}
}

void Crystalbreak::onTick(C_GameMode* gm) {
	if (notarget) {
		g_Data.forEachEntity([](C_Entity* ent, bool b) {
			int id = ent->getEntityTypeId();
			int range = moduleMgr->getModule<Crystalbreak>()->eRange;
			if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= range) {
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				g_Data.getCGameMode()->attack(ent);
				if (!moduleMgr->getModule<NoSwing>()->isEnabled()) {
					g_Data.getLocalPlayer()->swingArm();
				}
			}
			});
	}
	if (!notarget) {
		targetList20.clear();
		g_Data.forEachEntity(findEntity20);
		g_Data.forEachEntity([](C_Entity* ent, bool b) {
			int id = ent->getEntityTypeId();
			int range = moduleMgr->getModule<Crystalbreak>()->eRange;
			if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= range) {
				if (!targetList20.empty()) {
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					g_Data.getCGameMode()->attack(ent);
					if (!moduleMgr->getModule<NoSwing>()->isEnabled()) {
						g_Data.getLocalPlayer()->swingArm();
					}
				}
			}
			});
	}
}