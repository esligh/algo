#include "stdafx.h"
#include "util.h"

/************************************************************************/
/*
	method: sha_1  return a special message's identify
	param:  src --message value
				len --message len
				pret -- sha ret value
*/
/************************************************************************/

void sha_1(char* src,size_t len,digest_type* pret)
{
	assert(src!=NULL && len!=0);
	sha1 sha;
	sha.process_bytes(src,len);
	sha.get_digest(pret->_block);
}

/************************************************************************/
/*
	method: disatnce() --computer the distance of two sha_ret value 
	param: r1 -- sha_ret value 1
				r2--sha_ret value 2
				r -- the two sha_ret value 's distance
	return :  r1 > r2   LA
				r1==r2  EQ
				r1<r2    SM
*/
/************************************************************************/
int distance(const digest_type& s1,const digest_type& s2,digest_type* r)
{
	int ret = -2; //invalid ret value
	for(unsigned short i = 0;i<sha_ret_len;i++){
		if(s1._block[i] < s2._block[i]){
			ret = SM;
			break;
		}
		else if(s1._block[i] > s2._block[i]){
			ret = LA;
			break;
		}
	}
	if(ret ==-2){
		ret = EQ;
	}	
	for(unsigned short i =0;i<sha_ret_len;i++){
		r->_block[i]=s1._block[i]>s2._block[i] ? \
			s1._block[i]-s2._block[i] : s2._block[i]-s1._block[i];
	}
	return ret;
}
//////////////////////////////////////////////////////////////////////////

void hash(char* src,size_t len,int* r)
{	
	assert(src!=0);
	int sum=0;
	for(size_t i = 0;i<len;i++){
		sum+=(src[i]*i*7);
	}
	*r=sum%(32);
}

int hash(int* p1,int* p2,int* d)
{
	*d = p1-p2;
	return *d;
}
