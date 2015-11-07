#include "stdafx.h"
#include "node.h"
#include "common.h"
#include "network.h"
Node::Node()
{
}
Node::Node(const nodeinfo& info)
{
	local_.ip_ = info.ip_;	
	local_.port_ = info.port_;
	local_.id_ =hash_node(info);
	bOnline = false;
}

Node::~Node()
{
}

const Vnode* Node::GetFileDestLst(const hash_value& hv)
{
	return filedict_.GetDestLst(hv);
}
bool Node::StorePairKV(const hash_value& key,const destination& value)
{
	return filedict_.AddHolderDest(key,value);
}
bool Node::StorePairKV(const hash_value& key,const Vnode& nodelst)
{
	pair_kv pair;
	pair.key_ = key;
	pair.dest_ = nodelst;
	return filedict_.AddPairKV(pair);
}
bool Node::DelPairKV(const hash_value& key,const destination& value)	
{
	return filedict_.DelHolderDest(key,value);
}

bool Node::AddNeighbour(const hash_value& hv,const neighbour& someone)
{
	return router_.AddNeighbour(hv,someone);
}
bool Node::GetDestLst(const hash_value& key,Vnode& nodelst)
{
	return filedict_.GetDestLst(key,nodelst);
}

//定期发布节点所拥有的<key,value>
bool Node::schedule()
{
#ifndef _KAD_DEBUG
	std::cout<<"..//it's time for node "<<"(ip="<<getlocalinfo().ip_<<",id="<<getlocalhash().value_<<")"\
		<<" republish its key-value// detail info..."<<std::endl;	
#endif
	const DICT dict = filedict_.get_dict();	
	for(DICT_CITER iter = dict.begin();iter!=dict.end();iter++){
		NeighbourLst lst;
		get_closest_neighbour(iter->first,lst); 
#ifndef _KAD_DEBUG
		std::cout<<"file key ="<<iter->first.value_<<" is publishing now."<<std::endl;
		std::cout<<"destination:";
#endif		
		for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
#ifndef _KAD_DEBUG
			std::cout<<"("<<iter->ip_<<","<<iter->id_.value_<<")";
#endif
			Node* pNode = NetWork::find(iter->id_); 
			if(pNode != NULL){
				transer_kv(iter->id_,*pNode);
			}
		}
	}
	return true;
}
//从本地拷贝<key,value>到节点to
bool Node::transer_kv(const hash_value& key,const Node& to)
{
	Node* pto = NetWork::find(to.getlocalhash());
	if(pto == NULL)
		return false;
	Vnode lst;
	GetDestLst(key,lst);
	pto->StorePairKV(key,lst);
	return true;
}
//////////////////////////////////////////////////////////////////////////
const nodeinfo& Node::getlocalinfo()const
{
	return local_;
}
hash_value Node::getlocalhash()const
{
	return local_.id_;
}

size_t Node::getdictsize()const
{
	return filedict_.get_size();
}

