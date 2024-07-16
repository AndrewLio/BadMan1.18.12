#include "../../ModuleManager.h"
#include "TestModule.h"
#include <iomanip>
#include <chrono>
#include <string>
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <direct.h>
#include <conio.h>
#include <sstream>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <TlHelp32.h>
#include <tchar.h> 
#include <vector>
#include <thread>
#include <wtsapi32.h>
#include <psapi.h>
#include <math.h>
#include <algorithm> 
#include <playsoundapi.h>
#include <windows.h>
#include <tchar.h>
#include <urlmon.h>
#include "../pch.h"


#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdio>

#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>  
#define IP "47.115.216.151"				//change this to your own server ip 改成你自己服务器的ip
#define PORT 8090					//注意：客户端设置通信的端口 = 服务端的端口
#define BUFFER_SIZE 1024			//数据发送缓冲区大小

using namespace std;

#pragma region 全局变量

SOCKET client;					// 本地套接字
sockaddr_in sai_client;			// 存放客户端IP地址、端口

// 消息格式
struct umsg {
	int type;					// 协议（1：登录，2：退出，3：发消息）
	char name[64];				// 用户名字
	char text[512];				// 文本
};

#pragma endregion

#pragma region 依赖函数

TestModule::TestModule() : IModule(0, Category::OTHER, "For testing purposes") {
	registerBoolSetting("Notification", &notification, notification);
	registerBoolSetting("DebugInfo", &debugInfo, debugInfo);
	registerBoolSetting("TestSound", &test2, test2);
	registerBoolSetting("CopyChat", &copyChatMessages, copyChatMessages);
	registerBoolSetting("Blink", &blink, blink);
	registerBoolSetting("Test", &test, test);
	registerBoolSetting("AutoDisable", &autoDisable, autoDisable);
	registerFloatSetting("SliderX", &sliderX, sliderX, -40.f, 40.f);
	registerFloatSetting("SliderY", &sliderY, sliderY, -40.f, 40.f);
	registerFloatSetting("SliderZ", &sliderZ, sliderZ, -40.f, 40.f);
	registerFloatSetting("SliderX2", &sliderX2, sliderX2, -4.f, 4.f);
	registerFloatSetting("SliderY2", &sliderY2, sliderY2, -4.f, 4.f);
	registerFloatSetting("SliderZ2", &sliderZ2, sliderZ2, -4.f, 4.f);
	registerBoolSetting("Kow", &kowBool, kowBool);
	registerFloatSetting("KowFloat", &kowFloat, kowFloat, 1.f, 100.f);
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

static vector<C_Entity*> targetList;
void findEntity_TestModule(C_Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (currentEntity->getEntityTypeId() == 80)  // Arrows
		return;

	if (!TargetUtil::isValidTarget(currentEntity))
		return;

	if (currentEntity->getEntityTypeId() == 1677999)  // Villager
		return;

	if (currentEntity->getEntityTypeId() == 51)  // NPC
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 255) targetList.push_back(currentEntity);
}
void fuck() {

	if (g_Data.getLocalPlayer() == nullptr)
		return;
	string server2 = "Local";
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	if (g_Data.getRakNetInstance()->isonaServer()) {
		 server2 = g_Data.getRakNetInstance()->serverIp.getText();
	}
	//转为字符串
	std::stringstream ss;
	// 可以分别以不同的形式进行显示
	ss << std::put_time(std::localtime(&t), "%Y-%m-%d- %H:%M:%S > ");
	//ss << std::put_time(std::localtime(&t), "%Y年%m月%d日%H时%M分%S秒");
	//	ss << std::put_time(std::localtime(&t), "%Y%m%d%H%M%S");

	std::string str_time = ss.str();

	std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
	vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
	string coords =
		std::to_string((int)floorf(currPos->x)) + ", " +
		std::to_string((int)floorf(currPos->y)) + ", " +
		std::to_string((int)floorf(currPos->z));
	std::string message = str_time + playerID + ": " + coords + " on " + server2;
	char buf[1024];
	int i;
	for (i = 0; i < message.length(); i++)
		buf[i] = message[i];
	buf[i] = '\0';
	printf("%s\n", buf);
	cout << buf;						//buf数组存放客户端发送的消息  


	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) == SOCKET_ERROR)  //WSAStartup()函数对Winsock DLL进行初始化
	{
		//printf("Socket initialize fail!\n");
		return;
	}
	SOCKET sock;											//客户端进程创建套接字
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)  //创建流套接字（与服务端保持一致）
	{
		//printf("Socket create fail!\n");
		WSACleanup();
		return;
	}

	struct sockaddr_in ClientAddr;				//sockaddr_in结构用来标识TCP/IP协议下的地址，可强制转换为sockaddr结构
	ClientAddr.sin_family = AF_INET;				//指Internet域
	ClientAddr.sin_port = htons(PORT);			//指定服务端所预留的端口
	ClientAddr.sin_addr.s_addr = inet_addr(IP);	//指定服务端所绑定的IP地址
	if (connect(sock, (LPSOCKADDR)&ClientAddr, sizeof(ClientAddr)) == SOCKET_ERROR)  //调用connect()函数，向服务器进程发出连接请求  
	{
		//printf("Connect fail!\n");
		closesocket(sock);
		WSACleanup();
		return;
	}
	send(sock, buf, 1024, 0);				 //向服务器发送数据 
	closesocket(sock);							 //关闭套接字
	WSACleanup();								//终止对Winsock DLL的使用，并释放资源，以备下一次使用
}

