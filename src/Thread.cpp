/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/Thread.h>
#include <maxmm/Logger.h>

#include <boost/function.hpp>
#include <boost/bind.hpp>


namespace maxmm
{
    Thread::~Thread( void )
    {
        // No - Op.
    }
    
    Thread::Thread( void )
    :   _boost_thrd( 0 ),  
        _should_stop( false )
    {
        // No - Op.
    }
    
    bool Thread::start( void )
    {
    	try
    	{
    		_boost_thrd.reset( 
                    new boost::thread( 
                        boost::bind (
                            &Thread::run , 
                            this ) ) ) ;
    	}
    	catch( ... )
    	{
    		return false;
    	}
    	
    	return true;
    }
    
    bool Thread::should_stop( void ) const 
    {
    	ScopeLock lock( _should_stop_mtx );
    	return _should_stop;
    }
    
    void Thread::should_stop( bool b )
    {
    	ScopeLock lock( _should_stop_mtx );
        _should_stop = b;
    }
    
    bool Thread::join( void )
    {
    	_boost_thrd->join();
    }
}
