#include "stdafx.h"
#include "router.h"
#include "common.h"
#include "hash.h"

smallInt Router::capicity =Hash::get_hashbits();

Router::Router()
	:v_tbl(capicity)
{
}

Router::~Router()
{
	v_tbl.clear();
}
smallInt Router::get_size()const
{
	return static_cast<smallInt> (v_tbl.size());
}
K_Bucket& Router::GetBucket(smallInt seq)
{
	assert(seq>=0 && seq<capicity);
	return v_tbl[seq];
}
K_Bucket& Router::GetBucket(const hash_value& hv)
{
	assert(Hash::isvalid(hv));
	smallInt seq = Hash::log2(hv);
	return v_tbl[seq];
}
bool Router::AddBucket(const K_Bucket& bucket)
{
	if((smallInt)v_tbl.size()>=capicity)
		return false;
	smallInt seq = bucket.get_seq();
	v_tbl[seq] = bucket;
	return true;
}

bool Router::AddNeighbour(smallInt seq,const neighbour& someone)
{
	if(seq<0 || seq>capicity-1)
		return false;
	K_Bucket& bucket = GetBucket(seq);
	bucket.AddNeighbourTobucket(someone);
	return true;
}
bool Router::AddNeighbour(const hash_value& distance ,const neighbour& someone)
{
	if(!Hash::isvalid(distance))
		return false;
	K_Bucket& bucket =GetBucket(distance);
	bucket.AddNeighbourTobucket(someone);
	return true;
}

size_t Router::GetNeighbourCount()const
{
	size_t count = 0;
	for(std::vector<K_Bucket>::const_iterator iter=v_tbl.begin();iter!=v_tbl.end();iter++){
		count+=iter->get_size();
	}
	return count;
}
/*找到接近节点距离为v的k个节点*/
bool Router::NearBy(const hash_value& v,NeighbourLst& lst,size_t k)const
{
	if(!Hash::isvalid(v))
		return false;
	if(Hash::iszero(v)){
		CopyFromFirst(lst,k-1);//除了本节点获取K-1个
		return true;
	}
	int seq = Hash::log2(v);
	size_t curbuck_count = v_tbl[seq].get_size();	
	if(curbuck_count>=k){ //如果当前桶中节点数>=k
		v_tbl[seq].CopyBucketToLst(lst,k);
		return true;
	}	
	size_t count = GetNeighbourCount();//获取所有bucket中的节点数
	if(count<=k){ //copy all
		CopyAllNeighbourToLst(lst);
		return true;
	}else{
		int up,down;
		up=down=seq;
		size_t left =v_tbl[seq].CopyBucketToLst(lst,k); 
		while(left>0){
			--up;
			if(up>=0)
				left=v_tbl[up].CopyBucketToLst(lst,left);
			else
				break;
		}
		while(left>0){
			++down;
			if(down<capicity){
				left = v_tbl[down].CopyBucketToLst(lst,left);
			}else{
				break;
			}
		}
	}
	return true;	
}
//拷贝桶中所有节点到列表
void Router::CopyAllNeighbourToLst(NeighbourLst&lst)const 
{
	for(int i=0;i<capicity;i++){
		const NeighbourLst& list = v_tbl[i].get_neighbourlist();
		for(NeighbourLst_CIter iter=list.begin();iter!=list.end();iter++){
			lst.push_back(*iter);
		}			
	}
}
void Router::CopyFromFirst(NeighbourLst& lst,size_t k)const
{
	size_t count = GetNeighbourCount();//获取所有bucket中的节点数
	if(count<=k){ //copy all
		CopyAllNeighbourToLst(lst);
		return;
	}
	int down=0;
	size_t left =v_tbl[down].CopyBucketToLst(lst,k); 
	while(left>0){
		++down;
		if(down<capicity){
			left = v_tbl[down].CopyBucketToLst(lst,left);
		}else{
			break;
		}
	}
	return ;
}
/************************************************************************/
/*  拷贝List中的neighbour到相应的K-Bucket中 此方法在节点
	刚加入时调用
*/
/************************************************************************/
bool Router::CopyNeighbourTobucket(const hash_value& local,const NeighbourLst& lst)
{
	for(NeighbourLst_CIter iter = lst.begin();iter!=lst.end();iter++){
		hash_value d = Hash::distance(iter->id_,local);
		if(Hash::iszero(d)) //except node itself
			continue;
		smallInt seq = Hash::log2(d);
		v_tbl[seq].AddNeighbourTobucket(*iter);
	}
	return true;
}
bool Router::GetNeightbourByCount(const hash_value& distance,NeighbourLst& lst)
{
	int seq = Hash::log2(distance);
	bool ret =v_tbl[seq].get_neighbourlist(lst,Common::get_a());
	return ret;
}
//////////////////////////////////////////////////////////////////////////

void Router::PrintRouter(const nodeinfo& info)
{
	 std::cout<<"/Router Info.../"<<std::endl;
	 smallInt size = capicity;
	 std::fstream& file = Common::get_filehandle();
	 file<<"/Router Info.../"<<std::endl;

	for(int i=0;i<size;i++){
		std::cout<<"seq:"<<i<<"\t";
		std::cout<<"range:"<<"["<<Hash::pow2(i).value_<<","<<Hash::pow2(i+1).value_<<")"<<std::endl;
		file<<"seq:"<<i<<"\t";
		file<<"range:"<<"["<<Hash::pow2(i).value_<<","<<Hash::pow2(i+1).value_<<")"<<std::endl;
		v_tbl[i].PrintBucket(info);
		std::cout<<std::endl;		
		file<<std::endl;
	}
}