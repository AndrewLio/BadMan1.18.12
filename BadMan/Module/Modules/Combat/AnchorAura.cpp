#include "AnchorAura.h"

#include "../../SDK/Tag.h"

AnchorAura::AnchorAura() : IModule(VK_NUMPAD0, Category::COMBAT, u8"重生锚炸头") {
	this->registerIntSetting("Range", &this->range, this->range, 3, 10);
	this->registerIntSetting("AnchorDelay", &this->anchordelay, this->anchordelay, 0, 10);
	this->registerIntSetting("GsDelay", &this->gsdelay, this->gsdelay, 0, 10);
	this->registerIntSetting("BreakDelay", &this->breakdelay, this->breakdelay, 0, 10);
	this->registerBoolSetting("Pause On GApple", &this->appleA, this->appleA);
	this->registerBoolSetting("Silent", &this->spoof, this->spoof);
	this->registerBoolSetting("Return to GApple", &this->BackForWhat, this->BackForWhat);
}
AnchorAura::~AnchorAura() {
}
const char* AnchorAura::getModuleName() {
	return ("AnchorAura");
}

static std::vector<C_Entity*> targetList;

void AnchorAura::onEnable() {
	targetList.clear();
}

struct CompDis {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

bool Findentbyc(C_Entity* curEnt, bool isRegularEntity) {
	std::string Enzo = "EveryoneWhySoEZ";
	std::string hell = "AshleyNeo";
	std::string TargetUtilName = curEnt->getNameTag()->getText();
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;                    // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;                    // falling block
	if (curEnt->getEntityTypeId() == 64) return false;                    // item
	if (curEnt->getEntityTypeId() == 69) return false;                    // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (!TargetUtil::isValidTarget(curEnt)) return false;

	float dist = (*curEnt->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist <= moduleMgr->getModule<AnchorAura>()->range) {
		targetList.push_back(curEnt);
		sort(targetList.begin(), targetList.end(), CompDis());
		return true;
	}
	return false;
}



void getAnchor2() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == -272)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == -272) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

void getGS2() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 89)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 89) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}


void SpoofAnchor() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == -272) {  // select crystal
				supplies->selectedHotbarSlot = n;
				//return true;
				return;
			}
		}
	}
	//return false;
}

void SpoofGs() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 89) {  // select crystal
				supplies->selectedHotbarSlot = n;
				//return true;
				return;
			}
		}
	}
	//return false;
}
bool AnchorAura::Tryplace(vec3_t PlaceAnchor) {
	PlaceAnchor = PlaceAnchor.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(PlaceAnchor));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(PlaceAnchor);

		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i);

			return true;
		}
	}
	return false;
}
bool TryPlace2(vec3_t blkPlacement) {//好像是放锚子
	blkPlacement = blkPlacement.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blkPlacement);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i);
			return true;
		}
	}
	return false;
}
void AnchorAura::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 259 && g_Data.isRightClickDown() && appleA)
		return;
	targetList.clear();
	g_Data.forEachEntity(Findentbyc);
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;
	SlotSpoof = supplies->selectedHotbarSlot;


	if (!targetList.empty()) {
		for (auto& i : targetList) {
			vec3_t enemyLoc = (i->getHumanPos().floor());
			mid1 = enemyLoc.add(0, 2, 0);
			pos = mid1;
			if (!hasPlacedAnchor) {
				// NOT placed anchor
				if (!takenAnchor) {

					if (spoof)
						SpoofAnchor();

					if (!spoof)
						getAnchor2();

					takenAnchor = true;
					return;
				}

				if (g_Data.getLocalPlayer()->region->getBlock(pos)->toLegacy()->blockId == 0) {
					for (auto& i : targetList) {
						//gm->buildBlock(&vec3_ti(pos), 0);
						TryPlace2(pos.toVector3());
					}
				}
				Option = 1;

				if (spoof)
					supplies->selectedHotbarSlot = SlotSpoof;
				hasPlacedAnchor = true;
				// clientMessageF("Placed anchor!");
			}

			if (tickTimer >= anchordelay && !DhasPlacedAnchor) {
				tickTimer = 0;
				DhasPlacedAnchor = true;
			}
			else if (tickTimer < anchordelay && !DhasPlacedAnchor) {
				tickTimer++;
				return;
			}

			if (!hasCharged) {
				if (!takenGS) {
					if (spoof)
						SpoofGs();

					if (!spoof)
						getGS2();

					takenGS = true;
					return;
				}

				switch (Option) {
				case 1:
					bool sb = g_Data.getLocalPlayer()->region->getBlock(pos)->toLegacy()->blockId != 0;

					if (sb) {
						gm->buildBlock(&vec3_ti(pos), 0);
						//Tryplace(pos.toVector3());
					}
					break;
				}
				if (spoof)
					supplies->selectedHotbarSlot = SlotSpoof;

				hasCharged = true;
				takenAnchor = false;
				// clientMessageF("Charging!");
			}

			if (tickTimer >= gsdelay && !DhasCharged) {
				tickTimer = 0;
				DhasCharged = true;
			}
			else if (tickTimer < gsdelay && !DhasCharged) {
				tickTimer++;
				return;
			}

			if (!takenAnchor) {
				if (spoof)
					SpoofAnchor();

				if (!spoof)
					getAnchor2();

				takenAnchor = true;
				return;
			}

			if (!hasDetonated) {

				switch (Option) {
				case 1:
					//Tryplace(pos.toVector3());
					gm->buildBlock(&vec3_ti(pos), 0);
					break;

					if (spoof)
						supplies->selectedHotbarSlot = SlotSpoof;
					hasDetonated = true;
				}
				// clientMessageF("Detonated!");

				if (tickTimer >= breakdelay) {
					hasPlacedAnchor = false;
					hasCharged = false;
					hasDetonated = false;
					DhasPlacedAnchor = false;
					DhasCharged = false;
					tickTimer = 0;
					takenAnchor = false;
					takenGS = false;
					// stopSp();
					return;
				}
				else {
					tickTimer++;
					return;
				}
			}
		}
	}
}
void AnchorAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	{
		odelays--;
		float rentimer = 1;
		if (odelays != 0 && odelays <= 100) rentimer = (float)(odelays / 100.f);
		if (odelays == 0) {
			rentimer = 0;
			odelays = 100;
		}

		float zero = rentimer / 2;
		vec3_t block(pos.toVector3().add(0.5));
		DrawUtils::setColor(3.f, 0.f, 0.f, 0.3f);
		{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, -zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, -zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, -zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, -zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(zero, zero, -zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(zero, -zero, -zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
		{
			vec2_t fill1 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, -zero));
			vec2_t fill2 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, zero, zero));
			vec2_t fill3 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, zero));
			vec2_t fill4 = DrawUtils::worldToScreen(vec3_t(block).add(-zero, -zero, -zero));
			DrawUtils::drawQuad(vec2_t(fill3), vec2_t(fill4), vec2_t(fill1), vec2_t(fill2));
		}
	}
}

