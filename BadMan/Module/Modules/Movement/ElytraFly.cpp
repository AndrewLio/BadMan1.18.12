#include "ElytraFly.h"

ElytraFly::ElytraFly() : IModule(0x0, Category::MOVEMENT, u8"ÇÊ³á·ÉÐÐ") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.75f, 2.f);
}

ElytraFly::~ElytraFly() {
}

const char* ElytraFly::getModuleName() {
	return ("Elytra+");
}

void ElytraFly::onSendPacket(Packet* packet) {
	if (g_Data.getLocalPlayer() != nullptr && packet->isInstanceOf<PlayerActionPacket>()) {
		auto* pachet = reinterpret_cast<PlayerActionPacket*>(packet);

		if (pachet->action == 15 && pachet->entityRuntimeId == g_Data.getLocalPlayer()->entityRuntimeId) {
			flying = true;
		}
		else if (pachet->action == 16 && pachet->entityRuntimeId == g_Data.getLocalPlayer()->entityRuntimeId || g_Data.getLocalPlayer()->onGround) {
			flying = false;
		}

	}

}

void ElytraFly::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	float yaw = player->yaw;

	bool pressed = input->forward || input->backward || input->right || input->left;

	if (flying && !g_Data.getLocalPlayer()->onGround) {
		if (input->forward && input->backward)
			return;

		if (input->right) {
			yaw += 90.f;
			if (input->forward)
				yaw -= 45.f;
			else if (input->backward)
				yaw += 45.f;
		}
		if (input->left) {
			yaw -= 90.f;
			if (input->forward)
				yaw += 45.f;
			else if (input->backward)
				yaw -= 45.f;
		}

		if (input->backward && !input->left && !input->right)
			yaw += 180.f;

		float calcYaw = (yaw + 90) * (PI / 180);
		vec3_t moveVec;
		moveVec.x = cos(calcYaw) * speed;
		moveVec.y = player->velocity.y;
		moveVec.z = sin(calcYaw) * speed;
		if (pressed) player->lerpMotion(moveVec);
	}
}