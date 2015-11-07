#include "stdafx.h"
#include "hash.h"
#include <assert.h>

smallInt Hash::hash_bit= debug_size;

void Hash::set_hashbits(smallInt m)
{
	hash_bit =m;
}
smallInt Hash::get_hashbits()
{
	return hash_bit;
}

smallInt Hash::log2(const hash_value& v)
{
	assert(v.value_ >0);
	if(v.value_ == 1)
		return 0;
	hash_value ov;
	ov.value_= v.value_;
	smallInt n =0;
	while(ov.value_>=2){
		++n;
		(ov.value_=(ov.value_>>1));
	}
	return n;
}

hash_value Hash::pow2(smallInt n)
{
	assert(n>=0);
	hash_value hv;
	hv.value_ =1;
	for(smallInt i = 0;i<n;i++){
		hv.value_=hv.value_<<1;
	}
	return hv;
}

bool Hash::isequal(const hash_value& v1,const hash_value& v2)
{
	return v1.value_ == v2.value_;
}
bool Hash::isvalid(const hash_value& v)
{
	return v.value_ != INVALID_HASH;
}

hash_value Hash::distance(const hash_value& v1,const hash_value& v2)
{
	hash_value v;
	v.value_ = v1.value_^ v2.value_;
	return v;
}

hash_value Hash::hash(const std::string& src)
{
	hash_value ret;
	if(src.empty()){
		ret.value_ = INVALID_HASH;
		return ret;
	}
	int sum =0 ;
	int size = 1<<hash_bit;
	size_t len =src.length();
	for(size_t i =0;i<len;i++){
		sum+=(src[i]+i)*7;
	}
	ret.value_ =abs(sum%size);
	return ret;
}

bool Hash::issmall(const hash_value& v1,const hash_value& v2)
{
	return v1.value_<v2.value_;
}
//////////////////////////////////////////////////////////////////////////

 int Hash::abs(int n)
{
	return n<0 ? -n:n;
}