smallInt Node::getroutersize()const
{
	return router_.get_size();
}
bool Node::isonline()const
{
	return bOnline;
}
void Node::setonline(bool b)
{
	bOnline =b;
}
hash_value Node::hash_node(const nodeinfo& info)
{
	char temp[128];
	sprintf_s(temp,128,"%s:%d",info.ip_.c_str(),info.port_);
	std::string str(temp);
	return Hash::hash(str);
}
/************************************************************************/
/*当节点x 收到一个rpc 消息时，发送者y 的IP 地址就被用来更新对应的K 桶，具体步骤如下：
1．计算自己和发送者的距离：d(x,y) = x⊕y，注意：x 和y 是ID 值，不是IP 地址
2．通过距离d 选择对应的K 桶进行更新操作。
3．如果y 的IP 地址已经存在于这个K桶中，则把对应项移到该该K 桶的尾部
4．如果y 的IP 地址没有记录在该K 桶中
⑴如果该K 桶的记录项小于k 个，则直接把y 的(IP address,UDP port,Node ID)
信息插入队列尾部
⑵如果该K桶的记录项大于k个，则选择头部的记录项（假如是节点z）进行RPC_PING操作
①如果z 没有响应，则从K 桶中移除z的信息，并把y 的信息插入队列尾部
②如果z 有响应，则把z 的信息移到队列尾部，同时忽略y 的信息。   */
/************************************************************************/
bool Node::UpdateRouter(const neighbour& yemi)
{
	hash_value distance = Hash::distance(getlocalhash(),yemi.id_);
	if(Hash::iszero(distance)) //id为当前节点返回
		return false;
	smallInt seq = Hash::log2(distance);
	K_Bucket& bucket = router_.GetBucket(seq); //获取bucket
	if(bucket.HasNeighbourAndMove(yemi.id_))
		return false;
	if(bucket.get_size()<Common::get_k()){//k桶未满
		bucket.AddNeighbourTobucket(yemi);
		return true;
	}
	neighbour luzy = bucket.get_front(); 
	Node* pNode = NetWork::find(luzy.id_);
	if(pNode == NULL)
		return false;
	bool result = pNode->rpc_ping(*this); //ping it(pNode)
	if(!result){ // no response
		bucket.DelFront();
		bucket.AddNeighbourTobucket(yemi);
		return true;
	}
	neighbour siri = bucket.DelFront();
	bucket.AddNeighbourTobucket(siri);
	return true;
}
//////////////////////////////////////////////////////////////////////////
/*探测一个节点是否在线*/
bool Node::rpc_ping(const Node& sender)
{
	return isonline();
}
/*存储<K,V>对*/
bool Node::rpc_store(const Node& sender,const hash_value& key,const destination& dest)
{
	UpdateRouter(sender.getlocalinfo());
	return filedict_.AddHolderDest(key,dest);
}
/*查询最接近节点id的K个节点*/
bool Node::rpc_find_node(const Node& sender,const hash_value& id,NeighbourLst& nodelst)
{	
	//更新K桶
	UpdateRouter(sender.getlocalinfo());
	hash_value distance = Hash::distance(getlocalhash(),id);
	router_.NearBy(distance,nodelst,Common::get_a()); //注：返回的节点包含了请求节点 
	if(Hash::iszero(distance)){ //在查找某一资源的Id时，可能此id同节点id相同，这时当前节点是最接近该资源的节点，因为它的id与资源id的距离为0
		nodelst.push_back(getlocalinfo());
	}else{//在加入节点时从列表中剔除加入节点sender,此时distance不为0，因为所加入节点的id必然唯一,跟它所请求节点的id距离不可能为0
		NeighbourLst_Iter ifind = find(nodelst.begin(),nodelst.end(),sender.getlocalinfo());
		if(ifind != nodelst.end()){
			nodelst.erase(ifind);
		}
	}
	return true;
}
bool Node::find_key(const std::string& name,Vnode& nodelst)
{
	hash_value key = Hash::hash(name);
	return find_key(key,nodelst);
}
bool Node::find_key(const hash_value& key,Vnode& nodelst)
{
	return rpc_find_value(*this,key,nodelst);
}
bool Node::rpc_find_value(const Node& node,const hash_value& key,Vnode& dest)
{
	UpdateRouter(node.getlocalinfo());
	NeighbourLst lst;
	rpc_find_node(node,key,lst);
	NeightbourSet coll;
	NeighbourLst tempLst;
	while(1){
		bool ret = false;
		for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
			Node* pNode = NetWork::find(iter->id_);
			if(pNode != NULL){
				//这里需要判断节点当前是否在线
				if(!pNode->isonline()) 
					continue;				
				bool ret = pNode->get_dict().GetDestLst(key,dest); //获取列表
				if(ret)
					return true;
			}
		}
		for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
			Node* pNode = NetWork::find(iter->id_);
			if(pNode != NULL){
				//这里需要判断节点当前是否在线
				if(!pNode->isonline()) 
					continue;
				pNode->rpc_find_node(node,key,tempLst);
			}
			for(NeighbourLst_Iter it = tempLst.begin();it!=tempLst.end();it++){
				it->id_ = Hash::distance(key,it->id_);
				coll.insert(*it);
			}
			iter->id_ = Hash::distance(key,iter->id_);
			coll.insert(*iter);
			tempLst.clear();
		}
		ret = get_closer(coll,lst);
		//重置距离
		for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
			iter->id_ = Hash::distance(key,iter->id_);
		}
		coll.clear();
		if(!ret) //没有更新
			break; 
	}	
	return false;
}

