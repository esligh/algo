#ifndef _KADMELIA_COMMON_H__
#define _KADMELIA_COMMON_H__
#include "user_def.h"
#include <fstream>
/*COMMON共有类定义常量*/
class Common
{
public:
	/************************************************************************/
	/*      static method																									*/
	/************************************************************************/
	static bool Init();
	static void set_path(const std::string& s);
	static std::string get_path();
	static void set_k(smallInt n);
	static smallInt get_k();
	static void set_a(smallInt n);
	static smallInt get_a();
	static void random(std::string& s);
	static bool isequal_dest(const destination& d1,const destination& d2);
	static void Release();
	static std::fstream& get_filehandle();
private:
	static void rand_bytes(unsigned char*,int);
	static void closefile(){outFile.close();}
private:	
	static smallInt k;//bucket size
	static smallInt a; //get neighbour size
	static const smallInt default_ksize=4;
	static const smallInt default_asize=3;
	static std::fstream outFile;
	static std::string path;
private:/*noncopyable*/
	Common();
	~Common();
	Common(const Common&);
	Common& operator =(const Common&);
};
#endif