#include "stdafx.h"
#include "environment.h"

Environment::Environment()
{
}
Environment::Environment(Node& node)
:_ring(node){
}
Environment::~Environment()
{
}

bool Environment::join(const Node& node)
{
	return  _ring.add_node(node);	
}

bool Environment::leave(const Node& node)
{
	address addr = node.get_localnode();
	return _ring.del_node(addr);
}
bool Environment::share_file(const Node&node,const std::string&filename)
{
	address addr = node.get_localnode();
	return _ring.share_file(addr,filename);
}

bool Environment::find_file(const Node& node,const hash_value& hv,Addr_Lst& list)
{
	return _ring.find_file(node,hv,list);
}