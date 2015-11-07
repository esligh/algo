#include "stdafx.h"
#include <assert.h>
#include "node.h"
#include "hash.h"
#define  STR_LEN		30
Node::Node()
{
}
Node::~Node()
{
}
Node::Node(address& addr)
{
	hash_node(addr);	//计算本地节点的hash值
	local_node  = addr;
	finger_entry fe;	
	fe.start_.value_ = fe.end_.value_ = INVALID_MAP_VALUE;
	fe.start_.value_= local_node.map.value_;
	finger_tbl.set_finger_entry(0,fe);
	int M = 1<<(get_m());	//获取m值
	int size = finger_tbl.get_finger_size();//获取表大小
	for(int i = 1;i<size;i++){
		finger_entry loop;
		int n = 1<<(i-1);
		int m = 1<<i;
		loop.start_.value_ = (local_node.map.value_+n)%M;
		loop.end_.value_  = (local_node.map.value_+m)%M;
		loop.successor =addr;
		//successor设置默认 见address结构 构造方法
		finger_tbl.set_finger_entry(i,loop);
	}
	predecessor = local_node; //前继节点即为本节点
	bActive = false;//非活动节点
}
//设置其前继节点
void Node::set_predecessor(const address& ad)
{
	predecessor = ad;
}
const address& Node::get_predecessor()const
{
	return predecessor;
}
//是否活动
bool Node::isActive()const
{
	return bActive;
}
//设置活动标记
void Node::setActive(bool b)
{
	bActive =b;
}
//存储关键字表项
bool Node::store_key(const key_entry& ke)
{
	return (key_tbl.add_keyEntry(ke) == OK);
}
const FingerTable& Node::get_finger_table()const
{
	return finger_tbl;
}
hash_value Node::get_hash_value()const
{
	return local_node.map;	
}
void Node::set_successor(const address& ad)
{
	finger_tbl.set_successor(ad);
}
//更新节点的finger表目
//在chord环中ad节点的加入或离开需要更新其finger表
int Node::update_finger_table(const Node& node,NODE_OPER_TYPE type)
{
	if(CommonEnv::node_equal(local_node,node.get_localnode()))
		return 0;
	int size = finger_tbl.get_finger_size();
	hash_value ad_hv = node.get_hash_value();	//加入或离开节点的hash值;
	hash_value local_hv = local_node.map;
	const finger_entry* base = finger_tbl.get_finger();
	assert(base != NULL);
	if(type == ADD_NODE){//节点加入
		for(int i = 1;i<size;i++){
			if(get_finger_table().isneed_change_successor(i,ad_hv)){
				finger_tbl.set_entry_successor(i,node.get_localnode());
			}
		}
	}else if(type == DEL_NODE){//节点离开
		for(int i=1;i<size;i++){
			if(Hash::isequal(ad_hv,base[i].successor.map)){ //如果finger表项的successor是离开节点的hash_value 则需要更新
				address succ = node.get_successor();	//获取其后继的address
				finger_tbl.set_entry_successor(i,succ);	
			}
		}
	}
	return 1;
}
//获取节点的后继
const address& Node::get_successor()const
{
	return finger_tbl.get_successor()->successor;
}
//获取本地节点
const address& Node::get_localnode()const
{
	return local_node;
}
const  KeyTable& Node::get_key_table()const
{
	return key_tbl;
}
//设置索引为Index的finger表项的successor
void Node::set_fingerEntrySuccessor(int index,const address& addr)
{
	finger_tbl.set_entry_successor(index,addr);
}
/*
函数功能：添加一个文件信息到本地节点的keytable中
参数说明:
name: 文件名称
addr: 文件的存储节点地址
注：同一个节点不能共享hash_value值相同的文件
*/
bool Node::add_file(const std::string& name,const address& addr)
{
	key_entry en;
	en.name = name;
	hash_key(name,&en.key);//计算文件hash值
	int ret = key_tbl.add_holder(en.key,addr); //如果已经存在该文件的记录项，则只需添加存储节点地址到keytable中
	if(ret == -1){//没有文件记录
		en.holder_lst.push_back(addr); //add key 
		int  result=key_tbl.add_keyEntry(en); //添加表项
		return (result == 1);
	}
	return true;
}
//从chord环中删除s所指的节点
bool Node::DelKeyEntry(KeyTable::NodePointer s)
{
	if(s==NULL)
		return false;
	return (key_tbl.DeleteList(s) == OK);	
}
/*在节点的keyTable表中获取Key值为hv的存储地址列表*/
/*
hv: key值
lst:存储地址列表
*/
bool Node::find_file(const hash_value& hv,Addr_Lst& lst)
{
	key_entry* pke = key_tbl.find_entry(hv);
	if(pke == NULL)
		return false;
	lst = pke->holder_lst;	//返回存储列表
	return true;
}
/*函数功能：查找存储<K,V>对节点*/
/*参数说明:
key_hv: key值
addr: 查找结果即 存储节点
查找流程:
1.查找Key值是否在本地节点和其后继节点之间
2.查找key值是否在finger表中,如果不在，查询距离key_hv最近的前向节点
3.否则，在finger表目中查找不大于K的最大节点项
注：3中不大于K的节点是指表目中start值处于key的前继而end处于key的后继，
即start在chord环上处于(start,end)之间
*/
bool Node::get_closest_finger(const hash_value& key_hv,address& addr)
{	
	int size = finger_tbl.get_finger_size();
	const finger_entry* pbase= finger_tbl.get_finger();
	assert(pbase != NULL&&size>1);
	hash_value cur_hv = local_node.map; //本地节点hash
	hash_value successor_hv = get_successor().map; //后继节点hash
	if(Hash::between(key_hv,cur_hv,successor_hv)){ //查询key是否在本地节点和其后继之间 
		addr=get_successor();
		return true; //找到存储<K,V>节点
	}else if(!Hash::between(key_hv,pbase[1].start_,pbase[size-1].start_)){ //key_hv不在finger表中，查询离key_hv最近的前向节点
		hash_value min_hv = Hash::get_max();
		hash_value distance;
		int index=0;
		for(int i =1;i<size;i++){
			Hash::distance(pbase[i].successor.map,key_hv,&distance); //计算节点hash与key_hv的距离
			if(Hash::issmall(distance,min_hv)){
				min_hv = distance;
				index = i;
			}
		}
		addr = (pbase[index].successor); //找到key值的最近前向节点
		return false; 
	}else{//查询finger表目 返回key在(start,end)之间的finger[i].successor
		for(int i=2;i<size-1;i++){
			if(Hash::isequal(key_hv,pbase[i].start_)){ //找到key值则直接返回 successor 并返回成功
				addr =pbase[i].successor;
				return true; 
			}
			if(Hash::between(key_hv,pbase[i].start_,pbase[i].end_)){
				addr= pbase[i].successor;
				return false;
			}
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
int Node::hash_node(address& addr)
{
	char temp[STR_LEN];
	sprintf_s(temp,STR_LEN,"%s:%d",addr.ip.c_str(),addr.port);
	Hash::_hash(std::string(temp),&addr.map);
	return 1;
}
int Node::hash_key(const std::string& name,hash_value* phv)
{
	return Hash::_hash(name,phv);
}
//copy construct
Node::Node(const Node& node)
{
	if(this != &node){
		local_node  = node.local_node;
		predecessor = node.predecessor;
		finger_tbl	   = node.get_finger_table();
		key_tbl		   = node.get_key_table();
		bActive		 =node.isActive();
	}
}
Node& Node::operator=(const Node& rhs)
{
	local_node = rhs.local_node;
	predecessor = rhs.local_node;
	copy_finger(rhs.get_finger_table());
	key_tbl = rhs.get_key_table();
	bActive = rhs.isActive();
	return *this;
}
bool Node::copy_finger(const FingerTable& other)
{
	int size =other.get_finger_size();
	for(int i=0;i<size;i++){
		const finger_entry* pfe = other.get_finger_entry(i);
		if(pfe == NULL) 
			return false;
		finger_entry fe;
		fe.start_ = pfe->start_;
		fe.end_ =pfe->end_;
		fe.successor = pfe->successor;
		finger_tbl.set_finger_entry(i,fe);
	}
	return true;
}
//debug 
void Node::PrintFingerTable()
{
	finger_tbl.PrintFingerTable();
}
void Node::PrintKeyTable()
{
	char temp[20];
	sprintf_s(temp,20,"node hash_value=%d\n",get_hash_value().value_);
	Debuger::PrintDescript(temp);
	key_tbl.PrintKeyTable();
}