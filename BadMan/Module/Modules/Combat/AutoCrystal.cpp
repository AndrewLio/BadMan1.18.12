#include "AutoCrystal.h"
using namespace std;
string targetnameac = "None";
string mode;
int Odelay;
AutoCrystal::AutoCrystal() : IModule(0, Category::COMBAT, u8"×Ô¶¯Ë®¾§") {
	{ // Enum
		{ // ServerMode
			{ // AddEntry 
				ServerMode.addEntry(EnumEntry("Nukkit+", 0));
				ServerMode.addEntry(EnumEntry("Nukkit", 1));
				ServerMode.addEntry(EnumEntry("World", 2));
			};
			{ // registerEnumSettings
				registerEnumSetting("Server", &ServerMode, 0);
			};
		};
		{ // Calculation
			{ //AddEntry
				Calculation.addEntry(EnumEntry("Dis", 0));
				Calculation.addEntry(EnumEntry("Dmg", 1));
			};
			{ // registerEnumSettings
				registerEnumSetting("Calculation", &Calculation, 0);
			};
		};
		{ // EnemyType
			{ // AddEntry
				EnemyType.addEntry(EnumEntry("Player", 0));
				EnemyType.addEntry(EnumEntry("Mob", 1));
			};
			{ // registerEnumSettings
				registerEnumSetting("EnemyType", &EnemyType, 0);
			};
		}
	};
	{ // RegisterIntSettings
		{ // Stop Health
			registerIntSetting("Stop Health", &sthealth, sthealth, 1, 20);
		};
		{ // Range
			registerIntSetting("Multi Radius", &rad, rad, 1, 10);
			registerIntSetting("Place Range", &placerange, placerange, 0.f, 12.f);
			registerIntSetting("Break Range", &breakrange, breakrange, 0.f, 12.f);
		};
		{ // Place | Break
			{ // Place | Delay/Packet
				registerIntSetting("PlaceDelay", &placedelay, placedelay, 0, 10);
				registerIntSetting("PlacePacket", &placepacket, placepacket, 1, 6);
			};
			{ // AutoPlace/Bool
				registerBoolSetting("AutoPlace", &autoplace, autoplace);
			};
			{ // Break | Delay/Packet
				registerIntSetting("BreakDelay", &breakdelay, breakdelay, 0, 10);
				registerIntSetting("BreakPacket", &breakpacket, breakpacket, 1, 6);
			};
			{ // AutoBreak/Bool
				registerBoolSetting("AutoBreak", &autobreak, autobreak);
			};
		};
	};
	{ // RegisterFloatSettings 
		{ // Damage
			registerFloatSetting("Min Damage", &mindmg, mindmg, 0.f, 20.f);
			registerFloatSetting("Max Damage", &maxdmg, maxdmg, 0.f, 20.f);
		};
	};
	{ // RegisterBoolSettings 
		{
			registerBoolSetting("idpredict", &idpredict, idpredict);
			registerBoolSetting("onClick", &isClick, isClick);
			registerBoolSetting("Multi", &doMultiple, doMultiple);
			registerBoolSetting("Silent", &silentAC, silentAC);
			registerBoolSetting("Render", &renderAC, renderAC);
		};
	};
}
int crystalDelayac = 0;
int crystalDelayac2 = 0;
int crystalDelayac3 = 0;
int crystalDelayacbreak = 0;
int crystalDelayac2break = 0;
int crystalDelayac3break = 0;

AutoCrystal::~AutoCrystal() {
}
const char* AutoCrystal::getModuleName() {
	switch (ServerMode.getSelectedValue()) {
	case 0:
		mode = "Nukkit ";
		break;
	case 1:
		mode = "Nukkit ";
		break;
	case 2:
		mode = "Java";
		break;
	};
	string name = string("AutoCrystal " + string(GRAY) + mode + to_string(sthealth) + " " + targetnameac);
	return name.c_str();
}
const char* AutoCrystal::getRawModuleName() {
	return "AutoCrystal";
}
static std::vector<C_Entity*> targetListAC;
struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};
void getEntityAC(C_Entity* currentEntity, bool isRegularEntity) {
	static auto autocrystal = moduleMgr->getModule<AutoCrystal>();

	if (currentEntity == nullptr) return;
	if (currentEntity == g_Data.getLocalPlayer()) return;
	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
	if (!currentEntity->isAlive()) return;
	if (currentEntity->getEntityTypeId() == 66) // falling block
		return;
	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;
	if (currentEntity->getEntityTypeId() == 64) // item
		return;
	if (currentEntity->getEntityTypeId() == 80 || currentEntity->getEntityTypeId() == 69) return;

	if (autocrystal->EnemyType.getSelectedValue() == 1) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 319) return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) return;
		if (currentEntity->getEntityTypeId() == 0xC00050)  // Arrows - new item id
			return;
		if (currentEntity->getEntityTypeId() == 307) // NPC
			return;
	}
	else if (autocrystal->EnemyType.getSelectedValue() == 0) {
		if (!TargetUtil::isValidTarget(currentEntity)) return;
		if (currentEntity->getEntityTypeId() == 51 || currentEntity->getEntityTypeId() == 1677999) return;
	}
	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < autocrystal->range) {
		targetListAC.push_back(currentEntity);
		sort(targetListAC.begin(), targetListAC.end(), CompareTargetEnArray());
	}
}

