#include "Killsults.h"

#include <stdlib.h> /* srand, rand */

#include "../../../../Utils/Utils.h"
std::string plnm = "None";
vec3_t* plpos;
int poppedtotems = 0;
AutoEZ::AutoEZ() : IModule(0, Category::OTHER, "troll enemies and anger them2") {
	registerBoolSetting("Lose Message", &this->sayUserDied, this->sayUserDied);
	registerBoolSetting("Win Message", &this->sayEnemDied, this->sayEnemDied);
}

AutoEZ::~AutoEZ() {}

const char* AutoEZ::getModuleName() {
	std::string name = "AutoEZ";
	name += " " + std::string(GRAY) + plnm;

	return name.c_str();
}

const char* AutoEZ::getRawModuleName() {
	return "AutoEZ";
}

static std::vector<C_Entity*> targetListae;

void findEnemyPlayer(C_Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;  // something i just learned: return = terminate func

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (currentEntity->getEntityTypeId() != 319)  // player
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 12) {
		targetListae.push_back(currentEntity);
	}
}  // thanks badman

void AutoEZ::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	targetListae.clear();

	g_Data.forEachEntity(findEnemyPlayer);
	if (!targetListae.empty()) { plnm = std::string(targetListae[0]->getNameTag()->getText()); plpos = targetListae[0]->getPos();}
	else { plnm = "None"; }
	if (g_Data.isInGame() && sayEnemDied) {  // Send kill messages
		if (GameData::canUseMoveKeys()) {

			for (C_Entity* tgt : targetListae) {
				if (tgt->isAlive() && tgt->height > 1.5f) {
					foundPlayer = true;
				}

				if (foundPlayer && !tgt->isAlive() && tgt->getNameTag()->getTextLength() > 1) {
					isDead = true;
				}
				/*/
				vec3_t* targertpos = targetListae[0]->getPos();
				std::string pos = " | Target Pos: " + pos;
				/*/
				if (isDead) {
					C_TextPacket textPacket;
					//DrawUtils::drawEntityBox(targetListae[0], (float)fmax(0.2f, 1 / (float)fmax(1, (*player->getPos()).dist(*targetListae[0]->getPos()))));
					textPacket.message.setText("@" + plnm + " died after popping " + std::to_string(poppedtotems) + " totems");
					textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
					textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
					foundPlayer = false;
					isDead = false;
				}
			}
		}
	}
	if (targetListae.empty()) {
		foundPlayer = false;
		isDead = false;
	}

	if (g_Data.isInGame() && sayUserDied && antiSpamCheck) {  // Send death messages
		if ((g_Data.getLocalPlayer()->isAlive()) == false) {
			C_TextPacket textPacket;
			textPacket.message.setText(deathMsg);
			textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
			textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
			antiSpamCheck = false;
		}
	}

	if (antiSpamCheck == false) {
		if ((g_Data.getLocalPlayer()->isAlive()) == true) {
			antiSpamCheck = true;
		}
	}
}