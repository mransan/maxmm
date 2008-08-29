/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_CONDITION_H
#define MAXUTILS_CONDITION_H

#include <maxutils/Mutex.h>
#include <boost/utility.hpp>

namespace maxutils
{
	//! \brief encapsulate Condition for thread synchronization.
    //!
	class Condition : boost::noncopyable
	{
	private:
		boost::condition _condition;
		ScopeLock&       _lock;

	public:
		//! \brief constructor.
        //!
        //! \param lock the lock that the condition should work with.
        //!
		Condition( ScopeLock& lock );
		
        
		//! \brief destructor.
        //!
        ~Condition( void );	
		
        //! \brief block until the condition is raised.
        //!
        void wait( void );
		
        //! \brief raise the condition to every thread waiting.
        //!
        void broadcast( void );
		
		//! \brief return  the lock that the condition is bind to.
        //!
        //! \return the lock that the condition is bind to.
        //!
        ScopeLock& scope_lock( void );
	};

}//namespace utils
#endif