bool getGoodPlacementsAC(vec3_t* block, C_Entity* ent) {
	std::vector<vec3_t*> corners;
	corners.clear();

	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.lower.z));
	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.lower.z));
	int n = 0;
	if (!corners.empty())
		for (auto corner : corners) {
			//	DrawUtils::drawText(DrawUtils::worldToScreen(*corners[n]), &std::to_string(n + 1), MC_Color(1.f, 1.f, 1.f));
			//DrawUtils::setColor(1.f, 0.f, 0.f, 1.f);
			//if (g_Data.getLocalPlayer()->region->getBlock(vec3i(corners[n]->x, corners[n]->y - 0.5f, corners[n]->z))->toLegacy()->blockId != 0) {
			//DrawUtils::drawBox(vec3(floor(corners[n]->x), floor(corners[n]->y - 0.5f), floor(corners[n]->z)), g_Data.getLocalPlayer()->region->getBlock(vec3i(corners[n]->x, corners[n]->y - 0.5f, corners[n]->z))->toLegacy()->aabb.upper, 2.f);
			//	DrawUtils::drawBox(vec3(floor(corners[n]->x), floor(corners[n]->y - 0.5f), floor(corners[n]->z)), vec3(floor(corners[n]->x) + 1.f, floor(corners[n]->y - 0.5f) + 1.f, floor(corners[n]->z) + 1.f), 2.f);
			n++;

			if ((floor(corner->x) == floor(block->x)) && (floor(corner->y) == floor(block->y)) && (floor(corner->z) == floor(block->z))) {
				//	DrawUtils::setColor(1.f, 0.f, 0.f, 0.5f);
				//	DrawUtils::drawBox(block->floor(), {floor(block->x) + 1.f, floor(block->y) + 1.f, floor(block->z) + 1.f}, 0.7f, false);
				return true;
				//	}
			}
		}

	return false;
}

bool checkSurroundedAutoCrystal(C_Entity* ent) {
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 1;

	std::vector<vec3_ti*> blockChecks;
	blockChecks.clear();

	if (blockChecks.empty()) {
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z + 2));
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z - 2));
		blockChecks.push_back(new vec3_ti(entPos.x + 1, entPos.y, entPos.z));
		blockChecks.push_back(new vec3_ti(entPos.x - 1, entPos.y, entPos.z));
	}

	for (auto blocks : blockChecks) {
		if (!getGoodPlacementsAC(&blocks->toVector3(), ent)) {
			return false;
		}
	}
	return true;
}

std::vector<vec3_t*> getGoodPlacements2AC(C_Entity* ent) {
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 1;
	std::vector<vec3_t*> finalBlocks;
	std::vector<vec3_ti*> blockChecks;
	blockChecks.clear();
	finalBlocks.clear();
	if (blockChecks.empty()) {
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z + 1));
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z - 1));
		blockChecks.push_back(new vec3_ti(entPos.x + 1, entPos.y, entPos.z));
		blockChecks.push_back(new vec3_ti(entPos.x - 1, entPos.y, entPos.z));
	}

	for (auto blocks : blockChecks) {
		auto blkID = g_Data.getLocalPlayer()->region->getBlock(*blocks)->toLegacy()->blockId;
		auto blkIDL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 1, blocks->z))->toLegacy()->blockId;
		auto blkIDLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 2, blocks->z))->toLegacy()->blockId;
		auto blkIDLLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 3, blocks->z))->toLegacy()->blockId;
		auto blkIDLLLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 4, blocks->z))->toLegacy()->blockId;

		if (!getGoodPlacementsAC(&blocks->toVector3(), ent)) {  //very efficient coding here

			if (blkID == 0 && blkIDL == 0 && (blkIDLL == 49 || blkIDLL == 7)) {
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y - 1, blocks->z));
			}
			else if (blkID == 0 && (blkIDL == 7 || blkIDL == 49)) {
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y, blocks->z));
			}

			/*if (blkID == 0 && blkIDL == 0 && (blkIDLL == 49 || blkIDLL == 7)) {
				finalBlocks.push_back(new vec3(blocks->x, blocks->y - 1, blocks->z));
			} else if (blkID == 0 && (blkIDL == 7 || blkIDL == 49)) {
				finalBlocks.push_back(new vec3(blocks->x, blocks->y, blocks->z));
			} else if (blkID == 0 && blkIDL == 0 && blkIDLL == 0 && (blkIDLLL == 7 || blkIDLLL == 49)) {
				finalBlocks.push_back(new vec3(blocks->x, blocks->y - 2, blocks->z));
			} else if (blkID == 0 && blkIDL == 0 && blkIDLL == 0 && & blkIDLLL == 0 && (blkIDLLLL == 7 || blkIDLLLL == 49)) {
				finalBlocks.push_back(new vec3(blocks->x, blocks->y - 3, blocks->z));
			}*/
			static auto AC = moduleMgr->getModule<AutoCrystal>();

			if (AC->doMultiple) {
				for (int x = entPos.x - AC->rad; x <= entPos.x + AC->rad; x++) {
					for (int z = entPos.z - AC->rad; z <= entPos.z + AC->rad; z++) {
						int y = entPos.y;
						auto blk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y, z))->toLegacy()->blockId;
						auto lBlk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y - 1, z))->toLegacy()->blockId;

						if ((blk == 0 && (lBlk == 49 || lBlk == 7))) {
							finalBlocks.push_back(new vec3_t(x, y, z));
						}
					}
				}
			}
			else {
				for (int x = entPos.x - 1; x <= entPos.x + 1; x++) {
					for (int z = entPos.z - 1; z <= entPos.z + 1; z++) {
						int y = entPos.y;
						//‚±‚±‚ªCystal‚ð’u‚­‘åŒ³
						auto blk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y - 1, z))->toLegacy()->blockId;
						auto lBlk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y - 1, z))->toLegacy()->blockId;

						if ((blk == 0 && (lBlk == 49 || lBlk == 7))) {
							finalBlocks.push_back(new vec3_t(x, y, z));
						}
					}
				}
			}
		}
	}
	return finalBlocks;
}

