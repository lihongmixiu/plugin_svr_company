#include "proto.h"
#include "FileUtil.h"
#include "CodeMsg.h"
#include "md5.h"
#include "ReadCfg.h"
#include "log.h"
#define PROTO_HEAD_SIZE 12
#define MAX_SVR_PKG_LENGTH 1024*4096
int  Proto::dealPkg(const char *pkg,int len,int &type)
{
   unsigned int svrVersion = ReadCfg::getVersion();   /*the plugin version of server,read by cfg file*/
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
			   type = 12;
		   }
		   else
		   { 
			   type = 10;   /*this function is just deal the pkg and tell which pkgType is */
		   }
		   break;
	  case CS_CMD_REQ_REP_DATA:
		   //buildPkgByType(11,replyPkg);
		   type = 11;
	       break;
	  case CS_CMD_REQ_UPDATE_LINK:
		   type = 14;
		   break;
	  default:
		   printf("no find proto\n");
	       break;
   }
	free(pkgContent);
    free(pkgHead);	
	return 0;   
   	
}

int Proto::buildPkgByType(int pkgType,char *buff,size_t &n)
{
	 switch(pkgType)
		{
			case CS_CMD_RES_SEND_PLUGIN:
					buildSendPluginPkg(buff,n);
			         break;
            case CS_CMD_RES_IS_UPDATE:
					buildIsUpdatePkg(buff,n);
					 break;
            case CS_CMD_RES_REP_DATA:
		            buildResReportDataPkg(buff,n);
					 break;
			case CS_CMD_RES_SEND_LINK:
					 buildSendLinkPkg(buff,n);
					 break;
            default:
					 printf("no find\n");
		             break;
									    
	     }
										   
	return 0;
}

