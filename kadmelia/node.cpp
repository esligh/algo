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

//���ڷ����ڵ���ӵ�е�<key,value>
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
//�ӱ��ؿ���<key,value>���ڵ�to
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
/*���ڵ�x �յ�һ��rpc ��Ϣʱ��������y ��IP ��ַ�ͱ��������¶�Ӧ��K Ͱ�����岽�����£�
1�������Լ��ͷ����ߵľ��룺d(x,y) = x��y��ע�⣺x ��y ��ID ֵ������IP ��ַ
2��ͨ������d ѡ���Ӧ��K Ͱ���и��²�����
3�����y ��IP ��ַ�Ѿ����������KͰ�У���Ѷ�Ӧ���Ƶ��ø�K Ͱ��β��
4�����y ��IP ��ַû�м�¼�ڸ�K Ͱ��
�������K Ͱ�ļ�¼��С��k ������ֱ�Ӱ�y ��(IP address,UDP port,Node ID)
��Ϣ�������β��
�������KͰ�ļ�¼�����k������ѡ��ͷ���ļ�¼������ǽڵ�z������RPC_PING����
�����z û����Ӧ�����K Ͱ���Ƴ�z����Ϣ������y ����Ϣ�������β��
�����z ����Ӧ�����z ����Ϣ�Ƶ�����β����ͬʱ����y ����Ϣ��   */
/************************************************************************/
bool Node::UpdateRouter(const neighbour& yemi)
{
	hash_value distance = Hash::distance(getlocalhash(),yemi.id_);
	if(Hash::iszero(distance)) //idΪ��ǰ�ڵ㷵��
		return false;
	smallInt seq = Hash::log2(distance);
	K_Bucket& bucket = router_.GetBucket(seq); //��ȡbucket
	if(bucket.HasNeighbourAndMove(yemi.id_))
		return false;
	if(bucket.get_size()<Common::get_k()){//kͰδ��
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
/*̽��һ���ڵ��Ƿ�����*/
bool Node::rpc_ping(const Node& sender)
{
	return isonline();
}
/*�洢<K,V>��*/
bool Node::rpc_store(const Node& sender,const hash_value& key,const destination& dest)
{
	UpdateRouter(sender.getlocalinfo());
	return filedict_.AddHolderDest(key,dest);
}
/*��ѯ��ӽ��ڵ�id��K���ڵ�*/
bool Node::rpc_find_node(const Node& sender,const hash_value& id,NeighbourLst& nodelst)
{	
	//����KͰ
	UpdateRouter(sender.getlocalinfo());
	hash_value distance = Hash::distance(getlocalhash(),id);
	router_.NearBy(distance,nodelst,Common::get_a()); //ע�����صĽڵ����������ڵ� 
	if(Hash::iszero(distance)){ //�ڲ���ĳһ��Դ��Idʱ�����ܴ�idͬ�ڵ�id��ͬ����ʱ��ǰ�ڵ�����ӽ�����Դ�Ľڵ㣬��Ϊ����id����Դid�ľ���Ϊ0
		nodelst.push_back(getlocalinfo());
	}else{//�ڼ���ڵ�ʱ���б����޳�����ڵ�sender,��ʱdistance��Ϊ0����Ϊ������ڵ��id��ȻΨһ,����������ڵ��id���벻����Ϊ0
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
				//������Ҫ�жϽڵ㵱ǰ�Ƿ�����
				if(!pNode->isonline()) 
					continue;				
				bool ret = pNode->get_dict().GetDestLst(key,dest); //��ȡ�б�
				if(ret)
					return true;
			}
		}
		for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
			Node* pNode = NetWork::find(iter->id_);
			if(pNode != NULL){
				//������Ҫ�жϽڵ㵱ǰ�Ƿ�����
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
		//���þ���
		for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
			iter->id_ = Hash::distance(key,iter->id_);
		}
		coll.clear();
		if(!ret) //û�и���
			break; 
	}	
	return false;
}

bool Node::get_closest_neighbour(const hash_value& id,NeighbourLst& lst)
{
	return get_closest_neighbour(*this,id,lst);
}
/*�����ѯ�ڵ�id���������ID��k���ڵ�Ԫ��(�ǵݹ�)*/
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
				//������Ҫ�жϽڵ㵱ǰ�Ƿ�����
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
		//���þ���
		for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
			iter->id_ = Hash::distance(id,iter->id_);
		}
		coll.clear();
		if(!ret) //û�и���
			break; 
	}	
	return true;
}
/*�ӽ����߷��ص�N���ڵ���ѡ�����ѯID��ӽ���k���ڵ㣬������ڸ��ӽ��Ľڵ㷵��true�����򷵻�false*/
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
		if(ifind == temp.end()){ //δ�ҵ� ����
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
 /*����ÿ���յ�RPC��Ϣ�Ľڵ㣬����Ҫ���ݷ��ͷ��Ľڵ���Ϣ������k-bucket*/
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
 //��Kad�����У�ִ��STORE������k���ڵ�ÿСʱ�ط����Լ����е�<key,value>������
 bool Node::share_torrent(std::string& fname)
 {
	hash_value key = Hash::hash(fname);//�����ļ�hashֵ
#ifndef  _KAD_DEBUG
		std::cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<std::endl;
		std::cout<<"&&kad is sharing torrent now,filename="<<"\""<<fname<<"\""
			<<","<<"key="<<key.value_<<"&&"<<std::endl;
		std::cout<<"^~operator node info:"<<getlocalinfo().ip_<<","<<getlocalinfo().id_.value_<<std::endl;
#endif
	NeighbourLst lst;
	get_closest_neighbour(key,lst); //��ȡ����key��ӽ��ĵĽڵ�
#ifndef _KAD_DEBUG
	std::cout<<"get the closest neighbours,the info shows as follow:"<<std::endl;
#endif
	for(NeighbourLst_Iter iter = lst.begin();iter!=lst.end();iter++){
#ifndef _KAD_DEBUG
		std::cout<<iter->ip_<<","<<iter->id_.value_<<std::endl;
#endif
		Node* pNode = NetWork::find(iter->id_); //���ҵ��ڵ�
		if(pNode != NULL){
			pNode->rpc_store(*this,key,getlocalinfo()); //�洢kv��ÿ��һСʱ���·���<key,value>��
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