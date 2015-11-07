#ifndef  _UTIL_H__
#define _UTIL_H__

#include <boost/uuid/sha1.hpp>
#include <assert.h>
using namespace boost::uuids::detail;
#define  EQ		0
#define  SM		-1
#define  LA		1
const  unsigned short  sha_ret_len=5;
typedef struct tag_sha_ret
{
	unsigned int _block[sha_ret_len]; //20 byte  as value
}digest_type;
//gen the sha identify
void sha_1(char* src,size_t len,digest_type* pret);
//compute the same value
 int distance(const digest_type& r1,const digest_type& r2,digest_type* distance);
//////////////////////////////////////////////////////////////////////////
 void hash(char* src,size_t  len,int* r); //
 int distance_d(int* p1,int* p2,int* d);
#endif /*UTIL_H*/
