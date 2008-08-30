/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_Mutex_h
#define maxmm_Mutex_h

#include <boost/thread.hpp>
#include <boost/utility.hpp>

namespace maxmm
{
	//! \brief Mutex class
    //! 
    //! uses boost::mutex
    //! This class follow boost guide line for mutex usage, by preventing access
    //! to acquire and release method. Those method are used by the rescource
    //! management class ScopeLock.
    //!
    class Mutex : private boost::noncopyable
	{
        // This class need access to the underlying boost::mutex
        //
        friend class ScopeLock;
	
	    private:
            //! \brief underlying primitive.
            //!
	    	boost::mutex       _boost_mtx;

	    public:
	    	
            //! \brief constructor.
            //!
            Mutex( void );
	    	
            //! \brief destructor.
            //!
            ~Mutex( void );
	    };
	    
    //! \brief Resource management class for lock acquisition.
    //!
    class ScopeLock : private boost::noncopyable
    {
	    friend class Condition;
	    
	    private:
	    	
            //! \brief reference to the mutex 
            Mutex&                      _mutex;
	    	
            //! \brief underlying boost::scoped_lock
            boost::mutex::scoped_lock*  _boost_lock;
	    
        public:
	    	
	    	//! \brief resource acquisition Constructor.
            //!
            //! This constructor lock the mutex.
            //! 
            //! \param mtx the mutex to use for locking.
            //! 
            ScopeLock (Mutex& mtx);
	    	
	    	
            //! \brief resource deallocation Destructor.
            //! 
            //! This destructor release the lock.
            //!
            ~ScopeLock( void );
        
        private:	
            /**
	    	 * lock() Lock the underlying Mutex.
	    	 * \warning if the mutex is already lock an exception will be thrown
	    	 */
	    	void lock( void );
	    	/**
	    	 * unlock()
	    	 */
	    	void unlock( void );
    };
}//namespace utils

#endif


