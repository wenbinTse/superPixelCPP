// calcSuperPixel.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "Slic.h"
#include <iostream>
#include "ctime"
#include <cstring>

using namespace std;

wstring Str2Wstr(string str)
{
	if (str.length() == 0)
		return L"";

	wstring wstr;
	wstr.assign(str.begin(), str.end());
	return wstr;
}

int main(int argc, char* argv[])
{
	int K = 500;
	if (argc < 2) {
		cout << "请输入文件名" << endl;
		exit(-1);
	} else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		cout << "使用方法: calSuperPixel.exe {input} [k]" << endl;
		cout << "使用例子: calSuperPixel.exe Lenna.jpg 1000 或 calSuperPixel.exe Lenna.jpg" << endl;
		cout << "--------------------------------------------------------" << endl;
		cout << "input: 必填；字符串；输入的图像名" << endl;
		cout << "k    : 可选；正整数；超像素（晶格）的个数， 默认为500" << endl;
		cout << endl;
		exit(-1);
	} else if (argc < 2) {
		cout << "请输入文件名" << endl;
		exit(-1);
	}
	else if (argc == 3) {
		K = atoi(argv[2]);
		if (K <= 0) {
			cout << "K应为正整数" << endl;
			exit(-1);
		}
	}
	else if (argc > 3) {
		cout << "含非法参数" << endl;
		exit(-1);
	}

	try {
		string input(argv[1]);
		Slic slic(Str2Wstr(input), K, 32);
		slic.train(5);
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
