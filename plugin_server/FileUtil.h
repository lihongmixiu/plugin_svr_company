#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
using namespace std;
struct ByteInfo
{
	char *bytePointer;
	int length;
};
class FileUtil
{
	public:
		/*@param:fileName*/
		/*@return:byteinfo ,the transformed result*/
	static	int  FileToBytes(const char*file,char* bytes,size_t &len);
		
};
