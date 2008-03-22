/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef __maxutils_MTWrapper_h__
#define __maxutils_MTWrapper_h__

#include <maxutils/Logger.h>
#include <boost/utility.hpp>
#include <list>
namespace maxutils
{
    
    

    class HPtrRecord
    {
        int                 m_active;

        static HPtrRecord  *m_head;
        static int          m_length;
        
        HPtrRecord()
        {
            m_next      = 0;
            m_active    = 0;
            m_hazardPtr = 0;
        }

    public:
        
        HPtrRecord         *m_next;
        void               *m_hazardPtr;
        
        static HPtrRecord* head()
        {
            return m_head;
        }
        
        static HPtrRecord* acquire();
       
        static void release(HPtrRecord *p);
    };
    
    template<typename DATA>
    class MTWrapper : public boost::noncopyable
    {
    private:
        DATA  *m_dataPtr;
        int    m_sizeLimit;
    public:
        typedef std::list<DATA *>     RetiredList;
        typedef typename RetiredList::iterator RetiredListItr;

        MTWrapper(int sizeLimit, DATA *dataPtr)
        {
            m_dataPtr   = dataPtr;
            m_sizeLimit = sizeLimit; 
        }
        ~MTWrapper()
        {
            if (m_dataPtr) delete m_dataPtr;
        }
        void update(DATA *newPtr, RetiredList &rlist)
        {
            rlist.push_back(m_dataPtr);
            m_dataPtr = newPtr;
            if (rlist.size() > m_sizeLimit)
            {
                LOG_INFO    << "retired list as reach limit size: "
                            << m_sizeLimit
                            << std::endl;
                removeRetired(rlist);
            }
        }
        
        void getPtr(DATA **ptr, HPtrRecord **hp)
        {
            while(!__sync_bool_compare_and_swap(ptr,*ptr,m_dataPtr));
            *hp = HPtrRecord::acquire();
            (*hp)->m_hazardPtr = *ptr; 
        }

        void removeRetired(RetiredList &rlist)
        {
            std::vector<void *> hazardPtrList;
            HPtrRecord *head = HPtrRecord::head();
            while(head)
            {
                void *p = head->m_hazardPtr;
                if(p)
                {
                    hazardPtrList.push_back(p);
                }
            
                head = head->m_next;
            }
            std::sort(  hazardPtrList.begin(), 
                        hazardPtrList.end()  , 
                        std::less<void *>()   );
            for ( RetiredListItr itr = rlist.begin();
                  itr               != rlist.end()  ;
                  itr ++)
            {
                if(!std::binary_search(hazardPtrList.begin(), hazardPtrList.end(), *itr))
                {
                    delete *itr;
                    itr = rlist.erase(itr);
                }
            }
        }       

    private:
        MTWrapper();
    };
    
}//namespace maxutils


#endif
