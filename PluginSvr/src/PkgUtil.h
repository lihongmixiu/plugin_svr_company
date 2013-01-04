#ifndef _PKG_UTIL_
#define _PKG_UTIL_
class PkgUtil
{
	public:
		PkgUtil();
		~PkgUtil();
		int ProcessClientPkg(const char *inputBuff,int len,int &pkgType);
		int BuildPkgByType(int type,char *outputBuff,size_t &len);
	private:
		int BuildIsUpdatePkg(char* buff,size_t &len);
		int BuildSendPluginPkg(char* buff,size_t &len);
		int BuildResReportDataPkg(char* buff,size_t &len);
		int BuildSendLinkPkg(char* buff,size_t &len);
			
};
#endif
