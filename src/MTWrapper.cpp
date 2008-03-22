/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxutils/MTWrapper.h>

using namespace maxutils;

HPtrRecord* HPtrRecord::acquire()
{
    HPtrRecord *p = m_head;
    for( ; p ; p=p->m_next)
    {
        if (p->m_active 
             || 
            !__sync_bool_compare_and_swap(&p->m_active, 0, 1))
        {
            continue;
        }
        else
        {
            return p;
        }
    }

    int oldLength;
    do
    {
        oldLength = m_length;
    }
    while(!__sync_bool_compare_and_swap(&m_length, oldLength, oldLength +1));
    
    p  = new HPtrRecord();
    p->m_active    = 1;
    p->m_hazardPtr = 0;
    
    HPtrRecord *oldHead ;
    do
    {
        oldHead = m_head;    
    }
    while(!__sync_bool_compare_and_swap(&m_head, oldHead, p));
    return p;
}

void HPtrRecord::release(HPtrRecord *p)
{
    p->m_active    = 0;
    p->m_hazardPtr = 0;
}


HPtrRecord* HPtrRecord::m_head   = 0;
int         HPtrRecord::m_length = 0;
