/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_Thread_h
#define maxmm_Thread_h

#include <memory>

#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <boost/bind.hpp>

#include <maxmm/Mutex.h>

namespace maxmm
{
    
    class IThread
    {
    public:
        IThread( void ){ }
        virtual ~IThread( void ){ }
        virtual bool start( void ) = 0;
        virtual void stop( void ) = 0;
        virtual void join( void ) = 0;
    };
    
    //! \brief thread abstraction class. 
    //!
    template< class Controller >
    class Thread : 
        private boost::noncopyable , 
        public IThread
    {
        public:
            
            //! \brief Constructor.
            //!
            Thread( const Controller &controller );
            
            //! \brief Destructor.
            //!
            virtual ~Thread( void );
            
            //! \brief this method start the run method in a separated thread.
            //!
            //! return true if the thread was started correctly, false otherwise.
            //!
            bool start( void );
            
            //! \brief join the thread ( i.e. wait for its termination );
            //!
            void  join( void );
            
            //! \brief Stop the thread.
            //!
            //! Mutex protected method 
            //!
            void stop( void ) ;
            
            
            //! \brief set the should stop flag.
            //!
            //!  Mutex protected method.
            //!
            bool should_stop ( void ) const;

            Controller &controller( void );
            
        protected:
        
            //! \brief pure virtual method that contains the code to be threaded.
            //!
            virtual void loop( void ) = 0;

            virtual void init( void ) = 0;

            virtual void clean( void ) = 0;
        
        
        private:
            void run( void );
            
            
            //! \brief indicate that the thread should stop.
            //!
            bool _should_stop;
            
            //! \brief mutex to protec the _should_stop variable.
            //!
            mutable Mutex _should_stop_mtx;
            
            //! \brief pointer to the underlying boost thread.
            //!
            std::auto_ptr<boost::thread> _boost_thrd;
   
            //! \brief control the timing of the thread.
            //!
            Controller _controller;
    };

    
    template< class Controller >
    Thread<Controller>::~Thread( void )
    {
        // No - Op.
    }
    
    template< class Controller >
    Thread<Controller>::Thread( const Controller &controller )
    :   _boost_thrd( 0 ),  
        _should_stop( false ),
        _controller( controller )
    {
        // No - Op.
    }
    
    template< class Controller >
    bool Thread<Controller>::start( void )
    {
    	try
    	{
    		_boost_thrd.reset( 
                    new boost::thread( 
                        boost::bind (
                            &Thread< Controller >::run , 
                            this ) ) ) ;
    	}
    	catch( ... )
    	{
    		return false;
    	}
    	
    	return true;
    }
    
    template< class Controller >
    bool Thread<Controller>::should_stop( void ) const 
    {
    	ScopeLock lock( _should_stop_mtx );
    	return _should_stop;
    }
    
    template< class Controller >
    void Thread<Controller>::stop( void )
    {
    	ScopeLock lock( _should_stop_mtx );
        _should_stop = true;
    }
    
    template< class Controller > 
    void Thread<Controller>::join( void )
    {
    	_boost_thrd->join();
    }

    template< class Controller >
    void Thread<Controller>::run( void )
    {
        this->init( );
        
        while( 
               ( this->should_stop( ) == false ) && 
               ( _controller.execute( ) ) )
        {
            this->loop( );
        }
        
        this-> clean( );
    }

    template< class Controller >
    Controller &Thread<Controller>::controller( void )
    {
        return _controller;    
    }
}
#endif
