// kadmelia.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "hash.h"
#include "common.h"
#include "node.h"
#include "network.h"
#include <fstream>
/*
template <int theValue>
void add(int& elem)
{
	elem+=theValue;
}
class AddValue
{
public:
	AddValue(int v):theValue(v){}
	void operator()(int&elem)const{
		elem+=theValue;
	}
private:
	int theValue;
};
bool print(const int& elem)
{
	std::cout<<elem<<" ";
	return false;
}
*/
int _tmain(int argc, _TCHAR* argv[])
{/*
	hash_value v;
	v.value_ =7;
	std::cout<<Hash::log2(v)<<std::endl;
	*/
	/*
	std::vector<int> coll;
	for(int i=0;i<10;i++)
		coll.push_back(i);
	int n=10;
	//for_each(coll.begin(),coll.end(),AddValue(n));
	//for_each(coll.begin(),coll.end(),print);
	*/
	//copy(coll.begin(),coll.end(),std::ostream_iterator<int>(std::cout," "));
//	file.rdbuf();

//	file<<"hello world!";
	{		
		Common::Init();

		std::cout<<Hash::get_hashbits()<<std::endl;
		nodeinfo info1;
		info1.ip_ = "192.168.1.101";
		info1.port_=5566;
		Node node1(info1);
		NetWork net(node1);

		nodeinfo info2;
		info2.ip_ = "192.168.1.102";
		info2.port_=5566;
		Node node2(info2);
		net.join(node2);
		
		nodeinfo info3;
		info3.ip_ = "192.168.1.103";
		info3.port_=5566;
		Node node3(info3);
		net.join(node3);
		
		
		nodeinfo info4;
		info4.ip_ = "192.168.1.104";
		info4.port_=5566;
		Node node4(info4);
		net.join(node4);
		
		nodeinfo info5;
		info5.ip_ = "192.168.1.105";
		info5.port_=5566;
		Node node5(info5);
		net.join(node5);
		
		
		nodeinfo info6;
		info6.ip_ = "192.168.1.106";
		info6.port_=5566;
		Node node6(info6);
		net.join(node6);

		nodeinfo info7;
		info7.ip_ = "192.168.1.107";
		info7.port_=5566;
		Node node7(info7);
		net.join(node7);

		nodeinfo info8;
		info8.ip_ = "192.168.1.108";
		info8.port_=5566;
		Node node8(info8);
		net.join(node8);

		nodeinfo info9;
		info9.ip_ = "192.168.1.109";
		info9.port_=5566;
		Node node9(info9);
		net.join(node9);
		
		nodeinfo info10;
		info10.ip_ = "10.45.47.100";
		info10.port_=5566;
		Node node10(info10);
		net.join(node10);
		
		
		////23 30 37 44 51 58 1 8 15    24 31 38 45 52 59
		nodeinfo info11;
		info11.ip_ = "10.45.47.101";
		info11.port_=5566;
		Node node11(info11);
		net.join(node11);

		nodeinfo info12;
		info12.ip_ = "10.45.47.102";
		info12.port_=5566;
		Node node12(info12);
		net.join(node12);
	
		nodeinfo info13;
		info13.ip_ = "10.45.47.103";
		info13.port_=5566;
		Node node13(info13);
		net.join(node13);

		nodeinfo info14;
		info14.ip_ = "10.45.47.104";
		info14.port_=5566;
		Node node14(info14);
		net.join(node14);

		nodeinfo info15;
		info15.ip_ = "10.45.47.105";
		info15.port_=5566;
		Node node15(info15);
		net.join(node15);
		
		net.PrintFinger();
		std::string file1 ="hello.cpp";
		node6.share_torrent(file1);
		node5.share_torrent(file1);

		std::string file2 = "test.mp3";
		node3.share_torrent(file2);
		node4.share_torrent(file2);

		std::string file3 = "brave heart.rmvb";
		node7.share_torrent(file3);

		net.PrintNodeDict(node10);
		net.PrintNodeDict(node2);
		net.PrintNodeDict(node5);
		Vnode lst;
		net.find_kv(node9,file3,lst);
		net.find_kv(node3,file2,lst);
		net.find_kv(node6,hash_value(55),lst);
		Common::Release();
	}
	_CrtDumpMemoryLeaks();
	getchar();
	return 0;
}