bool Node::get_closest_neighbour(const hash_value& id,NeighbourLst& lst)
{
	return get_closest_neighbour(*this,id,lst);
}
/*请求查询节点id，返回最近ID的k个节点元素(非递归)*/
bool Node::get_closest_neighbour(const Node& node,const hash_value& id,NeighbourLst& lst)
{
	rpc_find_node(node,id,lst);
	NeightbourSet coll;
	NeighbourLst tempLst;
	while(1){
		bool ret = false;
		for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
			Node* pNode = NetWork::find(iter->id_);
			if(pNode != NULL){
				//这里需要判断节点当前是否在线
				if(!pNode->isonline()) 
					continue;
				pNode->rpc_find_node(node,id,tempLst);
			}
			for(NeighbourLst_Iter it = tempLst.begin();it!=tempLst.end();it++){
				it->id_ = Hash::distance(id,it->id_);
				coll.insert(*it);
			}
			iter->id_ = Hash::distance(id,iter->id_);
			coll.insert(*iter);
			tempLst.clear();
		}
		ret = get_closer(coll,lst);
		//重置距离
		for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
			iter->id_ = Hash::distance(id,iter->id_);
		}
		coll.clear();
		if(!ret) //没有更新
			break; 
	}	
	return true;
}
/*从接受者返回的N个节点中选择离查询ID最接近的k个节点，如果存在更接近的节点返回true，否则返回false*/
bool Node::get_closer(NeightbourSet& col,NeighbourLst& lst)
{
	if(col.size() == 0)
		return false;	
	bool bUpdate = false;
	int n = static_cast<int>(lst.size());
	NeightbourSet temp;
	int i=0;
	int limit = Common::get_a();
	for(NeightbourSet_Iter iter = col.begin();iter!=col.end()&&i<limit;iter++,i++){
		temp.insert(*iter);
	}
	for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
		NeightbourSet_Iter ifind = temp.find(*iter);
		if(ifind == temp.end()){ //未找到 更新
			bUpdate = true;
			break;
		}
	}
	if(n<limit || bUpdate){
		lst.clear();
		for(NeightbourSet_Iter iter = temp.begin();iter!=temp.end();iter++){
			lst.push_back(*iter);
		}
	}
	return bUpdate;
}
 /*对于每次收到RPC消息的节点，都需要根据发送方的节点信息，更新k-bucket*/
 bool Node::update(const nodeinfo& node)
 {
	 hash_value distance = Hash::distance(node.id_,getlocalhash());
	 bool ret = router_.AddNeighbour(distance,node);
	 Router router = get_router();
	 return ret;
 }
 //
 bool Node::CopyNeighbourLst(const NeighbourLst& lst)
 {
	return  router_.CopyNeighbourTobucket(getlocalhash(),lst);
 }
 bool Node::GetNeightbourByCount(const hash_value& id,NeighbourLst& lst)
 {
	 hash_value d = Hash::distance(id,local_.id_);
	 return router_.GetNeightbourByCount(d,lst);
 }
 //在Kad网络中，执行STORE操作的k个节点每小时重发布自己所有的<key,value>对数据
 bool Node::share_torrent(std::string& fname)
 {
	hash_value key = Hash::hash(fname);//计算文件hash值
#ifndef  _KAD_DEBUG
		std::cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<std::endl;
		std::cout<<"&&kad is sharing torrent now,filename="<<"\""<<fname<<"\""
			<<","<<"key="<<key.value_<<"&&"<<std::endl;
		std::cout<<"^~operator node info:"<<getlocalinfo().ip_<<","<<getlocalinfo().id_.value_<<std::endl;
#endif
	NeighbourLst lst;
	get_closest_neighbour(key,lst); //获取距离key最接近的的节点
#ifndef _KAD_DEBUG
	std::cout<<"get the closest neighbours,the info shows as follow:"<<std::endl;
#endif
	for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
#ifndef _KAD_DEBUG
		std::cout<<iter->ip_<<","<<iter->id_.value_<<std::endl;
#endif
		Node* pNode = NetWork::find(iter->id_); //查找到节点
		if(pNode != NULL){
			pNode->rpc_store(*this,key,getlocalinfo()); //存储kv，每隔一小时从新发布<key,value>对
		}
	}
	return true;
 }
 //////////////////////////////////////////////////////////////////////////
 bool compare(const neighbour& left,const neighbour& right)
 {
	 return Hash::issmall(left.id_,right.id_);
 }
 void Node::PrintRouter()
 {
	 std::cout<<"**************************************"<<std::endl;
	 std::cout<<"node info:"<<"ip="<<getlocalinfo().ip_<<",port=" \
		 <<getlocalinfo().port_<<",id="<<getlocalinfo().id_.value_<<std::endl;
	 std::fstream& file = Common::get_filehandle();
	 file<<"**************************************"<<std::endl;
	 file<<"node info:"<<"ip="<<getlocalinfo().ip_<<",port=" \
		 <<getlocalinfo().port_<<",id="<<getlocalinfo().id_.value_<<std::endl;

	 router_.PrintRouter(getlocalinfo());
 }
 void Node::PrintDict()
 {
	 std::cout<<"######################################"<<std::endl;
	 std::cout<<"file dictionary,node info:"<<"ip="<<getlocalinfo().ip_<<",port=" \
		 <<getlocalinfo().port_<<",id="<<getlocalinfo().id_.value_<<std::endl;
	 filedict_.printdict();
 }