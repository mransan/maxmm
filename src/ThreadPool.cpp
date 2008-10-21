/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/ThreadPool.h>
#include <maxmm/Mutex.h>

#include <boost/bind.hpp>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>

#include <assert.h>
namespace maxmm
{
    
    ThreadPool::Thread::Thread(
        Condition& condition , 
        ThreadPool& thread_pool)
    :   maxmm::Thread< maxmm::ConditionController>(
            maxmm::ConditionController( condition ) ) ,
        _thread_pool( thread_pool ),
        _work_counter( 0 )
    { }

    ThreadPool::Thread::~Thread( void )
    { }


    void ThreadPool::Thread::loop( void )
    { 
        IWork *work = _thread_pool.get_next_work( );
        if( 0 != work )
        {
            work->execute( );
            ++_work_counter;
            delete work;
            _controller.no_wait( );
        }
    }
    
    uint64_t ThreadPool::Thread::nb_works( void ) const
    {
        return _work_counter;
    }
    
    void ThreadPool::Thread::init( void )
    { }

    void ThreadPool::Thread::clean( void )
    { }
    
    //
    // Thread Pool
    // ----------
    //
    
    ThreadPool::ThreadPool( int nb_threads )
    :   _mutex( ) ,
        _condition( _mutex ),
        _stoped( true )
    { 
        std::generate_n(
            std::back_inserter( _threads ),
            nb_threads ,
            boost::lambda::bind(
                boost::lambda::new_ptr< ThreadPool::Thread >( ) , 
                boost::ref( _condition ) ,
                boost::ref( *this ) ) );
     
    }

    ThreadPool::~ThreadPool( void )
    {
        if( false == _stoped )
        {
            this->stop( );
        }
        
        std::for_each(
            _threads.begin( ), 
            _threads.end( ), 
            boost::lambda::bind(
                boost::lambda::delete_ptr( ),
                boost::lambda::_1 ) );
        _threads.clear( );
        
        std::for_each(
            _works.begin( ) , 
            _works.end( ), 
            boost::lambda::bind(
                boost::lambda::delete_ptr( ) ,
                boost::lambda::_1 ) );
        _works.clear( );
    }

    bool ThreadPool::append_work( IWork *work )
    { 
        {
            ScopeLock lock( _works_mtx );
            _works.push_back( work );
        }
        _condition.notify_one( );
        return true;
    }

    IWork *ThreadPool::get_next_work( void )
    {
        ScopeLock lock( _works_mtx );
        if( _works.empty( ) == true )
        {
            return 0;    
        }
        IWork * ret = _works.front( );
        _works.pop_front( );
        return ret;
    }

    void ThreadPool::start( void )
    {
        std::for_each(
            _threads.begin( ) , 
            _threads.end( ) , 
            boost::bind(
                &Thread::start ,
                _1 ) );
        _stoped = false;
        _condition.broadcast( );
    }

    void ThreadPool::stop( void )
    { 
        std::for_each(
            _threads.begin( ) , 
            _threads.end( ) , 
            boost::bind(
                &Thread::stop ,
                _1 ) );
         _condition.broadcast( );
         std::for_each(
            _threads.begin( ) , 
            _threads.end( ) , 
            boost::bind(
                &Thread::join ,
                _1 ) );

         _stoped = true;
    }

    std::vector< uint64_t > ThreadPool::execution_stats( void ) const 
    {
        assert( _stoped == true );
        std::vector< uint64_t > stats( _threads.size( ) );
        uint32_t ithread = 0;
        for( std::vector< ThreadPool::Thread* >::const_iterator
            it  = _threads.begin( ) ; 
            it != _threads.end( );
            ++it )
        {
            stats[ithread] = (*it)->nb_works( );
            ++ithread;
        }

        return stats;
    }
    

}

