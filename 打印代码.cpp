// 打印代码.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <easyx.h>			// 引用图形库头文件
#include <conio.h>
#include <string>
#include <locale.h>
#include <fstream>
#include <graphics.h>
#include <atltrace.h>
#include <Windows.h>
#include "iconv.h"
#include <assert.h>

#pragma comment(lib,"libiconv.lib")

#define DEBUG

using namespace std;

int UTF8ToGB2312(char* srcStr, char* desBuff, size_t desBuffLength);
void s_setFont(LOGFONT& lf);
void s_initEnv();
int InsertTab(char* str);

#ifdef DEBUG
int width = 500, height = 500;
#else
int width = 2000, height = 4000;
#endif
int lineHeight = 20;



int main(int argc,char **argv)
{
	//setlocale(LC_ALL, "chs.UTF-8");
	//setlocale(LC_ALL, "UTF-8");
	

	
	char buff[1024];
	ifstream ifs;
	//open 前不能调用本地化 UTF-8，因为系统传入得参数是Unicode，一转UTF-8得话open时
	//就会把这个参数认为是UTF-8的编码。
	//ifstream 在打开文件时是默认把char* 在内部转换成 宽字符。
	ifs.open(argv[1]);
	setlocale(LC_ALL, "zh_CN.UTF-8");
	int c = GetLastError();

	if (!ifs.is_open()) {
		MessageBoxA(NULL, "请检查是否有权限", "打开失败", MB_OK);
		exit(EXIT_FAILURE);
	}
	int line = 0;
	int maxLength = 0;
	while (!ifs.eof()) {
		ifs.getline(buff, 1023);
		if (!ifs) break;
		line++;
		sprintf(buff,"%s", buff);
		printf("%s", buff);
		ifs.gcount() > maxLength ? maxLength = ifs.gcount() : 0;
	}
	width = maxLength * 10;
	height = line * 20 + 100;

	s_initEnv();
	outtextxy(20, 20, argv[1]);
	ifs.seekg(ios::beg);
	setlocale(LC_ALL, "C");
	int i = 1;
	while (i<=line)
	{
		ifs.getline(buff, 1023);
		char desbuff[1024] = { 0 };
		desbuff[UTF8ToGB2312(buff, desbuff, 1023)-3] = '0';
		
		InsertTab(desbuff);

		outtextxy(20, (i + 1) * 20, desbuff);

		/*RECT rect;
		rect.left = 20;
		rect.right = 20 + textwidth(buff);
		rect.top = (i + 1) * 20;
		rect.bottom = rect.top + 20;
		drawtext(desbuff, &rect, DT_LEFT);*/
		i++;
	}
	saveimage("save.png");
	char ch = _getch();
	ch = _getch();
	closegraph();
}

void s_setFont(LOGFONT& lf) {
	printf("%d", lf.lfWidth);
	lf.lfHeight = 20;
	lf.lfWeight = FW_MEDIUM;
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
	strcpy(lf.lfFaceName, "Monaco");
}

void s_initEnv() {
	initgraph(width, height,SHOWCONSOLE);

	setbkcolor(RGB(0xee, 0xee, 0xee));
	settextcolor(RGB(0xff, 0x40, 0x40));
	setbkmode(TRANSPARENT);
	LOGFONT lf;
	char buff[128];
	gettextstyle(&lf);
	s_setFont(lf);
	settextstyle(&lf);
	cleardevice();
}

int UTF8ToGB2312(char* srcStr, char* desBuff, size_t desBuffLength)
{
	assert(strlen(srcStr) > 0);
	size_t iLen = strlen(srcStr);
	iconv_t cd;
	cd = iconv_open("gb2312", "utf-8");
	assert(cd != 0);
	iconv(cd, &srcStr, &iLen, &desBuff, &desBuffLength);
	iconv_close(cd);
	return desBuffLength;
}

/*把tab转换成四个空格*/
// "\tabc" --> "    abc"
int InsertTab(char* str) {
	int tab = 0; int t = 0;
	for (int j = 0; j < strlen(str); j++) {
		if (str[j] == '\t') {
			t = strlen(str);
			while (--t != j) str[t + 3] = str[t];
			t = j + 4;
			while (j < t) str[j++] = ' ';
			tab++;
		}
	}
	return tab;
}