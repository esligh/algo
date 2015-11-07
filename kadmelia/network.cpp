#include "stdafx.h"
#include "network.h"
#include <algorithm>
NET NetWork::net_;

NetWork::NetWork()
{
}
NetWork::NetWork(const Node& node)
{
	net_.push_back(node);
}
NetWork::~NetWork()
{
		net_.clear();
}
/*********************************************************************** */
/*  节点加入																											 */
/************************************************************************/
bool NetWork::join(Node& node)
{
	if(ishas(node)){
		std::cout<<"node info="<<node.getlocalinfo().ip_ \
			<<" has joined in the nework,please do not try again."<<std::endl;
		return false;
	}
	//随机获取一个Node节点 w 这里取头结点
	Node& w = net_.front();
	node.UpdateRouter(w.getlocalinfo());//首先将w节点放入自己的桶中
	NeighbourLst lst;
	w.get_closest_neighbour(node,node.getlocalhash(),lst); //请求w对自身节点的定位
	node.CopyNeighbourLst(lst); //拷贝list到node K-桶中
	node.setonline(true);
	net_.push_back(node);
	return true;
}

bool NetWork::leave(const Node& node)
{
	Node* pNode = find(node.getlocalhash());
	if(pNode != NULL){
		pNode->setonline(false);
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
Node* NetWork::find(const hash_value& hv)
{
	for(NET_ITER iter = net_.begin();iter!=net_.end();iter++){
		if(Hash::isequal(iter->getlocalhash(),hv)){
			return &(*iter);
		}
	}
	return NULL;
}

bool NetWork::ishas(const Node&node)
{
	for(NET_ITER iter = net_.begin();iter!=net_.end();iter++){
		if(Hash::isequal(node.getlocalhash(),iter->getlocalhash()))
			return true;
	}
	return false;
}
Node* NetWork::RandContact()
{
	size_t size =net_.size();
	srand(unsigned int(0));
	size_t step= rand()%size;
	size_t i =0;
	for(NET_ITER iter = net_.begin();iter!=net_.end();iter++,i++){
		if(i==step)
			return &(*iter);
	}
	return NULL;
}
bool NetWork::find_kv(const Node& node,const hash_value& key,Vnode& lst)
{
	std::cout<<"//////////////////////////////////////////////////////////////////"<<std::endl;
	Node* pNode = find(node.getlocalhash());
	if(pNode== NULL)
		return false;
	std::cout<<"node ip="<<pNode->getlocalinfo().ip_<<" find file key="<<key.value_<<" now.please wait answer."<<std::endl;
	bool ret= pNode->find_key(key,lst);
#ifndef _KAD_DEBUG
	if(ret){	
		std::cout<<"@yeah,we find it ! destination list info:"<<std::endl;
		for(Vnode_Iter iter  = lst.begin();iter!=lst.end();iter++){
			std::cout<<iter->ip_<<":"<<iter->port_<<std::endl;
		}
	}else{
		std::cout<<"sorry, cannot find it!"<<std::endl;
	}
#endif
	return ret;
}
bool NetWork::find_kv(const Node& node,const std::string& name,Vnode&lst)
{
	std::cout<<"//////////////////////////////////////////////////////////////////"<<std::endl;
	Node* pNode = find(node.getlocalhash());
	if(pNode == NULL)
		return false;
	std::cout<<"node ip="<<pNode->getlocalinfo().ip_<<" find file "<<name<<" key="<<Hash::hash(name).value_ \
		<<" now.please wait answer."<<std::endl;
	bool ret=pNode->find_key(name,lst);
#ifndef _KAD_DEBUG
	if(ret){		
		std::cout<<"@yeah,we find it ! destination list info:"<<std::endl;
		for(Vnode_Iter iter  = lst.begin();iter!=lst.end();iter++){
			std::cout<<iter->ip_<<":"<<iter->port_<<std::endl;
		}
	}else{
		std::cout<<"sorry, cannot find it!"<<std::endl;
	}
#endif
	return ret;
}

//////////////////////////////////////////////////////////////////////////

void NetWork::PrintFinger()
{
	for(NET_ITER iter = net_.begin();iter!=net_.end();iter++){
		iter->PrintRouter();
	}
}

void NetWork::PrintNodeDict(const Node&node)
{
	Node* pNode = find(node.getlocalhash());
	if(pNode != NULL){
		pNode->PrintDict();
	}
}