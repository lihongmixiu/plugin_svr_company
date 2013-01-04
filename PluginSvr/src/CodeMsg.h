#ifndef _CODE_MSG_H_
#define _CODE_MSG_H_
#include"Proto.h"
class CodeMsg
{
	public:

	/*pkg head making net bytes to host bytes  */
	 static void  HeadNtoh(tagCSMsgHead &msgHead);

	 /*pkg head making host bytes to net bytes*/
	static	void  HeadHton(tagCSMsgHead &msgHead);
				
};
#endif
