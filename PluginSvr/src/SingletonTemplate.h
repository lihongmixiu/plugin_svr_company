/******************************************************************************
  File Name       : SingletonTemplate.h
  Version         : 1.0
  Author          : penghao
  Created         : 2004-9-5
  Description     : singleton_template
  History         : 
  1 Date          : 2004-9-5
    Author        : penghao
    Modification  : Created file
******************************************************************************/

#ifndef __SINGLETON_TEMPLATE_H__
#define __SINGLETON_TEMPLATE_H__

#include <stdio.h>
#include <stdlib.h>
#include <new>

template <class TYPE>
class Singleton
{
	public:
		static TYPE * instance(void)
		{
			if(m_instance == NULL)
			{
				m_instance = new(std::nothrow) TYPE();
			}
			return m_instance;
		}
	private:
		Singleton();
		static TYPE* m_instance;

};


template<class TYPE>
TYPE* Singleton<TYPE>::m_instance = NULL;

template<class TYPE>
Singleton<TYPE>::Singleton()
{

}


#endif /* __SINGLETON_TEMPLATE_H__ */
