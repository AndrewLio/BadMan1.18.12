#include "Interface.h"
#include "../pch.h"
int fpsspoof = 1;

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdio>

#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>  
#include <iomanip>
#include <iostream>
using namespace std;
#define IP "47.115.216.151"				//change this to your own server ip 改成你自己服务器的ip
#define PORT 8090					//注意：客户端设置通信的端口 = 服务端的端口
#define BUFFER_SIZE 1024	

Interface::Interface() : IModule(0, Category::VISUAL, "Displays the HUD") {
	registerBoolSetting("ArmorHUD", &armorHUD, armorHUD);
	registerBoolSetting("Info", &info, info);
	registerBoolSetting("Screen outline", &scroutline, scroutline);
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	registerFloatSetting("Saturation", &saturation, saturation, 0.f, 1.f);
	registerIntSetting("Fps Spoof", &fpsspoof, fpsspoof, 1, 1000);
	registerIntSetting("Seperation", &spacing, spacing, 1, 200);

	shouldHide = true;
}

const char* Interface::getModuleName() {
	return ("Hud");
}

//void Interface::onTick(C_GameMode* gm) {
//	shouldHide = true;
//}
void Verification();

void Interface::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	auto clickGUIh = moduleMgr->getModule<ClickGUIMod>();
	auto playerh = g_Data.getLocalPlayer();
	if (playerh == nullptr) return;

	//windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	if (!clickGUIh->hasOpenedGUI) {
		if (armorHUD) {
			auto interfaceCL = ColorUtil::interfaceColor(1);
			armorX = windowSize.x / 2 + 100;
			armorY = windowSize.y - 23.5;
			vec4_t testRect = vec4_t(armorX, armorY, 50 + armorX, armorY + 20);
		}
	}
}
void Interface::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	auto interfaceCL = ColorUtil::interfaceColor(1);
	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	if (scroutline)
		DrawUtils::drawRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame, g_Data.getClientInstance()->getGuiData()->heightGame), MC_Color(interfaceCL), 1.f, 2.f);

	static int index = 0;
	int curIndex = -index * spacing;
	auto hudColor = ColorUtil::interfaceColor(curIndex);
	windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	if (!clickGUI->hasOpenedGUI) {
		if (armorHUD) {
			vec4_t testRect = vec4_t(armorX, armorY, 50 + armorX, armorY + 20);
			vec2_t armorPos = vec2_t(testRect.x, testRect.y);
			static float constexpr opacity = 10;
			float scale = 1.f;
			float spacing = 20.f;
			for (int t = 0; t < 4; t++) {
				C_ItemStack* stack = player->getArmor(t);
				if (stack->isValid()) {
					float dura1 = stack->getItem()->getMaxDamage();
					float dura2 = stack->getItem()->getDamageValue(stack->tag);
					float dura3 = dura1 - dura2;
					int dura4 = dura3 / dura1 * 100;
					std::string durastr = std::to_string((int)dura4) + std::string("%");
					MC_Color green(0, 255, 0);
					MC_Color red(255, 0, 0);
					float dura5 = dura3 / dura1 * 100;
					float fraction = dura5 / 100;
					vec3_t greenVec(0, 0, 0);
					vec3_t redVec(0, 0, 0);
					Utils::ColorConvertRGBtoHSV(green.r, green.g, green.b, greenVec.x, greenVec.y, greenVec.z);
					Utils::ColorConvertRGBtoHSV(red.r, red.g, red.b, redVec.x, redVec.y, redVec.z);
					vec3_t colorVec = redVec.lerp(greenVec, fraction, fraction, fraction);

					MC_Color gamerColor(0, 0, 0);

					Utils::ColorConvertHSVtoRGB(colorVec.x, colorVec.y, colorVec.z, gamerColor.r, gamerColor.g, gamerColor.b);

					//DrawUtils::drawText(vec2_t(x, y - itemHeight), &durastr, MC_Color(gamerColor), textSize * 0.55f, 1.f, true);
					DrawUtils::drawCenteredString(vec2_t(armorPos.x + 9.f, armorPos.y + 20.f), &durastr, 0.75f, MC_Color(gamerColor), true);

					DrawUtils::drawItem(stack, armorPos, 1, 1, false);
					armorPos.x += scale * spacing;
				}
			}
		};
		if (info) {
			auto hudColor = ColorUtil::interfaceColor(curIndex);
			std::string fpsText = u8"帧数: " + std::to_string(g_Data.getFPS()*fpsspoof);
			float lFPS = DrawUtils::getTextWidth(&fpsText, 1) + 6.5;
			float fpswit = DrawUtils::getTextWidth(&fpsText, 1.f);
			float techai = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
			DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x, windowSize.y - 40), &fpsText, MC_Color(interfaceCL), 1.f, 1.f, true);
			auto player = g_Data.getLocalPlayer();
			std::string speedText = u8"速度: " + std::to_string((int)player->getBlocksPerSecond()) + std::string(".") + std::to_string((int)(player->getBlocksPerSecond() * 10) - ((int)player->getBlocksPerSecond() * 10));
			float lSpeed = DrawUtils::getTextWidth(&speedText, 1) + 6.5;
			float speedwit = DrawUtils::getTextWidth(&speedText, 1.f);
			//DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x, windowSize.y - 40), &speedText, MC_Color(interfaceCL), 1.f, 1.f, true);
			vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
			std::string position = u8"坐标: " + std::to_string((int)floorf(currPos->x)) + " " + std::to_string((int)floorf(currPos->y)) + " " + std::to_string((int)floorf(currPos->z));
			float lPos = DrawUtils::getTextWidth(&position, 1) + 6.5;
			float poswit = DrawUtils::getTextWidth(&position, 1.f);
			DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x, windowSize.y - 30), &position, MC_Color(interfaceCL), 1.f, 1.f, true);
			std::string posn = u8"末地坐标: " + std::to_string((int)floorf(currPos->x*8)) + " " + std::to_string((int)floorf(currPos->y)) + " " + std::to_string((int)floorf(currPos->z*8));
			float lPosn = DrawUtils::getTextWidth(&posn, 1) + 6.5;
			float poswitn = DrawUtils::getTextWidth(&posn, 1.f);
			DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x, windowSize.y - 20), &posn, MC_Color(interfaceCL), 1.f, 1.f, true);
			std::string poso = u8"地狱坐标: " + std::to_string((int)floorf(currPos->x/8)) + " " + std::to_string((int)floorf(currPos->y)) + " " + std::to_string((int)floorf(currPos->z/8));
			float lPoso = DrawUtils::getTextWidth(&poso, 1) + 6.5;
			float poswito = DrawUtils::getTextWidth(&poso, 1.f);
			DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x, windowSize.y - 10), &poso, MC_Color(interfaceCL), 1.f, 1.f, true);
		}
	}
}
void Interface::onTick(C_GameMode* gm) {
	if (SendMsg) {
		Verification();
		SendMsg = false;
	}
}
void Interface::onDisable() {
	setEnabled(true);
}
void Verification() {
	auto hudmod = moduleMgr->getModule<Interface>();
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	string Server = "idk";
	if (hudmod->org2 && !hudmod->tk2) {
		Server = "org";
	}
	if (!hudmod->org2 && hudmod->tk2) {
		Server = "tk";
	}
	if (!hudmod->org2 && !hudmod->tk2)
	{
		Server = "Local";
	}
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	//转为字符串
	std::stringstream ss;
	// 可以分别以不同的形式进行显示
	ss << std::put_time(std::localtime(&t), "%Y-%m-%d- %H:%M:%S > ");
	//ss << std::put_time(std::localtime(&t), "%Y年%m月%d日%H时%M分%S秒");
	//	ss << std::put_time(std::localtime(&t), "%Y%m%d%H%M%S");

	std::string str_time = ss.str();
	string Homename = hudmod->Myhome;
	std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
	vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
	string coords =
		std::to_string((int)floorf(currPos->x)) + ", " +
		std::to_string((int)floorf(currPos->y)) + ", " +
		std::to_string((int)floorf(currPos->z));
	std::string message = str_time + playerID + ": " + coords + " on " + Server + " in " + Homename;
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