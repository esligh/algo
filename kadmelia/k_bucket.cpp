#include "stdafx.h"
#include "k_bucket.h"
#include "hash.h"
#include "common.h"

K_Bucket::K_Bucket()
:seq(0)
{
	range_.left_.value_ = INVALID_HASH;
	range_.right_.value_ = INVALID_HASH;
}

K_Bucket::K_Bucket(smallInt s)
:seq(s)
{
	range_.left_ = Hash::pow2(s);
	range_.right_ =Hash::pow2(s+1);
}

K_Bucket::~K_Bucket()
{
	bucket_.clear();
}

bool K_Bucket::HasNeighbour(const hash_value& v)const
{
	if(!Hash::isvalid(v))
		return false;
	for(NeighbourLst_CIter iter = bucket_.begin();iter!=bucket_.end();iter++){
		if(Hash::isequal(v,iter->id_)){
			return true;
		}
	}
	return false;
}
bool K_Bucket::HasNeighbourAndMove(const hash_value& v)
{
	if(!Hash::isvalid(v)){
		return false;
	}
	bool bRet =false;
	neighbour frezzy;
	for(NeighbourLst_Iter iter =bucket_.begin();iter!=bucket_.end();iter++){
		if(Hash::isequal(v,iter->id_)){		
			frezzy = *iter;
			bucket_.erase(iter);
			bRet =true;
			break;
		}
	}
	if(bRet)
		bucket_.push_back(frezzy);
	return bRet;
}

bool K_Bucket::AddNeighbourTobucket(const neighbour& ym)
{
		if(HasNeighbour(ym.id_))
			return false;
		bucket_.push_back(ym);
		return true;
}
neighbour K_Bucket::DelFront()
{
	NeighbourLst_Iter ihead =bucket_.begin();
	neighbour siri = *ihead;
	bucket_.erase(ihead);
	return siri;
}

const neighbour& K_Bucket::get_front()const
{
	return bucket_.front();
}
bool K_Bucket::DelNeighbour(const hash_value& hv)
{
	for(NeighbourLst_CIter iter=bucket_.begin();iter!=bucket_.end();iter++){
		if(Hash::isequal(hv,iter->id_)){
			bucket_.erase(iter);
			return true;
		}
	}
	return false;
}
smallInt K_Bucket::get_seq()const
{
	return seq;
}

range K_Bucket::get_range()const
{
	return range_;
}

const NeighbourLst& K_Bucket::get_neighbourlist()const
{
	return bucket_;
}

bool K_Bucket::get_neighbourlist(NeighbourLst& lst,smallInt k)const
{
	int i=0;
	for(NeighbourLst_CIter iter=bucket_.begin(); i<k &&iter!=bucket_.end();iter++,i++){
		lst.push_back(*iter);
	}
	return (lst.size() !=0);
}
smallInt K_Bucket::get_size()const
{
	return static_cast<smallInt>(bucket_.size());
}

int K_Bucket::CopyBucketToLst(NeighbourLst& lst,size_t k)const
{
	size_t size = bucket_.size();
	for(NeighbourLst_CIter iter = bucket_.begin();iter!=bucket_.end() && k>0;iter++,k--){
		lst.push_back(*iter);
	}
	return k; //返回未拷贝的个数
}

void K_Bucket::PrintBucket(const nodeinfo& info)
{
	 std::fstream& file = Common::get_filehandle();
	 file<<"id\tip\t\tport\tdistance"<<std::endl;
	std::cout<<"id\tip\t\tport\tdistance"<<std::endl;
	for(NeighbourLst_CIter iter = bucket_.begin();iter!=bucket_.end();iter++){
		printneighbour(*iter,info);
		std::cout<<""<<std::endl;
		file<<""<<std::endl;
	}
}
void K_Bucket::printneighbour(const neighbour& e,const nodeinfo& info)
{
	std::fstream& file = Common::get_filehandle();
	std::cout<<e.id_.value_<<"\t"<<e.ip_<<"\t"<<e.port_<<"\t"<<Hash::distance(e.id_,info.id_).value_;
	file<<e.id_.value_<<"\t"<<e.ip_<<"\t"<<e.port_<<"\t"<<Hash::distance(e.id_,info.id_).value_;
}