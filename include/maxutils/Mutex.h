/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_MUTEX_H
#define MAXUTILS_MUTEX_H

#include <boost/thread.hpp>

namespace maxutils
{
	/**
	 * Mutex class
	 */
	class Mutex
	{
	friend class ScopeLock;
	
	private:
		boost::mutex       m;

		Mutex(const Mutex& m);
	public:
		/**
		 * Constructor.
		 */
		Mutex();
		/**
		 * Destructor
		 */
		~Mutex();
	};
	/**
	 * ScopeLock class
	 * Enbles the locking of a mutex within a certain scope.
	 */
	class ScopeLock
	{
	friend class Condition;
	
	private:
		/**
		 * Reference to a Mutex.
		 */
		Mutex&                      mutex;
		boost::mutex::scoped_lock*  lock_p;
		ScopeLock(const ScopeLock& sl);
	public:
		/**
		 * Constructor
		 * Lock the underlying Mutex.
		 * @param _m a Mutex.
		 */
		ScopeLock (Mutex& _m);
		/**
		 * Destructor
		 * Unlock the underlying Mutex.
		 */
		~ScopeLock();
		/**
		 * lock() Lock the underlying Mutex.
		 * \warning if the mutex is already lock an exception will be thrown
		 */
		void lock();
		/**
		 * unlock()
		 */
		void unlock();
	};
}//namespace utils

#endif


