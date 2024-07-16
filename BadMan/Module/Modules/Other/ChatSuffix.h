#pragma once
#include "../Module.h"
class ChatSuffix : public IModule {
public:
	ChatSuffix();
	~ChatSuffix();
	bool green = true;
	bool bypass1 = true;
	bool bypass2 = true;
	bool suffix = true;
	bool reset = false;
	int bypass1length = 4;
	int bypass2length = 9;
	std::string Suffix = "BadMan";
	inline std::string& getCustomMessage2() { return Suffix; };
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(Packet* packet) override;
};