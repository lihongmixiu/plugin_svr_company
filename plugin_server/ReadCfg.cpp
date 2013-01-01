#include"ReadCfg.h"
#include"log.h"
void ReadCfg:: setVersion(const char *fileName)
{
	FILE *f = fopen(fileName,"r");
	char *content = (char*)malloc(100);
	if(content == NULL)
	{
		printf("malloc content failed\n");
	}
	memset(content,0,100);
	char *s = fgets(content,100,f);
	int a = atoi(s);
	svrVersion = a;
	free(content);
    
}

unsigned int ReadCfg::getVersion()
{
	return svrVersion;
}

int ReadCfg:: readXml(const char* xmlFileName)
{
	TiXmlDocument *myDocument = new TiXmlDocument(xmlFileName);
	if(myDocument == NULL)
	{
		Log::writeLog(3,"build xml dom model fail!");
		return -1;
	}
	myDocument->LoadFile();
	xmlDom = myDocument; /*set the xml document*/  
	TiXmlElement *rootElement = myDocument->RootElement();
	if(rootElement == NULL)
	{
		Log::writeLog(3,"root element not find!");
		return -1;
	}

	TiXmlElement *linkNumElement = rootElement->FirstChildElement();
	linkNum = atoll(linkNumElement->FirstChild()->Value());/*set the link number*/
	return 0;
}

int ReadCfg::getLinkText(int index,char *buff,size_t &len)
{
	if(index>linkNum)
	{
		Log::writeLog(3,"index out of boundry");
		return -1;
	}

	if(buff == NULL)
	{
		Log::writeLog(3,"buff is null");
		return -1;
	}
	assert(xmlDom!= NULL);
	TiXmlElement *linkNumElement = xmlDom->RootElement()->FirstChildElement();/*linkNum element*/
	for(int i=1;i<=linkNum;i++)
    {
       TiXmlElement *currLink = linkNumElement->NextSiblingElement();
       if(currLink!= NULL)
       {
          TiXmlElement *linkText = currLink->FirstChildElement();
		  if(linkText != NULL && index == i)/*if find ,then copy out and break the loop*/
		  {		
			  char *textValue = (char*)linkText->FirstChild()->Value();
			  //char endFlag = '\0';
			  //memcpy(textValue+strlen(textValue),&endFlag,1);
			  len = strlen(textValue)+4;/*add the length itself*/
			  size_t netLen = htonl(len);
			  memcpy(buff,&netLen,4);/*copy the length*/
              memcpy(buff+4,textValue,strlen(textValue));/*copy the data*/
			 
			  break;
		  }	
       }
              linkNumElement = currLink;
     }

	return 0;
}

int ReadCfg::getLinkValue(int index,char*buff, size_t &len)
{
	if(index>linkNum)
	{
		Log::writeLog(3,"index out of boundry");
		return -1;
	}
	
	if(buff == NULL)
	{
		Log::writeLog(3,"buff is null");
		return -1;
	}
	assert(xmlDom!= NULL);
	TiXmlElement *linkNumElement = xmlDom->RootElement()->FirstChildElement();/*linkNum element*/
	for(int i=1;i<=linkNum;i++)
    {
       TiXmlElement *currLink = linkNumElement->NextSiblingElement();
       if(currLink!= NULL)
       {
		 TiXmlElement *linkValue = currLink->FirstChildElement()->NextSiblingElement();

		  if(linkValue != NULL && index == i)/*if find ,then copy out and break the loop*/
		  {
			  char *valueData = (char*)linkValue->FirstChild()->Value();
			  len = strlen(valueData)+4;/*add the length itself*/
			  size_t netLen = htonl(len);
			  memcpy(buff,&netLen,4);/*copy the linkValue length*/
              memcpy(buff+sizeof(unsigned int),linkValue->FirstChild()->Value(),len-4);/*copy the linkValue Data*/
			  break;
		  }	
       }
              linkNumElement = currLink;
     }
	return 0;
}

int ReadCfg::getLinkNum()
{
	return linkNum;	
}
