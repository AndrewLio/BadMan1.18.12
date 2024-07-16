#include "FastStop.h"
#include "../pch.h"

FastStop::FastStop() : IModule(0, Category::MOVEMENT, u8"����ֹͣ����") {
}

const char* FastStop::getModuleName() {
	return ("FastStop");
}

void FastStop::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	auto targetStrafe = moduleMgr->getModule<TargetStrafe>();
	if (targetStrafe->isEnabled() && !targetStrafe->targetListEmpty && targetStrafe->mode.getSelectedValue() == 1)
		return;

	bool pressed = MoveUtil::keyPressed();
	if (!pressed) MoveUtil::stop(true);
}