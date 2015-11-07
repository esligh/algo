#include "stdafx.h"
#include "user_define.h"
#include "hash.h"
#include "debuger.h"
#include "chordring.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	//hash值测试
	/*
	CommonEnv::set_m(3);
	hash_value t1,t2,t3,t4,t5;
	Node::hash_key("月光边境.mp3",&t1);//2
	Node::hash_key("月光边境.mp3",&t2);//2
	Node::hash_key("那些年.mp3",&t3);//3
	Node::hash_key("七剑战歌.mp3",&t4);//1
	Node::hash_key("everything in the world.mp3",&t5);//6
	std::cout<<"t1="<<t1.value_<<std::endl;
	std::cout<<"t2="<<t2.value_<<std::endl;
	std::cout<<"t3="<<t3.value_<<std::endl;
	std::cout<<"t4="<<t4.value_<<std::endl;
	std::cout<<"t5="<<t5.value_<<std::endl;
	getchar();
	*/
	{
	CommonEnv::set_m(3);
	std::cout<<"*******************************************************"<<std::endl;
		address addr1;
		addr1.ip = "192.168.1.102";
		addr1.port = 5352;
		Node node1(addr1);
		ChordRing ring(node1);
		ring.PrintNodeFinger();	
		std::string file1("hello.cpp");//7
		ring.share_file(addr1,file1);

		std::string file2("122.txt");	//0
		ring.share_file(addr1,file2);
		std::string file3("大地之灯.pdf");//5
		ring.share_file(addr1,file3);
		ring.PrintKeyTable();
		
	std::cout<<"*******************************************************"<<std::endl;
	{
		address addr2;
		addr2.ip ="192.168.1.100";
		addr2.port=5352;	
		Node node2(addr2);
		ring.add_node(node2);
		std::cout<<"after adding node...\n"<<std::endl;
		ring.PrintNodeFinger();
		std::string file4("eclipse.exe");//4
		ring.share_file(addr2,file4);
		ring.PrintKeyTable();
	}
	std::cout<<"*******************************************************"<<std::endl;
	{
		address addr3;
		addr3.ip ="171.69.123.100";
		addr3.port=5352;
		Node node3(addr3);
		bool b=ring.add_node(node3);
		if(!b){
			std::cout<<"node exists in the chord ring."<<std::endl;
		}
		ring.PrintNodeFinger();
		std::string file5("月光边境.mp3");//2
		ring.share_file(addr3,file5);
		std::string file6("那些年.mp3");//3
		ring.share_file(addr3,file6);
		ring.PrintKeyTable();
	}
	std::cout<<"*******************************************************"<<std::endl;	
	/*
	ring.del_node(addr1);
	ring.PrintNodeFinger();
	ring.PrintKeyTable();
	*/
	
	address addr4;
	addr4.ip="200.154.130.90";
	addr4.port=5352;
	Node node4(addr4);
	bool b2 = ring.add_node(node4);
	if(!b2){
		std::cout<<"node exists in the chord ring."<<"hash_value="<<node4.get_hash_value().value_<<std::endl;
	}
	ring.PrintNodeFinger();
	std::string file7("everything in the world.mp3");//6
	ring.share_file(addr4,file7);
	ring.PrintKeyTable();
	
	std::cout<<"*******************************************************"<<std::endl;
	{
	address addr5;
	addr5.ip="10.45.47.105";
	addr5.port=5352;
	Node node5(addr5);
	bool b3 = ring.add_node(node5);
	if(!b3){
		std::cout<<"node exists in the chord ring."<<"hash_value="<<node4.get_hash_value().value_<<std::endl;
	}
	ring.PrintNodeFinger();
	std::string file8("chord.rar");//1
	ring.share_file(addr5,file8);
	std::string file9("hello.cpp");
	ring.share_file(addr5,file9);
	ring.PrintKeyTable();
	}
	std::cout<<"*******************************************************"<<std::endl;
	
	std::cout<<"after a node leaving...."<<std::endl;
	ring.del_node(addr4);
	ring.PrintNodeFinger();
	ring.PrintKeyTable();
	
	std::cout<<"*******************************************************"<<std::endl;	
	
	hash_value key;
	key.value_ = 7;
	std::cout<<"look up file,"<<"key="<<key.value_<<"......."<<std::endl;
	address aret;
	ring.aux_find_file(node1,key,aret);
	Debuger::PrintDescript("find result:\n");
	Debuger::PrintAddr(aret);
	Debuger::PrintDescript("\n");
	std::cout<<"find destination address...."<<std::endl;
	Addr_Lst lst;
	ring.find_file(node1,key,lst);
	for(Iterator_hLst iter = lst.begin();iter != lst.end();iter++){
		Debuger::PrintAddr(*iter);
		Debuger::PrintDescript("\n");
	}
	
	/*for(int i=0;i<8;i++){
		hash_value key;
		key.value_ = i;
		address aret;
		ring.aux_find_file(node5,key,aret);
		Debuger::PrintDescript("find result:\n");
		Debuger::PrintAddr(aret);
		Debuger::PrintDescript("\n");
	}	
	*/
	std::cout<<"*******************************************************"<<std::endl;

}
	_CrtDumpMemoryLeaks();	
	getchar();
	return 0;
}