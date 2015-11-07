/*
	debuger类:用于调试
*/
#ifndef DEBUGER_H_
#define DEBUGER_H_
#include "user_define.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
class Debuger{
public:
	static void PrintAddr(const address& addr);
	static void PrintKeyEntry(const key_entry& ke);
	static void PrintFingerEntry(const finger_entry& fe);
	static void PrintHashValue(const hash_value& hv);
	static void PrintDescript(const std::string& des);
	static void PrintNewLine();
	static void formatOutput(int width,char c=' ');
private:		
	static void PrintStr(const std::string& msg);
	static void PrintInt(int v);
	static void PrintuInt(unsigned int v);	
public:
	//格式化参数常量
	const static int STR_TEMP_LEN =30;	//临时字符缓冲长度
	const static int DEFAULT_WIDTH =8; //默认宽度 相当'\t'
	const static int OUT_NAME_LEN =15; //名称格式宽度
	const static int OUT_HASH_LEN=10; //hash值格式宽度
	const static int OUT_IP_LEN =18; //IP格式宽度
	const static int OUT_PORT_LEN = 10;//端口格式宽度
	const static int OUT_EXTENT	=8;//拓展格式宽度
};
#endif