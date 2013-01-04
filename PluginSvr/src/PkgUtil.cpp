#include <stdio.h>
#include <stdlib.h>
#include "Proto.h"
#include "CodeMsg.h"
#include "ConfigUtil.h"
#include "LogWrapper.h"
#include "FileUtil.h"
#include "Md5.h"
#include "PkgUtil.h"
PkgUtil::PkgUtil()
{
	//no -op
}

PkgUtil::~PkgUtil()
{
	//no -op
}

int PkgUtil::ProcessClientPkg(const char *pkg,int len,int &pkgType)
{
   unsigned int svrVersion = CONFIG_UTIL->GetServerVersion();
   char* pkgHead =(char*) malloc(PROTO_HEAD_SIZE);
   if(pkgHead == NULL)
   {
	   return -1;
   }
   char* pkgContent = (char*)malloc(len-PROTO_HEAD_SIZE);
   if(pkgContent == NULL)
   {
	   return -1;
   }
   memset(pkgHead,0,PROTO_HEAD_SIZE);
   memset(pkgContent,0,len-PROTO_HEAD_SIZE);
   memcpy(pkgHead,pkg,PROTO_HEAD_SIZE);
   memcpy(pkgContent,pkg+PROTO_HEAD_SIZE,len-PROTO_HEAD_SIZE);
   tagCSMsgHead* msgHeader = (tagCSMsgHead*)pkgHead;
   CodeMsg::HeadNtoh(*msgHeader);  /*trans to the host byte order*/
   if(msgHeader->protoVersion != 1111)
   {
	   return -2;
   }
   tagCSReqNewVersion* msgContent; /*g++ compiler do not allow define in case*/
   switch(msgHeader->cmdID)
   {
	   case CS_CMD_REQ_NEW_VERSION:
		   msgContent = (tagCSReqNewVersion*)pkgContent;
		   printf("version value:%d\n",ntohl(msgContent->version));
		   if(ntohl(msgContent->version)!=svrVersion)
		   {
			   pkgType = 12;
		   }
		   else
		   { 
			   pkgType = 10;   /*this function is just deal the pkg and tell which pkgType is */
		   }
		   break;
	  case CS_CMD_REQ_REP_DATA:
		   pkgType = 11;
	       break;
	  case CS_CMD_REQ_UPDATE_LINK:
		   pkgType = 14;
		   break;
	  default:
		   printf("no find proto\n");
	       break;
   }
	free(pkgContent);
    free(pkgHead);	
	return 0;   
}

int PkgUtil::BuildPkgByType(int pkgType,char *outputBuff,size_t &len)
{	
	switch(pkgType)
		{
			case CS_CMD_RES_SEND_PLUGIN:
					BuildSendPluginPkg(outputBuff,len);
			         break;
            case CS_CMD_RES_IS_UPDATE:
					BuildIsUpdatePkg(outputBuff,len);
					 break;
            case CS_CMD_RES_REP_DATA:
		            BuildResReportDataPkg(outputBuff,len);
					 break;
			case CS_CMD_RES_SEND_LINK:
					 BuildSendLinkPkg(outputBuff,len);
					 break;
            default:
					 printf("no find\n");
		             break;
									    
	     }
	return 0;
}

int PkgUtil::BuildIsUpdatePkg(char *buff,size_t &len)
{
	printf("buildIsUpdatePkg begin\n");
	TRACESVR(GG_DEBUG,"buildIsUpdatePkg begin.");
	tagCSMsgHead *msgHeader = (tagCSMsgHead*)malloc(PROTO_HEAD_SIZE);
	if(msgHeader == NULL)
	{
		TRACESVR(GG_ERROR,"msgHeader malloc failed");
		return -1;
	}	
	tagCSSendNoNeedUpdateS *msgContent = (tagCSSendNoNeedUpdateS*)malloc(16);
	if(msgContent == NULL)
	{
		 TRACESVR(GG_ERROR,"msgContent malloc failed");
		 return -1;
	}
	msgHeader->pkgLength = 28;
	msgHeader->cmdID = 10;
	msgHeader->protoVersion = 1111;
	CodeMsg::HeadHton(*msgHeader);
	char checkContent[20]="aaaaaaaaaaaaaaaa";
	memcpy(buff,msgHeader,PROTO_HEAD_SIZE);
	printf("checkContent:%s\n",checkContent);
	memcpy(buff+PROTO_HEAD_SIZE,checkContent,16);
	len = PROTO_HEAD_SIZE+16;/*record the pkg length*/
	free(msgContent);
	free(msgHeader);
	return 0;
}

int PkgUtil::BuildSendPluginPkg(char* buff,size_t &len)
{
	printf("buildSendPluginPkg begin\n");
	TRACESVR(GG_DEBUG,"buildSendPluginPkg begin!");
	tagCSMsgHead *msgHeader =(tagCSMsgHead*)malloc(PROTO_HEAD_SIZE);
	if(msgHeader == NULL)
	{
		TRACESVR(GG_ERROR,"msgHeader malloc failed");
		return -1;
	}	
	tagCSSendPluginDataS *msgContent = (tagCSSendPluginDataS*)malloc(MAX_SVR_PKG_LENGTH);
	if(msgContent == NULL)
	{
		TRACESVR(GG_ERROR,"msgHeader malloc failed");
		return -1;
	}
	size_t contentLen = 0;
	if((FileUtil::FileToBytes("./../vfs/pt.vfs",(char*)msgContent,contentLen))<0)
	{
		printf("FileToBytes error\n");
		TRACESVR(GG_ERROR,"File To Bytes error!");
		return -2;
	}
	if(msgContent == NULL)
	{
		TRACESVR(GG_ERROR,"msg is null");
		return -1;
	}
	char *md5Check = (char*)malloc(33);
	MD5_CTX md5Util;
	if(!md5Util.GetFileMd5(md5Check,"./../vfs/pt.vfs"))
	{
		printf("cal md5 error\n");
		TRACESVR(GG_ERROR,"cal md5 error!");
		return -3;
	}
	printf("md5 value:%s\n",md5Check);
	TRACESVR(GG_DEBUG,"the current file md5 value is:%s",md5Check);
	msgHeader->pkgLength = PROTO_HEAD_SIZE+contentLen+32;
	msgHeader->cmdID = 12;
	msgHeader->protoVersion = 1111;
	CodeMsg::HeadHton(*msgHeader);
	memcpy(buff,msgHeader,PROTO_HEAD_SIZE);
	memcpy(buff+PROTO_HEAD_SIZE,md5Check,32);
	memcpy(buff+PROTO_HEAD_SIZE+32,msgContent,contentLen);
	len = PROTO_HEAD_SIZE+32+contentLen;	
	free(md5Check);
	free(msgContent);
	free(msgHeader);	
	printf("buildIsSendPluginPkg end\n");	
	TRACESVR(GG_DEBUG,"build isSendPluginPkg end.");	
	return 0;
}

int PkgUtil::BuildResReportDataPkg(char* buff,size_t &len)
{
	
	return 0;
}

int PkgUtil::BuildSendLinkPkg(char* buff,size_t &len)
{
	return 0;
}
