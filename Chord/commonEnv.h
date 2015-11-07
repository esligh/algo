/*Common类
	定义环境
*/
#ifndef CHORD_COMMON_H_
#define CHORD_COMMON_H_
#include "user_define.h"
class CommonEnv
{
public:
	CommonEnv();
	static void set_m(int bz);
	static void set_debuger(bool b);
	static bool get_debuger();
	static int get_m();
	static bool node_equal(const address& a1,const  address& a2);
private:
	static int m; //查询表级数即hash_value的位数
	static bool bdebug ;
};
#endif