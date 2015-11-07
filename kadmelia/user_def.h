#ifndef _KADMELIA_USER_DEF_H_
#define _KADMELIA_USER_DEF_H_

//data define
typedef unsigned short	uShort;
typedef short					smallInt;
#define  INVALID_PORT		(uShort)(~0)
#define  INVALID_HASH		(int)(~0)

struct hash_value
{
	int value_;
	hash_value():value_(INVALID_HASH){}
	hash_value(int m):value_(m){}
	hash_value& operator=(const hash_value& r){value_ = r.value_; return *this;}
};

typedef struct tag_nodeinfo
{
	std::string		ip_;		//ip address
	uShort			port_;		//port number
	hash_value	id_;			//id
	bool operator ==(const tag_nodeinfo& info){
		return id_.value_ == info.id_.value_;
	}
}nodeinfo;

typedef struct tag_range
{
	hash_value left_;
	hash_value right_;
}range;

typedef nodeinfo neighbour;
typedef neighbour  destination;
typedef std::vector<destination> Vnode;
typedef Vnode::iterator Vnode_Iter;

typedef struct tag_pair_kv
{
	hash_value		key_;
	Vnode				dest_;
}pair_kv;

typedef std::list<neighbour>					NeighbourLst;
typedef NeighbourLst::iterator				NeighbourLst_Iter;
typedef NeighbourLst::const_iterator		NeighbourLst_CIter;

struct lessN
{
	bool operator()(const neighbour& left,const neighbour& right)
	{
		return left.id_.value_<right.id_.value_;
	}
};
typedef std::set<neighbour,lessN> NeightbourSet;
typedef NeightbourSet::iterator NeightbourSet_Iter;
#endif/*USER_DEF*/