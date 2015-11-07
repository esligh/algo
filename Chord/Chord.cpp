// Chord.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "util.h"

const int M = 5;
const int N= 1<<M;

typedef struct tag_pair_kv{
	int key;
	char host[N];
}pair_kv;
struct hash_table_dht_entry
{
	int node_id; //节点id
	int successor;//后继节点
	pair_kv pair;
};
struct hash_table_dht
{
	bool bon_;
	hash_table_dht_entry record[M];
	int next;
};

int set_table(hash_table_dht* table,int node_id)
{
	int ret = -1;
	assert(0<=node_id<N);
	for(int i=node_id+1;i != node_id;){
		if(table[i].bon_){
			ret = i;
			break;
		}
		i = table[i].next;
	}
	return ret; 
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	char* szMsg1 ="abcdefg";	
	digest_type ret1;
	sha_1(szMsg1,strlen(szMsg1),&ret1);
	for(int i=0;i<5;++i){
		printf("%x ",ret1._block[i]);
	}
	printf("\n");
	char * szMsg2 ="abcdefh";
	digest_type ret2;
	sha_1(szMsg2,strlen(szMsg2),&ret2);
	for(int i=0;i<5;i++){
		printf("%x ",ret2._block[i]);
	}

	printf("\n");
	digest_type d;
	int ret = distance(ret1,ret2,&d);
	printf("ret =%d:",ret);
	for(int i=0;i<5;i++){
		printf("%x ",d._block[i]);
	}
	*/
	//m =5 ;chord环用32(2^5)个节点 
	//文件节点
	char* fname1= "abc.txt";
	char* fname2="123.rmvb";
	char* fname3 ="hello.mp3";
	char* fname4 ="dds.pdf";

	//在线节点
	char* hostIp1="192.168.0.1:5557";
	char* hostIp2="222.220.14.231:80";
	char* hostIp3="151.78.24.120:3074";

	const int num = (1<<M);
	printf("num = %d\n",num);
	hash_table_dht node_table[num];
	//reset table
	for(int i=0;i<num;i++){
		node_table[i].bon_ = false;
		node_table[i].next = (i+1)%(1<<M);
	}

	//生成<K,V>对
	int key1,key2,key3,key4;
	hash(fname1,strlen(fname1),&key1);
	hash(fname2,strlen(fname2),&key2);
	hash(fname3,strlen(fname3),&key3);
	hash(fname4,strlen(fname4),&key4);
	printf("key1=%d,key2=%d,key3=%d,key4=%d\n",key1,key2,key3,key4);
	int node_id1,node_id2,node_id3;
	hash(hostIp1,strlen(hostIp1),&node_id1);
	hash(hostIp2,strlen(hostIp2),&node_id2);
	hash(hostIp3,strlen(hostIp3),&node_id3);
	printf("node_id1=%d,node_id2=%d,node_id3=%d\n",node_id1,node_id2,node_id3);
	node_table[node_id1].bon_ = true;

	node_table[node_id2].bon_ = true;
	node_table[node_id3].bon_ = true;
	//(a+2^(k-i))mod 2^m , 1<= k<=m

	int t = set_table(node_table,node_id1);
	printf("t = %d",t);

	getchar();
	return 0;
}