bool acplaceCheck = false;
void AutoCrystal::onEnable() {
	crystalDelayac = 0;
	crystalDelayacbreak = 0;
	acplaceCheck = false;
}

std::vector<vec3_t*> placeRenderac;

void findCrystalAC() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 637) {  // select crystal
				supplies->selectedHotbarSlot = n;
				//return true;
				return;
			}
		}
	}
	//return false;
}

void AutoCrystal::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr) return;
	if (isClick && !g_Data.isRightClickDown()) return;

	if (g_Data.getLocalPlayer()->getSelectedItemId() == 259 && g_Data.isRightClickDown()) return;

	targetListAC.clear();

	g_Data.forEachEntity(getEntityAC);
	if (!targetListAC.empty()) { targetnameac = string(targetListAC[0]->getNameTag()->getText()); }
	else { targetnameac = "None"; }
	Odelay = 0;
	int healthy = g_Data.getLocalPlayer()->getHealth();
	if (healthy > sthealth - 1) {
		if (autoplace) {
			if ((crystalDelayac >= this->placedelay) && !(targetListAC.empty())) {
				crystalDelayac = 0;
				if (!checkSurroundedAutoCrystal(targetListAC[0])) {
					std::vector<vec3_t*> gucciPositions = getGoodPlacements2AC(targetListAC[0]);

					auto supplies = g_Data.getLocalPlayer()->getSupplies();
					auto inv = supplies->inventory;
					slotAC = supplies->selectedHotbarSlot;

					//615 = normal id for crystal || 616 = crystal id for nukkit servers
					if (!gucciPositions.empty()) {
						if (silentAC) findCrystalAC();
						if (g_Data.getLocalPlayer()->getSelectedItemId() == 637) {
							placeRenderac.clear();
							for (auto place : gucciPositions) {
								if (acplaceCheck && !doMultiple) break;
								if (targetListAC.empty()) return;
								//if (silentAC) findCrystalAC(); //place code
								C_Block* block = g_Data.getLocalPlayer()->region->getBlock(place->floor());
								int blockID = (int)block->toLegacy()->blockId;
								Odelay++;
								if (Odelay >= 0) {
									gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
									gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
									gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
									gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
									gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
									gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
								}
								placeRenderac.push_back(new vec3_t(place->x, place->y - 1, place->z));
								acplaceCheck = true;
							};
						};
						if (silentAC) supplies->selectedHotbarSlot = slotAC;
					};
					gucciPositions.clear();
				};
			}

			else if (!targetListAC.empty()) {
				crystalDelayac++;
			};
		};
		if (autobreak) {
			g_Data.forEachEntity([](C_Entity* ent, bool b) { //break code
				if (!(targetListAC.empty())) {
					crystalDelayacbreak = 0;
					int id = ent->getEntityTypeId();
					int range = moduleMgr->getModule<AutoCrystal>()->range;
					if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= range) {
						g_Data.getCGameMode()->attack(ent);
						g_Data.getCGameMode()->attack(ent);
						g_Data.getCGameMode()->attack(ent);
						g_Data.getCGameMode()->attack(ent);
						g_Data.getCGameMode()->attack(ent);
						g_Data.getCGameMode()->attack(ent);
						acplaceCheck = false;
						if (!moduleMgr->getModule<NoSwing>()->isEnabled())
							g_Data.getLocalPlayer()->swingArm();
					};
				};
				});
		};
	};
};

void AutoCrystal::onDisable() {
	crystalDelayac = 0;
	acplaceCheck = false;
}
void AutoCrystal::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (renderAC) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		auto cd = ColorUtil::interfaceColor(1);
		if (localPlayer != nullptr && GameData::canUseMoveKeys()) {
			if (!targetListAC.empty()) {
				if (!placeRenderac.empty()) {
					for (auto postt : placeRenderac) {
						postt->floor().y + 1;
						DrawUtils::drawwtf(*postt, 0.2f);
					}
				}
			}
		}
	}
}