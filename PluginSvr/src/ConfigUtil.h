#ifndef _CONFIG_UTIL_H_
#define _CONFIG_UTIL_H_
#include "Config.h"
#include "SingletonTemplate.h"
class ConfigUtil
{
	public:
		ConfigUtil();
		~ConfigUtil();
		int ReadCfg();
		void SetCfgFile(const char* cfgFile);
		const char* GetServerIp();
		const char* GetLogDir();
		const int GetServerPort();
		const int GetLogLevel();
		const int GetServerVersion();
	private:
		char cfgFileName[128];
		char serverIp[16];
		int serverPort;
		char logDir[128];
		int logLevel;
		int serverVersion;
		CConfigFile *configFile;
};
#define CONFIG_UTIL Singleton<ConfigUtil>::instance()
#endif
