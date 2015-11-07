#include "stdafx.h"
#include "filedict.h"
#include "common.h"

FileDict::FileDict()
{
}
FileDict::~FileDict()
{
	clear();
}
size_t FileDict::get_size()const
{
	return fdict_.size();
}
bool FileDict::haskey(const hash_value& hv)
{
	
	DICT_CITER pos=fdict_.find(hv); 
	if(pos == fdict_.end())
		return false;
	return true;
}

DICT_CITER FileDict::GetPairKV(const hash_value& key)const
{
	return fdict_.find(key);
}

const Vnode* FileDict::GetDestLst(const hash_value& key)const
{
	DICT_CITER ifind = fdict_.find(key);
	if(ifind != fdict_.end()){
		return &(ifind->second.dest_);
	}
	return NULL;
}
bool FileDict::GetDestLst(const hash_value& key,Vnode& lst)const
{
	DICT_CITER ifind = fdict_.find(key);
	if(ifind == fdict_.end())
		return false;
	lst = ifind->second.dest_;
	return true;
}

bool FileDict::AddHolderDest(const hash_value& key,const destination& addr)
{
	fdict_[key].key_ = key;
	fdict_[key].dest_.push_back(addr);
	return true;
}
bool FileDict::AddPairKV(const pair_kv& pair)
{
	fdict_[pair.key_].key_ = pair.key_;
	fdict_[pair.key_].dest_=pair.dest_;
	return true;
}

bool FileDict::DelHolderDest(const hash_value& key,const destination& addr)
{

	for(Vnode::iterator iter = fdict_[key].dest_.begin();iter!=fdict_[key].dest_.end();iter++)
	{
		if(Common::isequal_dest(addr,*iter)){
			fdict_[key].dest_.erase(iter);
			return true;
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
void FileDict::clear()
{
	for(DICT_ITER iter = fdict_.begin();iter!=fdict_.end();iter++){
		//(iter->second.dest_).erase(iter->second.dest_.begin(),iter->second.dest_.end());
		(iter->second.dest_).clear();
	}
	//fdict_.erase(fdict_.begin(),fdict_.end());
	fdict_.clear();
}
void FileDict::printdict()
{
	for(DICT_ITER iter = fdict_.begin();iter!=fdict_.end();iter++){
		std::cout<<"key="<<(iter->second).key_.value_<<":";
		for(Vnode_Iter it = (iter->second).dest_.begin();it!=(iter->second).dest_.end();it++){
			std::cout<<"("<<it->ip_<<":"<<it->port_<<","<<it->id_.value_<<")";
		}
		std::cout<<std::endl;
	}
}
