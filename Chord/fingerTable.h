/*finger Table类定义
*/
#ifndef FINGER_TABLE_H_
#define FINGER_TABLE_H_
#include "stdafx.h"
#include "LinkList.hpp"
#include "user_define.h"
#include "debuger.h"
class FingerTable
{
public:
	FingerTable();
	FingerTable(const FingerTable& other);
	~FingerTable();
	void Init_finger();//初始化finger表
	int get_finger_size()const;
	void set_finger_entry(int i,finger_entry& fe); //根据索引设置finger表项
	int set_finger_entry(hash_value& va,finger_entry& fe);//根据hash_value设置表项 并返回状态	
	const finger_entry* get_finger()const;	//获取finger表
	const finger_entry* get_finger_entry(int i)const; //根据索引获取finger表项
	const finger_entry* get_finger_entry(const hash_value& start)const; //根据hash_value值寻找finger表项	
	const finger_entry* get_successor()const;
	void set_successor(const address& ad);
	void set_entry_successor(const hash_value& start,const address& ad); //设置某一个表项的节点后继
	void set_entry_successor(int index,const address& ad);
	//const finger_entry* get_closest_finger_entry(hash_value& hv)const;  //获取距离hash_value最近的表项		
	void PrintFingerTable()const;
	bool isneed_change_successor(int index,const hash_value& hv)const;
private:
	void init_finger_entry(finger_entry& fe);
private:
	finger_entry* pbase; //finger表基址
};
#endif

