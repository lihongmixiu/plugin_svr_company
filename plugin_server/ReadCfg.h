#ifndef READCFG_H
#define READCFG_H
#include<stdlib.h>
#include<stdio.h>
#include<fstream>
#include"tinyxml.h"
#include"tinystr.h"
#include"proto.h"
static unsigned int svrVersion = 0;
static TiXmlDocument *xmlDom = NULL;
static  int linkNum = 0;

class ReadCfg{
	public:
		static void setVersion(const char*cfgFile);
		static unsigned int getVersion();
	    static int getLinkNum();
		static int readXml(const char* fileName);
		int getLinkText(int index,char *buff,size_t &len);
		int getLinkValue(int index,char *buff,size_t &len);		
};
#endif
