/*keyTable 类定义，由链表管理:	
*/
#ifndef KEY_TABLE_H_
#define KEY_TABLE_H_
#include "stdafx.h"
#include "LinkList.hpp"
#include "user_define.h"
class KeyTable:public LinkList<key_entry>
{
public:
	KeyTable();
	~KeyTable();
	int get_holder(const std::string& fname,Addr_Lst& lst) const;
	int get_holder(const hash_value& hv,Addr_Lst& lst)const;
	key_entry* find_entry(const std::string& name)const;
	key_entry* find_entry(const hash_value& hv)const;
	int add_holder(const std::string& fname,const address& addr);
	int add_holder(const hash_value&hv,const address& addr);
	int add_keyEntry(const key_entry& ke);
	const address& get_address(const hash_value& ke)const;
	void PrintKeyTable();
	bool has_address(const key_entry& ,const address&)const;
	bool is_equal_address(const address& a1,const address& a2)const;
	int get_keyTableSize()const ;
private:
	
	static Status IsEqual_HV(const key_entry& ke1,const key_entry& ke2); //
	static Status IsEqual_NM(const key_entry& ke1,const key_entry& ke2);
};
#endif/*KEY_TABLE.H*/

