#include "stdafx.h"
#include "keyTable.h"
#include "debuger.h"
#include "hash.h"

KeyTable::KeyTable()
{
}
//析构方法
KeyTable::~KeyTable()
{
	NodePointer p=gethead();
	while(p){
		Addr_Lst lst=p->data.holder_lst;
		lst.erase(lst.begin(),lst.end()); //释放list 否则造成内存泄露
		p = p->next;
	}
}
int KeyTable::get_keyTableSize()const 
{
	return getlength();
}
//获取拥有文件hash_value节点列表
int KeyTable::get_holder(const hash_value& hv,Addr_Lst& lst)const
{
	key_entry ke;
	ke.key = hv;
	int i = LocateElem(ke,(KeyTable::IsEqual_HV));
	Status ret = GetElem(i,ke);
	if(ERROR == ret)
		return ERROR;
	lst = ke.holder_lst;
	return OK;
}
//获取拥有文件名称的节点列表
int KeyTable::get_holder(const std::string& fname,Addr_Lst& lst)const
{
	key_entry ke;
	ke.name = fname;
	int i = LocateElem(ke,(KeyTable::IsEqual_NM));
	Status ret = GetElem(i,ke);
	if(ERROR == ret)
		return ERROR;
	lst = ke.holder_lst;
	return OK;
}
//查找hash值指定的表项
key_entry* KeyTable::find_entry(const hash_value& hv)const
{
	NodePointer p=gethead();
	while(p){
		if(Hash::isequal(hv,p->data.key)){
			return &(p->data);
		}
		p = p->next;
	}
	return NULL;
}
//查找文件名称指定的表项
key_entry* KeyTable::find_entry(const std::string& name)const
{
	NodePointer p = gethead();
	while(p){
		if(name == p->data.name){
			return &(p->data);
		}
		p=p->next;
	}
	return NULL;	//not found
}
//添加拥有者
int KeyTable::add_holder(const std::string& fname,const address& addr)
{
		key_entry* pk = find_entry(fname);
		if(pk == NULL)
			return -1;
		if(!has_address(*pk,addr))
			pk->holder_lst.push_back(addr);
		return 1;
}
int KeyTable::add_holder(const hash_value& hv,const address& addr)
{
	key_entry* pk = find_entry(hv);
	if(pk == NULL)
		return -1;
	if(!has_address(*pk,addr)){		//node地址不同
		pk->holder_lst.push_back(addr);
	}
	return 1;
}
//添加表项
int KeyTable::add_keyEntry(const key_entry& ke)
{
	key_entry* pke = find_entry(ke.key);	//依据hash值来查找
	if(pke != NULL)	//如果已存在该表目返回
		return 0;//-1
	Status ret = InsertList(1,ke);	
	return ret;
}	
//打印finger 表
void KeyTable:: PrintKeyTable()
{
	int i=0,m=getlength();
	NodePointer p=gethead();
	Debuger::PrintDescript(std::string("key table:\n"));
	while(p){
		Debuger::PrintKeyEntry(p->data);
		p=p->next;
	}
	std::cout<<endl;
}
//////////////////////////////////////////////////////////////////////////
//判断hash值是否相等
Status KeyTable::IsEqual_HV(const key_entry& ke1,const key_entry& ke2)
{
	return Hash::isequal(ke1.key,ke2.key);
}
//判断文件名称是否相等
Status KeyTable::IsEqual_NM(const key_entry& ke1,const key_entry& ke2)
{
	return ke1.name.compare(ke2.name);
}
//address 是否相等
bool KeyTable::is_equal_address(const address& a1,const address& a2)const 
{
	return (a1.ip == a2.ip && a1.port == a2.port && Hash::isequal(a1.map,a2.map));
}
//是否拥有者列表已存储addr
bool KeyTable::has_address(const key_entry& entry,const address& addr)const
{
	for(Const_Iterator_hLst iter = entry.holder_lst.begin();iter!=entry.holder_lst.end();iter++){
		if(is_equal_address(*iter,addr)){
			return true;
		}
	}
	return false;
}