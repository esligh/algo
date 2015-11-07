#ifndef _KADMELIA_HASH_H__
#define _KADMELIA_HASH_H__
#include "user_def.h"
class Hash
{
public:
	static void set_hashbits(smallInt m);
	static smallInt get_hashbits();
	static smallInt log2(const hash_value& );
	static hash_value hash(const std::string& src);
	static hash_value pow2(smallInt n);
	static bool isequal(const hash_value& v1,const hash_value& v2);
	static bool isvalid(const hash_value& v);
	static hash_value distance(const hash_value& v1,const hash_value& v2);
	static bool issmall(const hash_value& v1,const hash_value& v2);
	static bool iszero(const hash_value& v){return v.value_==0;}
	static hash_value get_max()
	{
		hash_value max;
		max.value_ = (1<<get_hashbits());
		return max;
	}
private:
	static int abs(int n);
private:
	static smallInt hash_bit;
	static const smallInt debug_size = 6;
};
#endif