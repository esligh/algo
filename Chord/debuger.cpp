#include "stdafx.h"
#include "debuger.h"
#ifdef _WIN32_DEBUG
	#include <windows.h>
#endif
//打印节点地址
void Debuger::PrintAddr(const address& addr)
{
	formatOutput(OUT_IP_LEN);
	PrintStr(addr.ip);
	formatOutput(OUT_PORT_LEN);
	PrintInt(addr.port);
	formatOutput(OUT_HASH_LEN);
	PrintHashValue(addr.map);
}
//打印关键字表项
void Debuger::PrintKeyEntry(const key_entry& ke)
{
	formatOutput(OUT_NAME_LEN+OUT_EXTENT);
	PrintStr(ke.name);	
	formatOutput(OUT_HASH_LEN);
	PrintHashValue(ke.key);
	std::list<address> lst=ke.holder_lst;
	for(Const_Iterator_hLst iter = lst.begin();iter!=lst.end();iter++){
		PrintAddr(*iter);
		PrintStr("\n");
	}
}
void Debuger::PrintFingerEntry(const finger_entry& fe)
{
	formatOutput(OUT_HASH_LEN);
	PrintHashValue(fe.start_);
	formatOutput(OUT_HASH_LEN);
	PrintHashValue(fe.end_);
	PrintAddr(fe.successor);
	PrintNewLine();
}
void Debuger::PrintDescript(const std::string& des)
{
	PrintStr(des);
}
//打印hash映射值
void Debuger::PrintHashValue(const hash_value& hv)
{
	PrintInt(hv.value_);
}
//打印字符串
void Debuger::PrintStr(const std::string& msg)
{
#ifdef _WIN32_DEBUG
	OutputDebugString(_T(msg.c_str()));
#else 
	cout<<msg;
#endif
}
void Debuger::PrintInt(int v)
{
char temp[STR_TEMP_LEN];
sprintf_s(temp,STR_TEMP_LEN,"%d",v);
#ifdef _WIN32_DEBUG
	OutputDebugStringA(temp);
#else
	cout<<temp;
#endif
}
void Debuger::PrintuInt(unsigned int v)
{
	char temp[STR_TEMP_LEN];
	sprintf_s(temp,STR_TEMP_LEN,"%u",v);
#ifdef _WIN32_DEBUG
	OutputDebugStringA(temp);
#else
	cout<<temp;
#endif
}
//打印换行
void Debuger::PrintNewLine()
{
#ifdef _WIN32_DEBUG
	OutputDebugString(_T("\n"));
#else
	cout<<endl;
#endif
}
void Debuger::formatOutput(int width,char c/* =' ' */)
{
	cout.fill(c);
	cout.width(width);
}