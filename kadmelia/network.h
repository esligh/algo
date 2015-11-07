#ifndef _KADMELIA_NETWORK_CLASS_DEF_H__
#define _KADMELIA_NETWORK_CLASS_DEF_H__
/*NetWork��ģ�����绷�����в���*/
#include "node.h"
typedef std::list<Node>		NET; //ʹ��list��֯�ڵ�������ģ������
typedef NET::iterator			NET_ITER;
typedef NET::const_iterator			NET_CITER;
class NetWork
{
public:
	NetWork();
	NetWork(const Node& node);
	~NetWork();
	bool join(Node& node);//����һ���ڵ�
	bool leave(const Node& node);//�뿪һ���ڵ�
	bool find_kv(const Node& node,const hash_value& key,Vnode& lst);
	bool find_kv(const Node& node,const std::string& name,Vnode&lst);
	static Node* find(const hash_value& hv);
	
	static bool ishas(const Node&node);
	//////////////////////////////////////////////////////////////////////////
	static void PrintFinger();
	void PrintNodeDict(const Node&node);
	Node* RandContact();
private:
		 static NET net_;
};
#endif