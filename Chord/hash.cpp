#include "stdafx.h"
#include "hash.h"
#include <assert.h>
int Hash::hash_bits = 0;
hash_value Hash::max ;
hash_value Hash::min;
void Hash::set_hashbits(int m)
{
	hash_bits = m;
}
int Hash::get_hashbits()
{
	return hash_bits;
}
//hash函数 测试  在chord中使用SHA-1
int Hash::_hash(const std::string& r,hash_value* va)
{
	if(r.empty())
		return 0;
	int sum =0 ;
	int size = 1<<hash_bits;
	size_t len =r.length();
	for(size_t i =0;i<len;i++){
		sum+=(r[i]+i)*7;
	}
	va->value_ = abs(sum%size);
	return 1;
}
//判断hash值是否相等
bool Hash::isequal(const hash_value& v1,const hash_value& v2)
{
	return (v1.value_ == v2.value_);
}
//判断v1是否大于v2 的hash值
bool Hash::islarge(const hash_value& v1,const hash_value& v2)
{
	return (v1.value_>v2.value_);
}
bool Hash::issmall(const hash_value& v1,const hash_value& v2)
{
	return (v1.value_<v2.value_);
}
//between方法判断一个节点在chord环上是否在两个节点之间
/*
参数说明: v 待判断节点
(v1，v2]节点指定判断的区间即(pre_node,cur_node] 
注:在chord上按顺时针进行判定
*/
bool Hash::between(const hash_value& v,const hash_value& v1,const hash_value& v2)
{
	if(isequal(v,v2)){  //v1与v2相同不考虑
		return true; 
	}else if(islarge(v2,v1)){
		if(islarge(v,v1) && issmall(v,v2)){
			return true;
		}else{
			return false;
		}
	}else{
		if((islarge(v,v1)&& islarge(v,v2)) || issmall(v,v1)&& issmall(v,v2)){
			return true;
		}else{
			return false;
		}
	}
}

const hash_value& Hash::get_max()
{
	max.value_ = (1<<get_hashbits());
	return max;
}
const hash_value& Hash::get_min()
{
	min.value_ =0;
	return min;
}
//计算在chord环上连个节点的距离(按照顺时针方向，即v2为v1的后继) 
void Hash::distance(const hash_value& v1,const hash_value& v2,hash_value* pr)
{
	int mod = 1<<(get_hashbits());
	if(issmall(v1,v2)){
		pr->value_=v2.value_-v1.value_;
	}else if(islarge(v1,v2)){
		pr->value_ = (v2.value_+mod)-v1.value_;
	}else{
		pr->value_ = 0;
	}
}