#include "Killaura.h"
#include "../pch.h"
using namespace std;
string targetname = "None";
Killaura::Killaura() : IModule(0, Category::COMBAT, u8"杀戮光环") {
	registerEnumSetting("Rotations", &rotations, 0);
	rotations.addEntry("None", 0);
	rotations.addEntry("Silent", 1);
	rotations.addEntry("Client", 2);
	rotations.addEntry("Actual", 3);
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Single", 0);
	mode.addEntry("Multi", 1);
	registerEnumSetting("AutoWeapon", &AutoWeapon, 0);
	AutoWeapon.addEntry("None", 0);
	AutoWeapon.addEntry("Full", 1);
	AutoWeapon.addEntry("Silent", 2);
	registerBoolSetting("MobAura", &mobs, mobs);
	registerBoolSetting("Visualize", &visualize, visualize);
	registerBoolSetting("Hold", &hold, hold);
	registerFloatSetting("Range", &range, range, 3.f, 20.f);
	registerIntSetting("Attack delay", &delayka, delayka, 0, 20);
	registerBoolSetting("Swing", &swingKA, swingKA);
	registerBoolSetting("hurttime", &hurttimeka, hurttimeka);
}

void Killaura::findWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

const char* Killaura::getRawModuleName() {
	return "Killaura";
}

const char* Killaura::getModuleName() {
	name = string("Killaura " + string(GRAY) + targetname);
	return name.c_str();
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

static vector<C_Entity*> targetList;
void findEntity(C_Entity* currentEntity, bool isRegularEntity) {
	static auto killaura = moduleMgr->getModule<Killaura>();

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

	if (killaura->mobs) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 319) return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) return;
		//if (currentEntity->getEntityTypeId() == 64) return;
		if (currentEntity->getEntityTypeId() == 0xC00050)  // Arrows - new item id
			return;
		if (currentEntity->getEntityTypeId() == 307) // NPC
			return;
	}
	else {
		if (!TargetUtil::isValidTarget(currentEntity)) return;
		if (currentEntity->getEntityTypeId() == 51 || currentEntity->getEntityTypeId() == 1677999) return;
	}
	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < killaura->range) {
		targetList.push_back(currentEntity);
		sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	}
}

void Killaura::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	targetListEmpty = true;
	targetList.clear();
}

void Killaura::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	int dbrr = g_Data.getLocalPlayer()->getSelectedItemId();
	if (player == nullptr) return;

	PointingStruct* pointing = g_Data.getLocalPlayer()->pointingStruct;
	//auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	//if (clickGUI->isEnabled()) targetListEmpty = true;
	targetListEmpty = targetList.empty();

	targetList.clear();

	g_Data.forEachEntity(findEntity);
	Odelay++;
	slotKA = supplies->selectedHotbarSlot;
	if (!targetList.empty()) {targetname = std::string(targetList[0]->getNameTag()->getText());}
	else { targetname = "None"; }
	if (hold && !g_Data.isLeftClickDown())
		return;
	//if (wtfdb && (dbrr == 259 || dbrr == 426) && g_Data.isRightClickDown()) return;
	if (AutoWeapon.getSelectedValue() != 0 && !targetList.empty()) findWeapon();
	if (!targetList.empty() && Odelay >= delayka) {
		if (mode.getSelectedValue() == 0) {
			for (auto& i : targetList) {
				if (!(i->damageTime > 1 && hurttimeka)) {
					gm->attack(targetList[0]);
					gm->attack(targetList[0]);
					gm->attack(targetList[0]);
					gm->attack(targetList[0]);
					if (swingKA) {
						player->swing();
					}
				}
			}
		}
		if (mode.getSelectedValue() == 1) {
			for (auto& i2 : targetList) {
				if (!(i2->damageTime > 1 && hurttimeka)) {
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					gm->attack(i2);
					if (swingKA) {
						player->swing();
					}
				}
			}
		}
		if (AutoWeapon.getSelectedValue() == 2) supplies->selectedHotbarSlot = slotKA;
		if (rotations.getSelectedValue() == 3) {
			tro = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
		}
		Odelay = 0;
	}
	else targetListEmpty = true;
	//gm->destroyBlock(&vec3_ti(277070, 69, -254355), 1);
}