int Proto::buildIsUpdatePkg(char *buff,size_t &n)
{
	printf("buildIsUpdatePkg begin\n");
	Log::writeLog(1,"buildIsUpdatePkg begin");
	tagCSMsgHead *msgHeader = (tagCSMsgHead*)malloc(PROTO_HEAD_SIZE);
	if(msgHeader == NULL)
	{
		Log::writeLog(3,"msgHeader malloc failed!");
		return -1;
	}	
	tagCSSendNoNeedUpdateS *msgContent = (tagCSSendNoNeedUpdateS*)malloc(16);
	if(msgContent == NULL)
	{
		 Log::writeLog(3,"msgContent malloc failed!");
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
	n = PROTO_HEAD_SIZE+16;/*record the pkg length*/
	free(msgContent);
	free(msgHeader);
	return 0;
}

int Proto::buildSendPluginPkg(char *buff,size_t &n)
{
	printf("buildSendPluginPkg begin\n");
	Log::writeLog(1,"buildSendPluginPkg begin");
	tagCSMsgHead *msgHeader =(tagCSMsgHead*)malloc(PROTO_HEAD_SIZE);
	if(msgHeader == NULL)
	{
		printf("msgHeader malloc failed!\n");
		return -1;
	}	
	tagCSSendPluginDataS *msgContent = (tagCSSendPluginDataS*)malloc(MAX_SVR_PKG_LENGTH);
	if(msgContent == NULL)
	{
		printf("msgContent malloc failed!\n");
		return -1;
	}
	size_t contentLen = 0;
	if((FileUtil::FileToBytes("pt.vfs",(char*)msgContent,contentLen))<0)
	{
		printf("FileToBytes error\n");
		Log::writeLog(2,"FileToBytes error");
		return -2;
	}
	if(msgContent == NULL)
	{
		printf("msg is NULL\n");
	}
	char *md5Check = (char*)malloc(33);
	MD5_CTX md5Util;
	if(!md5Util.GetFileMd5(md5Check,"pt.vfs"))
	{
		printf("cal md5 error\n");
		return -3;
	}
	printf("md5 value:%s\n",md5Check);
	Log::writeLog(1,"the current md5 value is:%s",md5Check);
	msgHeader->pkgLength = PROTO_HEAD_SIZE+contentLen+32;
	msgHeader->cmdID = 12;
	msgHeader->protoVersion = 1111;
//	memcpy(msgHeader->md5Check,md5Check,32);
	CodeMsg::HeadHton(*msgHeader);
	memcpy(buff,msgHeader,PROTO_HEAD_SIZE);
	memcpy(buff+PROTO_HEAD_SIZE,md5Check,32);
	memcpy(buff+PROTO_HEAD_SIZE+32,msgContent,contentLen);
	n = PROTO_HEAD_SIZE+32+contentLen;	
	free(md5Check);
	free(msgContent);
	free(msgHeader);	
	printf("buildIsSendPluginPkg end\n");	
	return 0;
}

int Proto::buildSendLinkPkg(char* buff,size_t &len)
{
	printf("buildSendLinkPkg begin\n");
	
	int linkNum = ReadCfg::getLinkNum();
	//	tagCSLinkData[linkNum];
	char *posFlag = buff+PROTO_HEAD_SIZE+4; /*point to the position of buff,4is link num*/
	for(int i=1;i<=linkNum;i++)
	{
		char *linkText = (char*)malloc(sizeof(tagCSLinkKey));
		if(linkText == NULL)
		{
			Log::writeLog(3,"linkText malloc fail");
			return -1;
		}
		char *linkValue =(char*)malloc(sizeof(tagCSLinkValue));
		if(linkValue == NULL)
		{
			Log::writeLog(3,"linkValue malloc fail");
			return -1;
		}
		char *linkData = (char*)malloc(sizeof(tagCSLinkData));
		if(linkData == NULL)
		{
			Log::writeLog(3,"linkData malloc fail");
			return -1;
		}
		printf("linkText struct len:%d\n",sizeof(tagCSLinkKey));
		printf("linkValue struct len:%d\n",sizeof(tagCSLinkValue));
		printf("linkData struct len:%d\n",sizeof(tagCSLinkData));
		memset(linkText,0,sizeof(tagCSLinkKey));
		memset(linkValue,0,sizeof(tagCSLinkValue));
		memset(linkData,0,sizeof(tagCSLinkData));
		size_t textLen = 0;
		size_t valueLen = 0;
		size_t linkLen = 0;
		size_t linkDataLen = sizeof(unsigned int)+sizeof(tagCSLinkKey)+sizeof(tagCSLinkValue);
		if(readCfgUtil->getLinkText(i,linkText,textLen)==0
		&& readCfgUtil->getLinkValue(i,linkValue,valueLen)==0)
		{
			printf("linkLen:%d,valueLen:%d\n",textLen,valueLen);
			if(textLen>0 && valueLen>0)
			{
				linkLen = textLen + valueLen +4;
				size_t netLinkLen = htonl(linkLen);
				memcpy(linkData,&netLinkLen,4);
				memcpy((char*)linkData+sizeof(unsigned int),linkText,textLen);
				memcpy((char*)linkData+sizeof(unsigned int)+textLen,linkValue,valueLen);
				//linkData->length = htonl(linkLen);
				//linkData->key =(tagCSLinkKey)(*linkText);
				//linkData->value =(tagCSLinkValue) *linkValue;
			}
		    printf("the linkData len:%d\n",linkLen);	
			memcpy(posFlag,linkData,linkLen);/*copy one linkData*/
			posFlag += linkLen;
			len += linkLen; /*add to the length of total buff*/
		}

		free(linkData);
		free(linkValue);
		free(linkText);
	}
	tagCSMsgHead *msgHeader =(tagCSMsgHead*)malloc(PROTO_HEAD_SIZE);
	if(msgHeader == NULL)
	{
		printf("msgHeader malloc failed!\n");
		return -1;
	}
	msgHeader->pkgLength = PROTO_HEAD_SIZE+len+4;
	msgHeader->cmdID = 14;
	msgHeader->protoVersion = 1111;
//	memcpy(msgHeader->md5Check,md5Check,32);
	CodeMsg::HeadHton(*msgHeader);
	memcpy(buff,msgHeader,PROTO_HEAD_SIZE);
	size_t linkNum4Net = htonl(linkNum);
	printf("linkNum:%d\n",linkNum);
	memcpy(buff+PROTO_HEAD_SIZE,&linkNum4Net,4);
	len += PROTO_HEAD_SIZE+4;/*add the length of pkg head and the linkNum*/
	printf("buildSendLinkPkg end and the total len is :%d\n",len);
	return 0;
}

int Proto::buildResReportDataPkg(char *buff,size_t &n)
{
	
	printf("buildResReportDataPkg begin\n");	
	return 0;
}
