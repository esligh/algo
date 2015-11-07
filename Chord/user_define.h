/*
*/
#ifndef USER_DEFINE_H__
#define  USER_DEFINE_H__
#include <string>
#include<list>
#define  INVALID_PORT					(int)(~0)	//-1
#define  INVALID_MAP_VALUE		(int)(~0)	
//hash值
typedef struct tag_hash_value
{
	int value_; 
}hash_value;
//节点的ip结构
struct address
{
	std::string ip;
	int				port; //端口
	hash_value map;		//ip及端口的hash映射值
	address():
	ip(""),
	port(INVALID_PORT)	
	{
		map.value_=INVALID_MAP_VALUE;
	}
};
//finger表结构
struct finger_entry
{
	hash_value start_ ;
	hash_value end_;
	address successor; // 节点后继
};
//关键字表项结构
typedef std::list<address>							Addr_Lst;
typedef std::list<address>::iterator				Iterator_hLst;
typedef std::list<address>::const_iterator	Const_Iterator_hLst;
struct key_entry
{
	std::string				name;			//文件名	
	hash_value			key;			  //文件关键字	
	Addr_Lst				holder_lst;	 //拥有关键字的节点列表
};
#endif /*DEFINE.H*/