void Killaura::onPlayerTick(C_Player* plr) {
	if (plr == nullptr) return;

	if (!targetList.empty()) {
		if (hold && !g_Data.isLeftClickDown())
			return;

		for (auto& i : targetList) {
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			vec2_t pos = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());

			if (rotations.getSelectedValue() == 2) {
				plr->yawUnused1 = angle.y;
				plr->pitch = angle.x;
				plr->bodyYaw = angle.y;
			}

			// Strafe
			if (rotations.getSelectedValue() == 3) {
				plr->setRot(tro);
				plr->yawUnused1 = angle.y;
			}
		}
	}
}

void Killaura::onSendPacket(Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (!targetList.empty()) {
		if (hold && !g_Data.isLeftClickDown())
			return;

		if (packet->isInstanceOf<C_MovePlayerPacket>() && (rotations.getSelectedValue() == 1 || rotations.getSelectedValue() == 2) && !targetList.empty()) {
			vec2_t anglefd = g_Data.getLocalPlayer()->getPos()->CalcAngle(targetList[0]->eyePos0);
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);

			movePacket->pitch = anglefd.x;
			movePacket->headYaw = anglefd.y;
			movePacket->yaw = anglefd.y;
		}
	}
}

void Killaura::onLevelRender() {
	auto targetStrafe = moduleMgr->getModule<TargetStrafe>();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr || targetStrafe->isEnabled() || !visualize) return;

	static float n = 0;
	static float anim = 0;
	if (g_Data.canUseMoveKeys() && !targetList.empty()) {
		if (hold && !g_Data.isLeftClickDown())
			return;
		if (!targetList.empty()) {
			anim++;
			DrawUtils::setColor(1, 1, 1, 0.9f);

			vec3_t permutations[56];
			for (int i = 0; i < 56; i++) {
				permutations[i] = { sinf((i * 10.f) / (180 / PI)), 0.f, cosf((i * 10.f) / (180 / PI)) };
			}
			vec3_t permutations2[56];
			n++;
			if (n == 360)
				n = 0;
			for (int i = 0; i < 56; i++) {
				permutations2[i] = { sinf((i * 10.f) / (180 / PI)), sinf((i * 10.f) / (180 / PI)) ,cosf((i * 10.f) / (180 / PI)) };
			}

			const float coolAnim = 0.9f + 0.9f * sin((anim / 60) * PI * 1);

			vec3_t* start = targetList[0]->getPosOld();
			vec3_t* end = targetList[0]->getPos();

			auto te = DrawUtils::getLerpTime();
			vec3_t pos = start->lerp(end, te);

			auto yPos = pos.y;
			yPos -= 1.6f;
			yPos += coolAnim;

			vector<vec3_t> posList;
			posList.reserve(56);
			for (auto& perm : permutations) {
				vec3_t curPos(pos.x, yPos, pos.z);
				posList.push_back(curPos.add(perm));
			}
			auto interfaceMod = moduleMgr->getModule<Interface>();
			//auto interfaceColor = ColorUtil::interfaceColor(1);
			//DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, 1.f);
			DrawUtils::drawLineStrip3D(posList, 5);

			//Draw Sims Esp
			/*vector<vec3_t> posList2;
			posList2.clear();
			// Iterate through phi, theta then convert r,theta,phi to XYZ
			for (double phi = 0.; phi < 2 * PI; phi += PI / 100.) // Azimuth [0, 2PI]
			{
				for (double theta = 0.; theta < PI; theta += PI / 100.) // Elevation [0, PI]
				{
					vec3_t point;
					point.x = 0.2 * cos(phi) * sin(theta);
					point.y = 0.2 * sin(phi) * sin(theta);
					point.z = 0.2 * cos(theta);
					vec3_t curPos = targetList[0]->eyePos0;
					curPos.y += 0.5f;
					posList2.push_back(curPos.add(point));
				}
			}
			DrawUtils::setColor(0.1f,0.9f,0.1f,255);
			DrawUtils::drawLineStrip3D(posList2, 4);*/
		}
	}
}

void Killaura::onDisable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	targetListEmpty = true;
	targetList.clear();
}