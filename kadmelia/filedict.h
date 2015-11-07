#ifndef _KADMELIA_FILEDICT_CLASS_DEF_H__
#define _KADMELIA_FILEDICT_CLASS_DEF_H__
#include "user_def.h"
#include "hash.h"

/*ÎÄ¼þË÷Òý×Öµä*/
struct lessX
{
	bool operator()(const hash_value& v1,const hash_value& v2)
	{
		return Hash::issmall(v1,v2);
	}
};

typedef std::map<hash_value,pair_kv,lessX>	DICT;
typedef DICT::iterator							DICT_ITER;
typedef DICT::const_iterator					DICT_CITER;
class FileDict
{
public:
	FileDict();
	~FileDict();
	bool haskey(const hash_value&);
	size_t get_size()const;
	DICT_CITER GetPairKV(const hash_value& )const;
	const Vnode* GetDestLst(const hash_value& )const;
	bool GetDestLst(const hash_value& ,Vnode&)const;
	bool AddHolderDest(const hash_value& key,const destination& addr);
	bool DelHolderDest(const hash_value& key,const destination& addr);
	bool AddPairKV(const pair_kv& pair);
	const DICT& get_dict()const{return fdict_;}
	//////////////////////////////////////////////////////////////////////////
	void printdict();
private:
	void clear();
private:
	DICT fdict_;
};
#endif