#include "stdafx.h"
#include "commonEnv.h"
#include "hash.h"
int CommonEnv::m = 0;
bool CommonEnv::bdebug = false;
void CommonEnv::set_m(int bz)
{
	m = bz;
	Hash::set_hashbits(m);
}
int CommonEnv::get_m()
{
	return m;
}
bool CommonEnv::node_equal(const address& a1,const address& a2)
{
	return (a1.ip == a2.ip && a1.port == a2.port && Hash::isequal(a1.map,a2.map));
}
void CommonEnv::set_debuger(bool b)
{
	bdebug  =b;
}
bool CommonEnv::get_debuger()
{
	return bdebug;
}
