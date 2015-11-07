#ifndef _KAD_INTERFACE_RPC_H_
#define  _KAD_INTERFACE_RPC_H_
#include "user_def.h"
class Node;
/*inferface define inherit by node*/
struct interface_rpc
{
	virtual bool rpc_ping(const Node& sender)=0;
	virtual bool rpc_store(const Node& sender,const hash_value& key,const destination& dest)=0;
	virtual bool rpc_find_value(const Node& sender,const hash_value& key,Vnode& dest) =0;
	virtual bool rpc_find_node(const Node& sender,const hash_value& id,NeighbourLst& nodelst)= 0;
};
#endif