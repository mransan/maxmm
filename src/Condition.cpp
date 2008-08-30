/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/Condition.h>


namespace maxmm
{

    Condition::Condition( ScopeLock& lock )
    :_lock( lock )
    {
        // No - Op.
    }
    
    void Condition::wait( void )
    {
    	_condition.wait(* ( _lock._boost_lock ) );
    }
    
    void Condition::broadcast( void )
    {
    	_condition.notify_all( );
    }
    
    ScopeLock& Condition::scope_lock( void )
    {
    	return _lock;
    }
    Condition::~Condition( void )
    {
        // No - Op.
    }
}


