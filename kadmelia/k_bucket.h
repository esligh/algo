#ifndef _KADMELIA_K_BUCKET_H__
#define _KADMELIA_K_BUCKET_H__
#include "user_def.h"
/*class K_Bucket define*/
class K_Bucket
{
public:
	K_Bucket();
	K_Bucket(smallInt s);
	~K_Bucket();
	bool AddNeighbourTobucket(const neighbour& ym);		
	bool DelNeighbour(const hash_value& hv);
	bool HasNeighbour(const hash_value& v)const;
	bool HasNeighbourAndMove(const hash_value& v);	
	int CopyBucketToLst(NeighbourLst& lst,size_t k)const;
	 neighbour DelFront();
	const neighbour&get_front()const;
	smallInt get_seq()const;
	smallInt get_size()const;
	range get_range()const;
	const NeighbourLst& get_neighbourlist()const;
	bool get_neighbourlist(NeighbourLst& lst,smallInt k)const;
	//////////////////////////////////////////////////////////////////////////
	void PrintBucket(const nodeinfo& info);
	void printneighbour(const neighbour&,const nodeinfo& info);	
private:
	smallInt				seq; //base zero
	range				range_;
	NeighbourLst	bucket_;
};
#endif