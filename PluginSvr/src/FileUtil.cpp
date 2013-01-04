#include "FileUtil.h"
#include <fstream.h>
#include "LogWrapper.h"
using namespace std;
int  FileUtil::FileToBytes(const char* fileName,char *bytes,size_t &len)
{
	fstream in;
	if(fileName ==NULL)
	{
	
		TRACESVR(GG_ERROR,"vfs file name is null");
		return -1;
	}
	if(bytes == NULL)
	{
		TRACESVR(GG_ERROR,"byte pointer is null");
		return -1;
	}
	in.open(fileName,ios::in);
	if(in.fail())
	{
		TRACESVR(GG_ERROR,"file open fail,try check the path!");
		return -2;
	}
	char ch;
	unsigned	int count = 0;
	char *point;
	point = bytes;
	while(in.get(ch))
	{
		count++;
		*point = ch;
		point++;
		
	}

	len = count;
	TRACESVR(GG_DEBUG,"the vfs file size is:%d bytes",len);
	return 0;	
}