void TestModule::onEnable() {
	/*auto blinkMod = moduleMgr->getModule<Blink>();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	targetList.clear();

	if (notification) {
		NotificationManager notification;
		// idk it wont edik
		notification.addNotification("Title", "Message");
	}
	if (blink) testCounter = 0;

	initPos = *player->getPos();
	tick = 0;
	*/
}

void TestModule::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr) return;
	/*C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	auto blinkMod = moduleMgr->getModule<Blink>();

	if (player == nullptr) return;
	if (input == nullptr) return;
	tick++;

	float calcYaw = (gm->player->yaw + 90) * (PI / 180);

	targetList.clear();
	g_Data.forEachEntity(findEntity_TestModule);

	if (test) {
		//MoveUtil::setSpeed(1.f);
		/*static int testTick = 0;
		testTick++;

		if (testTick == 20) {
			clientMessageF("sent");
			CommandRequestPacket commandPacket;
			commandPacket.payload = string("q sky");
			commandPacket.one = 1;
			commandPacket.two = 2;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&commandPacket);
		}
		else if (testTick == 21) testTick = 0;

		// testing random shit
	}

	if (blink) {
		if (testCounter == 60) {
			blinkMod->setEnabled(false);
			testCounter = 1;
		}
		else {
			testCounter++;
		}
		if (testCounter == 2) {
			blinkMod->setEnabled(true);
		}
	}

	if (GameData::isKeyDown(*input->sneakKey) && kowBool) g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	setEnabled(!autoDisable);*/
}

void TestModule::onMove(C_MoveInputHandler* input) {
}

void TestModule::onPlayerTick(C_Player* plr) {
	if (plr == nullptr) return;

	PointingStruct* level = plr->pointingStruct;
	vec3_t* pos = plr->getPos();

	static bool shouldFly = false;
	if (plr->damageTime >= 1) shouldFly = true;

	if (!shouldFly) {
		//MoveUtil::freeze(initPos, false);
	}

	//for (auto& i : targetList) {
		//targetList[0]->spawnDustParticles(100);
	//}
	//plr->spawnDustParticles(10000);
	//level->playSound("game.player.hurt", *pos, 1, 1); plr->animateHurt();
}

void TestModule::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	/*auto hudMod = moduleMgr->getModule<Interface>();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	targetList.clear();


	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	if (kowBool) {
		string Text = "Player";
		MC_Color CumColor = MC_Color(255, 255, 255); // lol
		if (g_Data.canUseMoveKeys()) {
			int ySize = 0;
			int ind = 1;
			ind++;
			int curIndex = -ind * 75;
			auto FunkyColor = ColorUtil::waveColor(hudMod->r, hudMod->g, hudMod->b, hudMod->r2, hudMod->g2, hudMod->b2, curIndex * 2);
			float TextWidth = DrawUtils::getTextWidth(&Text, 1) + 6.5;
			vec4_t RectPos = vec4_t(4.f, 12.f + 1, TextWidth + 52.f, 12.f + 13);//Start X, Positive Y Height, how long to make width going right, bg negative y height
			vec4_t SeparaterPos = vec4_t(4.f, RectPos.y + 11, TextWidth + 52.f, RectPos.w + 1.f);
			vec2_t TextPos = vec2_t(RectPos.x + 2, RectPos.y + 3.5);

			DrawUtils::fillRectangleA(RectPos, MC_Color(0, 0, 0, 150));
			DrawUtils::fillRectangleA(SeparaterPos, FunkyColor);
			DrawUtils::drawText(TextPos, &Text, CumColor, .7f, 1);//Player Label

			for (auto& i : targetList) {
				ySize += 10;
				string EnemyIgn = i->getNameTag()->getText();
				vec2_t EnemyName = vec2_t(RectPos.x + 2, SeparaterPos.y + (3.5 + ySize));

				DrawUtils::drawText(EnemyName, &EnemyIgn, CumColor, .7f, 1);
			}
		}
	}
	else {
		vec2_t targetPosition = vec2_t(200, 200);
		vec2_t position = vec2_t(10, 10);
		position.x = lerp(position.x, targetPosition.x, 0.1f);

		string textStr = "Test";
		DrawUtils::drawGradientText(position, &textStr, 1, 255);
		//DrawUtils::fillRectangleA(vec4_t(10, 10, 50, 50), MC_Color(255, 255, 255, 255));
	}*/
	//so far can only get rendering from minecraft files
	//DrawUtils::drawImage("textures/ui/promo_creeper.png", vec2_t(100, 100), vec2_t(800, 100), vec2_t(0.f, 0.f), vec2_t(1.f, 1.f) , MC_Color(0.f, 0.f, 1.f));
	//DrawUtils::flushImage();
}

void TestModule::onSendPacket(C_TextPacket * packet) {
	/*auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (g_Data.canUseMoveKeys()) {
		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			if (tick <= 10) {
				clientMessageF("dam");
				if (player->onGround) movePacket->Position.y -= 2.f;
			}
		}
	}*/
}

void TestModule::onDisable() {
	/*
	auto blinkMod = moduleMgr->getModule<Blink>();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	if (blink) blinkMod->setEnabled(false);
	*/
}