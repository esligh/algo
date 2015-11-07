#ifndef _KADMELIA_ROUTER_H_
#define _KADMELIA_ROUTER_H_
#include "user_def.h"
#include "k_bucket.h"
/*class Router define*/
class Router
{
public:
	Router();
	~Router();
	bool AddBucket(const K_Bucket& bucket);
	K_Bucket& GetBucket(smallInt seq);
	K_Bucket& GetBucket(const hash_value& hv);
	smallInt get_size()const;
	bool AddNeighbour(smallInt seq,const neighbour& someone);
	bool AddNeighbour(const hash_value& distance ,const neighbour& someone);
	//bool AddNeighbour(const neighbour& someone);
	bool NearBy(const hash_value& v,NeighbourLst& lst,size_t k)const;
	size_t GetNeighbourCount()const;
	bool CopyNeighbourTobucket(const hash_value& local,const NeighbourLst& lst);
	bool GetNeightbourByCount(const hash_value& hv,NeighbourLst& lst);
	void CopyAllNeighbourToLst(NeighbourLst&lst)const;
	void CopyFromFirst(NeighbourLst& lst,size_t k)const;
	//////////////////////////////////////////////////////////////////////////
	//debug
	void PrintRouter(const nodeinfo& info);
private:
	std::vector<K_Bucket> v_tbl; //router table
	static smallInt capicity;
};
#endif