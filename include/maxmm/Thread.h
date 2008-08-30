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

#include <maxmm/Mutex.h>

namespace maxmm
{
    //! \brief thread abstraction class. 
    //!
    class Thread : private boost::noncopyable
    {
    protected:
        //! \brief indicate that the thread should stop.
        //!
        bool                         _should_stop;
        
        //! \brief mutex to protec the _should_stop variable.
        //!
        mutable Mutex                        _should_stop_mtx;
        
        //! \brief pointer to the underlying boost thread.
        //!
        std::auto_ptr<boost::thread> _boost_thrd;
    
    public:
        
        //! \brief Constructor.
        //!
        Thread( void );
        
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
        //! \return true if the thread was correctly joined.
        //!
        bool join( void );
        
        //! \brief indicate that the thread should stop.
        //!
        //! Mutex protected method 
        //!
        //! \return true if the thread should stop.
        //!
        bool should_stop( void ) const ;
        
        
        //! \brief set the should stop flag.
        //!
        //!  Mutex protected method.
        //!
        void should_stop (bool b );
        
        //! \brief pure virtual method that contains the code to be threaded.
        //!
        virtual void run() = 0;

    };

}//namespace utils


#endif
