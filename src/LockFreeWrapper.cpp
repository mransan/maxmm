/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/LockFreeWrapper.h>

namespace maxmm
{
    //
    // STATIC VARIABLES
    // ----------------
    //

    HzPtrRec* HzPtrRec::_head   = 0;
    volatile uint32_t HzPtrRec::_length = 0;

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
            if ( ( p->_active ) 
                 || 
                 ( apr_atomic_cas32( &p->_active , 1 , 0 ) != 0 ) )
            {
                continue;
            }
            else
            {
                return p;
            }
        }
    
        uint32_t oldLength;
        do
        {
            oldLength = _length;
        }
        while( oldLength != apr_atomic_cas32( &_length , (oldLength +1 ) , oldLength ) );
       
        p  = new HzPtrRec();
        p->_active     = 1;
        p->_hazard_ptr = 0;
        
        HzPtrRec *oldHead ;
        
        do
        {
            oldHead   = _head;   
            p->_next = oldHead;
        }
        while(oldHead != apr_atomic_casptr((volatile void **)&_head, p,  oldHead));

        
        return p;
    }
    
    void HzPtrRec::release(HzPtrRec *p)
    {
        p->_hazard_ptr = 0;
        //
        // before: 
        // while(!__sync_bool_compare_and_swap(&p->_active, 1, 0));
        //
        assert( apr_atomic_cas32(&p->_active, 0 , 1) != 0 );

    }
    
    
}
