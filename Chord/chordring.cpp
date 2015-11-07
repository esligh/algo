#include "stdafx.h"
#include "chordring.h"
#include "hash.h"
ChordRing::ChordRing()
{
}
//构造方法 即第一个节点加入到网络中
ChordRing::ChordRing(Node& node)
{
	int length = getlength();
	node.setActive(true);
	InsertList(length+1,node,NULL); //插入该节点到chord环中	
}
//查找节点是否已经在chord环中
bool ChordRing::has_node(const Node& node)
{
	NodePointer head  = Gethead();
	if(head == NULL)
		return NULL;
	hash_value hfind = node.get_hash_value();	
	NodePointer p=head;
	while(head!=p->next){
		if(Hash::isequal(p->data.get_hash_value(),hfind))
			return true;
		p=p->next;	
	}
	if(Hash::isequal(p->data.get_hash_value(),hfind))
		return true;
	return false;
}
/*加入节点*/
/*
	(1)初始化本地节点finger表：
	(2)更新其他节点finger表：
	(3)转移关键字信息
*/
bool ChordRing::add_node(const Node& node)
{
	if(has_node(node))
		return false;		//已经存在chord环中				
	int len = getlength();
	NodePointer pp=NULL;
	InsertList(len+1,node,&pp);	//插入节点到chord环中并返回指向该节点的指针pp
	if(pp == NULL)
		return false;
	update_local_finger(pp->data); 
	update_others_finger(node,Node::ADD_NODE);
	//重置后继节点的前继
	Node* successor = find_successor(node.get_hash_value());		//找到后继
	if(successor == NULL)
		return false;
	address predecessor = successor->get_predecessor();	//找到后继的原有前继
	if(Hash::between(node.get_hash_value(),successor->get_hash_value(),predecessor.map)){
		successor->set_predecessor(node.get_localnode());
	}
	Node* pPrecessor =find_predeccessor(node);	//找到加入节点的前继
	if(pPrecessor == NULL)
		return false;
	//设置当前节点的前继	
	(pp->data).set_predecessor(pPrecessor->get_localnode());
	(pp->data).setActive(true);
	//转移关键字信息
	update_keyTable(pp->data);
	return true;
}
//更新本地节点的finger表目
/*
	注:该方法使用参数为引用类型，是因为需要改变该节点内部的finger表目
*/
bool ChordRing::update_local_finger(Node& node)	
{
	int size = node.get_finger_table().get_finger_size();
	Node* succ_node=NULL;
	address addr ;
	hash_value hv = node.get_finger_table().get_finger_entry(1)->start_;
	succ_node=find_closest_successor(hv);	
	if(succ_node != NULL){
		addr = succ_node->get_localnode();
		node.set_successor(addr);	//设置node后继
	}
	for(int i=2;i<size;i++){
			hash_value hv = node.get_finger_table().get_finger_entry(i)->start_;
			succ_node = find_closest_successor(hv);
			addr = succ_node->get_localnode();
			node.set_fingerEntrySuccessor(i,addr);
	}
	return true;
}
//通知所有的其他节点node加入或者离开
/*
参数说明:
node : 加入后者离开节点
typde:类型标记
*/
bool ChordRing::update_others_finger(const Node& node,Node::NODE_OPER_TYPE type)
{
	//对所有节点更新
	NodePointer head = Gethead();
	NodePointer p = head;
	while(head!=p->next){		
		(p->data).update_finger_table(node,type);
		p = p->next;
	}
	(p->data).update_finger_table(node,type);
	return true;
}
/*
//查找节点的后继
Node* ChordRing::find_successor(const Node& node)const
{
	if(getlength()<=0)
		return NULL;
	Node* assist = Gethead()->data;	//协助节点 节点未加入前 协助节点可以是环中的任何节点
	hash_value cur_hv = node.get_hash_value();//当前节点的hash_value值
}
*/
/*
//辅助查找节点后继
const Node* ChordRing::aux_find(const hash_value& hv,const Node& node,int size)
{
	if(Hash::issmall(hv,node.get_hash_value())){
		return NULL;
	}
	for(int i=1;i<size;i++){
		if()
	}
}
//环中hash_valude为hv的节点
const Node* ChordRing::find(const hash_value& hv)
{
	NodePointer head = Gethead();
	if(head == NULL)
		return NULL;
	NodePointer p  = head;
	while(head != p->next){
		if(Hash::isequal(hv,(p->data).get_hash_value())){
			return &(p->data);
		}
		p=p->next;
	}
	if(Hash::isequal(hv,(p->data).get_hash_value())){
		return &(p->data);
	}
	return NULL;	//未查找到
}
*/
//在chord环上查找顺时针距离cur_hv最近的节点(查找结果可能为当前查找节点本身)
Node* ChordRing::find_closest_successor(const hash_value& cur_hv)const
{
	//O(N) 效率低 改进算法可以参考key的查找即get_closest_finger

	/*从头结点开始查找 这里可以随机指定一个节点，相当于在网络环境中
	随机选择一个节点让其帮助查询其后继或前继节点
	*/
	NodePointer head = Gethead(); 
	if(head == NULL)
		return NULL;
	NodePointer p  = head;
	NodePointer p2f = NULL;			//指向查找到的后继节点
	hash_value min = Hash::get_max();//hash_value的无穷大值
	hash_value d,roop;
	while(head != p->next){
		roop=(p->data).get_hash_value();
		Hash::distance(cur_hv,roop,&d);		//计算当前节点与环中节点的顺时针距离
		if(Hash::issmall(d,min)){
			min = d;
			p2f = p;
		}
		p=p->next;
	}
	roop = (p->data).get_hash_value();
	Hash::distance(cur_hv,roop,&d);
	if(Hash::issmall(d,min)){
		min=d;
		p2f =p;
	}
	if(Hash::isequal(min,Hash::get_max())){ //没有找到
		return NULL;
	}
	return &(p2f->data);
}
//查找hash_value指定节点的后继(结果不能为当前查找节点)
/*return : 
成功 返回节点的后继 
失败返回 NULL*/
Node* ChordRing::find_successor(const hash_value& cur_hv)const
{//同上
	NodePointer head = Gethead(); //从头结点开始查找
	if(head == NULL)
		return NULL;
	NodePointer p  = head;
	NodePointer p2f = NULL;			//指向查找到的后继节点
	hash_value min = Hash::get_max();//hash_value的无穷大值
	hash_value d,roop;
	while(head != p->next){
		roop=(p->data).get_hash_value();
		Hash::distance(cur_hv,roop,&d);		//计算当前节点与环中节点的顺时针距离
		if(Hash::get_hash(d)!=0&&Hash::issmall(d,min)){
			min = d;
			p2f = p;
		}
		p=p->next;
	}
	roop = (p->data).get_hash_value();
	Hash::distance(cur_hv,roop,&d);
	if(Hash::get_hash(d)!=0&&Hash::issmall(d,min)){
		min=d;
		p2f =p;
	}
	if(Hash::isequal(min,Hash::get_max())){ //没有找到
		return NULL;
	}
	return &(p2f->data);
}
//查找某节点的前继
 Node* ChordRing::find_predeccessor(const Node& node)const
{//同上
	NodePointer head = Gethead();
	if(head == NULL)
		return NULL;
	hash_value cur_hv = node.get_hash_value();
	NodePointer p = head;
	NodePointer p2f = NULL;
	hash_value max = Hash::get_min();
	hash_value d,roop;
	while(head != p->next){
		roop=(p->data).get_hash_value();
		Hash::distance(cur_hv,roop,&d);		//计算当前节点与环中节点的顺时针距离
		if(Hash::islarge(d,max)){
			max = d;
			p2f = p;
		}
		p=p->next;
	}
	roop = (p->data).get_hash_value();
	Hash::distance(cur_hv,roop,&d);
	if(Hash::islarge(d,max)){
		max=d;
		p2f =p;
	}
	if(Hash::isequal(max,Hash::get_min())){ //没有找到
		return NULL;
	}
	return &(p2f->data);
}
 /*更新keytable*/
 /*
	说明:节点node加入后，即向其后继a请求关键字，因为归属于node节点的关键字
	必然存在于其后继节点
	node: 新加入节点(引用)
 */
 bool ChordRing::update_keyTable(Node& node)
 {	 
	 hash_value cur_hv = node.get_hash_value();
	 Node* successor = find_successor(cur_hv); //查找后继节点
	 if(successor == NULL)
		 return false;
	 return transfer_key(successor,&node); //从后继转移关键字表项到加入节点
 }
 /*
 函数功能：将满足条件的keyentry项从本地节点转移到新加入节点
 successor: 后继节点
 new_node:新加入节点
 */
 bool ChordRing::transfer_key(Node* successor,Node* new_node)
 {
	// Node* new_node = find(node->get_hash_value());	//在环上找到加入节点
	 hash_value new_hv = new_node->get_hash_value();//获取加入节点的hash_value值
	 KeyTable::NodePointer head =successor->get_key_table().gethead();	//获取后继keyTable指针
	 if(head== NULL) 
		 return false;
	 KeyTable::NodePointer p = head;
	 KeyTable::NodePointer q= head;
	 const address new_pre_addr=new_node->get_predecessor(); //获取新加入节点的前继
	 hash_value new_pre_hv = new_pre_addr.map; //后去前继节点的hash值
	 while(p){		 
		 hash_value key = p->data.key;		
		 if(Hash::between(key,new_pre_hv,new_hv)){ //满足条件 从successor中删除KeyEntry，并添加其到new_node的keyTable
			 new_node->store_key(p->data); //添加keyEntry到新节点
			 ///////////////////////////////////////////////////////////
			 q=p;		//指向待删除节点
			 p=p->next;	//下一个可用节点
			 successor->DelKeyEntry(q); //从后继中删除表项
			 continue;			 
			 //////////////////////////////
		 }
		 p=p->next;		
	 }
	 return true;
 }
 //地址为addr的节点共享文件filename
 bool ChordRing::share_file(const address& addr,const std::string& filename)
 {
	 hash_value key;
	 Node::hash_key(filename,&key);	//获取文件hash值
	 Node* node = find_closest_successor(key); //查找键值对<K,V>归属
	 if(node == NULL){
		 return false;
	 }
	 //更新keyTable
	 node->add_file(filename,addr);
	return true;
 }
 //转移keyTable
 /*
	将node的关键字表转移到pNode
 */
 bool ChordRing::move_keyTable(const Node& node,Node* pNode)
 {
	 if(pNode == NULL)
		 return false;
	 KeyTable kt = node.get_key_table();	 
	 int size = kt.get_keyTableSize();
	 key_entry ke;
	 for(int i=1;i<=size;i++){ 
		 kt.GetElem(i,ke);	//获取表项
		 pNode->store_key(ke);//存储表项
	 } 
	 return true;
 }
 //////////////////////////////////////////////////////////////////////////
 //删除一个节点
 /*
 1.从环中删除一个节点
 2.更新其他节点的finger表
 3.转移关键字表
 */
 bool ChordRing::del_node(address& addr)
 {
	 if(!Hash::isvalid(addr.map))
		 Node::hash_node(addr);
	 NodePointer p = find(addr.map);//找到该节点
	 update_others_finger(p->data,Node::DEL_NODE);
	 Node* successor = find_successor(addr.map);	//找到其后继节点
	 if(successor == NULL)
		 return false;
	move_keyTable(p->data,successor);
	 DeleteList(p);
	 return true;
 }
 bool ChordRing::aux_find_file(const Node&node,const std::string& filename,address& afind)
 {
	 return true;
 }
  //查找hash值为hv的文件
 /*
 参数说明:
	node:发起查询的节点
	hv:查询文件的hash值
	lst:返回的地址列表
 */
 bool ChordRing::find_file(const Node& node,const hash_value& hv,Addr_Lst& lst)
 {
	  NodePointer pNode = find(node.get_hash_value()); //找到查询节点
	 if(pNode == NULL)
		 return false;
	 //先在keyTable查找
	 bool ret = (pNode->data).find_file(hv,lst);
	 if(ret){ 
		 return ret; //查找成功返回
	 }
	 address afind;
	 bool result = false;
	 result =aux_find_file(node,hv,afind); //本地查找失败，在环上查找
	 if(result){ //查找成功
		 pNode = find(afind.map); //找到存储<K,V>节点
		 if(pNode == NULL)
			 return false;
		 return (pNode->data).find_file(hv,lst);//获取存储地址列表
	 }
	 return result;
 }
 /*查询辅助函数*/
 /*
 函数功能:辅助查询存储key值为hv的节点
 参数说明:
 node ：查询节点
 hv:	key值
 afind:存储<K,V>对的节点
 return :  true -查找成功  false-查找失败
 */
 bool ChordRing::aux_find_file(const Node& node,const hash_value& hv,address& afind)
 {
	 assert(Hash::isvalid(hv));
	 NodePointer pNode = find(node.get_hash_value()); //找到查询节点
	 if(pNode == NULL)
		 return false;
	Node* p = &(pNode->data);
	 while(!(p->get_closest_finger(hv,afind))){
		 pNode =find(afind.map);
		 if(pNode == NULL)
			 return false;
		 p =&(pNode->data);
	 }
	 return true;
 }

 
 //////////////////////////////////////////////////////////////////////////
 /*在环中查找hash值为hv的节点 失败返回NULL*/
 /*注：在真实的网络环境中通过socket通信完成*/
 ChordRing::NodePointer ChordRing::find(const hash_value& hv)
 {
	 NodePointer head = Gethead();
	 if(head ==NULL)
		 return NULL;
	 NodePointer p = head;
	 while(head!=p->next){		
		 if(Hash::isequal(hv,(p->data).get_hash_value())){
			 return p;
		 }
		 p = p->next;
	 }
	 if(Hash::isequal(hv,(p->data).get_hash_value())){
		 return p;
	 }
	 return NULL;
 }
 //打印finger表
void ChordRing::PrintNodeFinger()
{
	NodePointer head = Gethead();
	if(head ==NULL)
		return ;
	NodePointer p = head;
	while(head!=p->next){		
		(p->data).PrintFingerTable();
		p = p->next;
	}
	(p->data).PrintFingerTable();
}
//打印KeyTable
void ChordRing::PrintKeyTable()
{
	NodePointer head = Gethead();
	if(head ==NULL)
		return ;
	NodePointer p = head;
	while(head!=p->next){		
		(p->data).PrintKeyTable();
		p = p->next;
	}
	(p->data).PrintKeyTable();
}
