#include "FileUtil.h"
#include "fstream"
#include "log.h"
int  FileUtil::FileToBytes(const char* fileName,char *bytes,size_t &len)
{
	fstream in;
	/*
	char *fileData =(char*) malloc(1024*4096);
	if(fileData==NULL)
	{
		printf("fileData malloc failed\n");
		return -1;
	}
	memset(fileData,0,1024*4096);
	*/
	if(fileName ==NULL)
	{
	
		Log::writeLog(3,"the fileName is NULL");
		return -1;
	}
	if(bytes == NULL)
	{
		Log::writeLog(3,"the bytes is NULL");
		return -1;
	}
	in.open(fileName,ios::in);
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
	Log::writeLog(1,"the vfs pkg size is:%d",count);
	return 0;	
}
