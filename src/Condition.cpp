/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/Condition.h>


namespace maxmm
{

    Condition::Condition( Mutex& mutex )
    :_mutex( mutex )
    {
        // No - Op.
    }
    
    void Condition::wait( void )
    {
        boost::mutex::scoped_lock lock( _mutex._boost_mtx );
        _condition.wait( lock );
    }
    
    void Condition::broadcast( void )
    {
    	_condition.notify_all( );
    }
    
    void Condition::notify_one( void )
    {
        _condition.notify_one( );
    }
    Mutex& Condition::mutex( void )
    {
    	return _mutex;
    }
    Condition::~Condition( void )
    {
        // No - Op.
    }
}


