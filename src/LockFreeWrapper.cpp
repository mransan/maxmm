/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxutils/LockFreeWrapper.h>

namespace maxutils
{
    //
    // STATIC VARIABLES
    // ----------------
    //

    HzPtrRec* HzPtrRec::_head   = 0;
    int         HzPtrRec::_length = 0;

    //
    // METHODS
    // -------
    //
    
    //
    // Member Method.
    // -------------
    //
    HzPtrRec::HzPtrRec( void )
    {
        _next      = 0;
        _active    = 0;
        _hazard_ptr = 0;
    }

    const void * HzPtrRec::hazard_ptr( void ) const 
    {
        return _hazard_ptr;
    }
    
    void HzPtrRec::hazard_ptr( void * ptr )
    {
        _hazard_ptr = ptr ;
    }

    
    //
    // Static Methods
    // --------------
    //
    
    void  HzPtrRec::next( HzPtrRec *& hz_ptr )
    {
        
        hz_ptr = hz_ptr->_next;
    }

    HzPtrRec* HzPtrRec::head( void )
    {
        return _head;
    }

    HzPtrRec* HzPtrRec::acquire()
    {
        HzPtrRec *p = _head;
        for( ; p ; p=p->_next)
        {
            if (p->_active 
                 || 
                !__sync_bool_compare_and_swap(&p->_active, 0, 1))
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
            oldLength = _length;
        }
        while(!__sync_bool_compare_and_swap(&_length, oldLength, oldLength +1));
        
        p  = new HzPtrRec();
        p->_active     = 1;
        p->_hazard_ptr = 0;
        
        HzPtrRec *oldHead ;
        do
        {
            oldHead   = _head;   
            p->_next = oldHead; 
        }
        while(!__sync_bool_compare_and_swap(&_head, oldHead, p));
        return p;
    }
    
    void HzPtrRec::release(HzPtrRec *p)
    {
        p->_hazard_ptr = 0;
        while(!__sync_bool_compare_and_swap(&p->_active, 1, 0));
    }
    
    
}