void AnchorAura::onDisable() {
	if(BackForWhat){
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 259)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 259) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		  } 
		}
	}
}
/*if (!hasPlacedAnchor) {
			// NOT placed anchor
			if (!takenAnchor) {
					getAnchor2();
				takenAnchor = true;
				return;
			}

			if (g_Data.getLocalPlayer()->region->getBlock(pos)->toLegacy()->blockId == 0 || g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId == 0) {
				for (auto& i : targetList)

						gm->buildBlock(&vec3_ti(pos), 0, 3);
					}
			Option = 1;
			}
			// stopSp();
			hasPlacedAnchor = true;
			// clientMessageF("Placed anchor!");
		}

		if (tickTimer >= anchordelay && !DhasPlacedAnchor) {
			tickTimer = 0;
			DhasPlacedAnchor = true;
		} else if (tickTimer < anchordelay && !DhasPlacedAnchor) {
			tickTimer++;
			return;
		}

		if (!hasCharged) {
			if (!takenGS) {
					getGS2();
				takenGS = true;
				return;
			}

			switch (Option) {
			case 1:

				bool sb = g_Data.getLocalPlayer()->region->getBlock(pos)->toLegacy()->blockId != 0;
				bool sb2 = g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId != 0;
				if (sb || sb2) {
						gm->buildBlock(&vec3_ti(pos), 0, 3);
					}

				break;
			}


			hasCharged = true;
			takenAnchor = false;
			// clientMessageF("Charging!");
		}

		if (tickTimer >= gsdelay && !DhasCharged) {
			tickTimer = 0;
			DhasCharged = true;
		} else if (tickTimer < gsdelay && !DhasCharged) {
			tickTimer++;
			return;
		}

		if (!takenAnchor) {
				getAnchor2();
			// getAnchor2();
			takenAnchor = true;
			return;
		}

		if (!hasDetonated) {
			switch (Option) {
			case 1:
				gm->buildBlock(&vec3_ti(pos), 0, 4);
				break;


			hasDetonated = true;
		}
		// clientMessageF("Detonated!");

		if (tickTimer >= breakdelay) {  // reset variables; prepare for another loop
			hasPlacedAnchor = false;
			hasCharged = false;
			hasDetonated = false;
			DhasPlacedAnchor = false;
			DhasCharged = false;
			tickTimer = 0;
			takenAnchor = false;
			takenGS = false;
			// stopSp();
			return;
		} else {
			tickTimer++;
			return;
		}
	}
}
}*/