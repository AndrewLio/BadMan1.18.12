#include "AutoLog.h"
#include "../pch.h"

AutoLog::AutoLog() : IModule(0, Category::PLAYER, "Spams so many packets server kicks you") {
	registerIntSetting("Health", &health, health, 1, 20);
	registerEnumSetting("Text Modes", &ByeByeMessage, 0);
	registerBoolSetting("Log Message", &message, message);
	registerBoolSetting("Custom Message", &CustomMsg, CustomMsg);
	registerBoolSetting("Anvil Log", &OnAnvil, OnAnvil);
	ByeByeMessage = SettingEnum(this)
		.addEntry(EnumEntry("ByeBye!!", 0))
		.addEntry(EnumEntry("Good Bye!!", 1));
}
AutoLog::~AutoLog() {
}

const char* AutoLog::getModuleName() {
	return ("AutoLog");
}

bool FallingAnvil(C_Block* block) {
	int blockID = (int)block->toLegacy()->blockId;
	return (blockID == 145);
}
bool FallingConcretePowder(C_Block* block) {
	int blockID = (int)block->toLegacy()->blockId;
	return (blockID == 252);
}
bool FallingGravel(C_Block* block) {
	int blockID = (int)block->toLegacy()->blockId;
	return (blockID == 13);
}
bool FallingSand(C_Block* block) {
	int blockID = (int)block->toLegacy()->blockId;
	return (blockID == 12);
}

bool isItemAboveInPlayer(C_Player* plr, int itemID) {
	// Get the player's position
	vec3_t playerPos = plr->getPos()->floor();

	// Check the block above the player's head
	vec3_t blockAbovePos = vec3_t(playerPos.x, playerPos.y + 1, playerPos.z);
	C_Block* blockAbovePlayer = g_Data.getLocalPlayer()->region->getBlock(blockAbovePos);

	// Check if the block above has the specified item ID
	if (blockAbovePlayer->toLegacy()->blockId == itemID) {
		return true;  // Item ID detected above the player's head
	}

	return false;  // Item ID not detected
}

bool AutoLog::isFallingAnvilAboveinPlayer(C_Player* plr) {
	if (plr == nullptr) return false;

	vec3_ti blockpos3 = plr->getPos()->floor();
	blockpos3.y = (int)blockpos3.y - 2;

	// Check for anvils falling above the player's head
	for (int yOffset = 0; yOffset <= (int)height; yOffset++) {  // Use the height value from the slider
		vec3_ti blockposInAbove = blockpos3;
		blockposInAbove.y += yOffset;

		C_Block* blockInAbove = g_Data.getLocalPlayer()->region->getBlock(blockposInAbove);
		int blockAboveID = (int)blockInAbove->toLegacy()->blockId;

		if (blockAboveID == 145 && FallingAnvil(blockInAbove)) {
			return true;
		}
		if (blockAboveID == 252 && FallingConcretePowder(blockInAbove)) {
			return true;
		}
		if (blockAboveID == 13 && FallingGravel(blockInAbove)) {
			return true;
		}
		if (blockAboveID == 12 && FallingSand(blockInAbove)) {
			return true;
		}
	}

	return false;
}

void AutoLog::onTick(C_GameMode* gm) {
	auto plr = g_Data.getLocalPlayer();
	if (OnAnvil) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		int slottd2 = supplies->selectedHotbarSlot;
		bool fallingAnvilsDetected = isFallingAnvilAboveinPlayer(plr);
		bool item12Detected = isItemAboveInPlayer(plr, 12);  // Check if item ID 12 (sand) is detected above the player's head
		if (fallingAnvilsDetected || item12Detected) {
			caidelljthe = true;
			supplies->selectedHotbarSlot = slottd2;
			if (message == true) {
				C_TextPacket textPacket;
				if (ByeByeMessage.selected == 0) {
					textPacket.message.setText("ByeBye!!");
				}
				if (ByeByeMessage.selected == 1) {
					textPacket.message.setText("Good Bye!!");
				}
				if (CustomMsg) {
					textPacket.message.setText(getCustomMessage());
				}
				textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
				textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
			};
			for (int pp = 0; pp < 1000; pp++) {
				C_MovePlayerPacket movePacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
			}
		};
	}
	int healthy = g_Data.getLocalPlayer()->getHealth();
	if (healthy < health) {
		if (message == true) { 
			C_TextPacket textPacket;
			if (ByeByeMessage.selected == 0) {
				textPacket.message.setText("ByeBye!!");
			}
			if (ByeByeMessage.selected == 1) {
				textPacket.message.setText("Good Bye!!");
			};
			if (CustomMsg) {
				textPacket.message.setText(getCustomMessage());
			}
			textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
			textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
		};
		for (int pp = 0; pp < 1000; pp++) {
			C_MovePlayerPacket movePacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
		}		
	}
}