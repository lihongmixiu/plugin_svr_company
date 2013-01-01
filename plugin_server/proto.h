#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#ifndef PROTO_CODE_H
#define PROTO_CODE_H
#include "CodeMsg.h"
#endif
#ifndef PROTO_CFG_H
#define PROTO_CFG_H
#include "ReadCfg.h"
#endif
#ifndef PROTO_H
#define PROTO_H
/*desc:Proto definenition */
#define MAX_SVR_PKG_LENGTH						1024*4096	/*max data length:4M*/
#define CLIENT_DATA_LENGTH                      16;         /*max client to svr for req version*/
#define PROTO_VERSION                           1111;       /*the version of protocal*/
#define MAX_LINK_LENGTH							100         /*max hyperlink length:100byte*/
#define MAX_LINK_NUM							12			/*max hyperlink number*/
/*protocal cmd const define*/
/*
#define CS_CMD_REQ_NEW_VERSION                  0;
#define CS_CMD_REQ_REP_DATA                     1;
#define CS_CMD_RES_IS_UPDATE                    10;
#define CS_CMD_RES_REP_DATA                     11;
#define CS_CMD_RES_SEND_PLUGIN                  12;
*/
enum protoType{
		CS_CMD_REQ_NEW_VERSION = 0,
		CS_CMD_REQ_REP_DATA = 1,
		CS_CMD_RES_IS_UPDATE = 10,
		CS_CMD_RES_REP_DATA = 11,
		CS_CMD_RES_SEND_PLUGIN = 12,
		CS_CMD_REQ_UPDATE_LINK = 13,
	    CS_CMD_RES_SEND_LINK = 14,

};
struct tagCSMsgHead;
typedef struct tagCSMsgHead							CSMSGHEAD;
typedef struct tagCSMsgHead							*LPCSMSGHEAD;

struct tagCSReqNewVersion;
typedef struct tagCSReqNewVersion CSREQNEWVER;
typedef struct tagCSReqNewVersion *LPCSREQNEWVER;

struct tagCSReqRepData;
typedef struct tagCSReqRepData CSREQREPDATA;
typedef struct tagCSReqRepData *LPCSREQREPDATA;

struct tagCSReqNewVersionS;
typedef struct tagCSReqNewVersionS CSREQNEWVERS;
typedef struct tagCSReqNewVersionS *LPCSREQNEWVERS;

struct tagCSSendPluginDataS;
typedef struct tagCSSendPluginDataS CSSENDPLUGINDATAS;
typedef struct tagCSSendPluginDataS *LPCSSENDPLUGINDATAS;


struct tagCSReqRepDataS;
typedef struct tagCSReqRepDataS CSREQREPDATAS;
typedef struct tagCSReqRepDataS *LPCSREQREPDATAS;

struct tagCSSendLinkDataS;

struct tagCSLinkData;
typedef struct tagCSLinkData CSLINKDATA;
typedef struct tagCSLinkData *LPCSLINKDATA;

struct tagCSLinkKey;
typedef struct tagCSLinkKey CSLINKKEY;
typedef struct tagCSLinkKey *LPCSLINKKEY;

struct tagCSLinkValue;
typedef struct tagCSLinkValue CSLINKVALUE;
typedef struct tagCSLinkValue *LPCSLINKVALUE;

/* msg head struct*/
struct tagCSMsgHead{
		unsigned int pkgLength;			   /*the total length of pkg*/
		unsigned int cmdID;			       /*thd cmd or proto*/
		unsigned int protoVersion; /*the version of proto*/	
};

/* msg body struct */
struct tagCSMsgBody{
	char data[MAX_SVR_PKG_LENGTH];			/*just a data length*/
};

/*struct of req new version*/
struct tagCSReqNewVersion{
	unsigned int version;	/*the client plugin version*/
};

/*struct of req report data*/
struct tagCSReqRepData{
	char data[MAX_SVR_PKG_LENGTH];
};

/*struct of notify client updating plugin or not*/
struct tagCSReqNewVersionS{
		unsigned int flag;
};

/*struct of send plugin data to client*/
struct tagCSSendPluginDataS{
	char md5Check[32];         /*check file md5 make sure file is not lost*/	
	char data[MAX_SVR_PKG_LENGTH];
};
struct tagCSSendNoNeedUpdateS{
	char content[16];/*offer some info*/
};
/*struct of response req report data*/
struct tagCSReqRepDataS{
   char data[MAX_SVR_PKG_LENGTH];
};



/*struct of hyper link text as a key ,usualy is a string of chinese*/
struct tagCSLinkKey{
	unsigned int keyLength;
	char keyData[MAX_LINK_LENGTH];
};

/*struct of hyper link as a value ,usualy is a website*/
struct tagCSLinkValue{
	unsigned int valueLength;
	char valueData[MAX_LINK_LENGTH];
};

/*struct of hyper link data*/
struct tagCSLinkData{
	unsigned int length;
	CSLINKKEY key;
	CSLINKVALUE value;
};
/*struct of all hyper link data*/
struct tagCSSendLinkDataS{
	unsigned int linkNum;   /*the num of hyperlink*/
	CSLINKDATA linkData[MAX_LINK_NUM]; /*the real data*/
};


class Proto{
    public:
		int  dealPkg(const char* pkg,int len,int &pkgType);
	    int  buildPkgByType(int type,char* buff,size_t &len);
	private:
		class ReadCfg *readCfgUtil;
		int buildIsUpdatePkg(char* buff,size_t &len);
		int buildSendPluginPkg(char* buff,size_t &len);
		int buildResReportDataPkg(char* buff,size_t &len);
		int buildSendLinkPkg(char* buff,size_t &len);
};
#endif
