#include "stdafx.h"
#include "common.h"
#include "hash.h"

smallInt Common::k =default_ksize;
smallInt Common::a=default_asize;
std::string Common::path="F:\\out.txt";
std::fstream Common::outFile(path,std::ios::out);

bool Common::Init()
{
	return true;
}
void Common::set_path(const std::string& s)
{
	path =s;
}
std::string Common::get_path()
{
	return path;
}
std::fstream& Common::get_filehandle()
{
	return outFile;
}
void Common::set_k(smallInt n)
{
	k = n;
}
smallInt Common::get_k()
{
	return k;
}
void Common::set_a(smallInt n)
{
	a = n;
}
smallInt Common::get_a()
{
	return a;
}

bool Common::isequal_dest(const destination& d1,const destination& d2)
{
	return (Hash::isequal(d1.id_,d2.id_)&&d1.ip_ == d2.ip_ && d1.port_ == d2.port_ );
}

void Common::Release()
{
	closefile();
}
//////////////////////////////////////////////////////////////////////////
void Common::rand_bytes(unsigned char* buf,int buf_len)
{
	typedef boost::variate_generator<boost::mt19937,boost::uniform_smallint<>> var_gen;
	static var_gen gen(boost::mt19937((boost::int32_t)std::time(0)),boost::uniform_smallint<>(1,255));
	for(int i =0;i<buf_len;++i){
		buf[i] = gen();
	}
}