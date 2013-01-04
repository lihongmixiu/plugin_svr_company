#include <stdio.h>
#include "LogWrapper.h"
#include "ConfigUtil.h"

ConfigUtil::ConfigUtil()
{
	configFile = new CConfigFile();
	//snprintf(cfgFileName,sizeof(cfgFileName),"%s",cfgFile);
	
}

ConfigUtil::~ConfigUtil()
{
	delete configFile;
	configFile = NULL;
}

void ConfigUtil::SetCfgFile(const char* cfgFile)
{
    snprintf(cfgFileName,sizeof(cfgFileName),"%s",cfgFile);
}

int ConfigUtil::ReadCfg()
{
	int ret = 0;
	configFile->OpenFile(cfgFileName);
	if(configFile->IsOpen())
	{
		TRACESVR(GG_DEBUG,"config file open succ!");
	}
	else
	{
		TRACESVR(GG_ERROR,"config file open failed!");
		return -1;	
	}
	const char* serverSectionName = "ServerInfo";
	const char* logSectionName = "LogInfo";
	const char* versionSectionName = "VersionInfo";
	if(!configFile->GetItemValue(serverSectionName,"ServerIp",serverIp,16,"0.0.0.0"))
	{
		TRACESVR(GG_ERROR,"get server ip info failed!");
		return -1;
	}	
	TRACESVR(GG_DEBUG,"server ip is:%s",serverIp);
	if(!configFile->GetItemValue(serverSectionName,"ServerPort",serverPort))
	{
		TRACESVR(GG_ERROR,"get server port failed!");
		return -1;
	}
	TRACESVR(GG_DEBUG,"server port is:%d",serverPort);
	if(!configFile->GetItemValue(logSectionName,"LogDir",logDir,128,"./../log"))
	{
		TRACESVR(GG_ERROR,"get log dir info failed!");
		return -1;
	}
	TRACESVR(GG_DEBUG,"log dir is:%s",logDir);
	if(!configFile->GetItemValue(logSectionName,"LogLevel",logLevel,6))
	{
		TRACESVR(GG_ERROR,"get log level info failed!");
		return -1;
	}
	TRACESVR(GG_DEBUG,"log level is:%d",logLevel);
	if(!configFile->GetItemValue(versionSectionName,"ServerVersion",serverVersion))
	{
		TRACESVR(GG_ERROR,"get server version info failed!");
		return -1;
	}
	TRACESVR(GG_DEBUG,"server version is :%d",serverVersion);
	return ret;
}
const char* ConfigUtil::GetServerIp()
{
	return serverIp;
}

const char* ConfigUtil::GetLogDir()
{
	return logDir;
}

const int ConfigUtil::GetServerPort()
{
	return serverPort;
}

const int ConfigUtil::GetLogLevel()
{
	return logLevel;
}

const int ConfigUtil::GetServerVersion()
{
	return serverVersion;
}
