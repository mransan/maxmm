/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_CONDITION_H
#define MAXUTILS_CONDITION_H

#include <maxutils/Mutex.h>

namespace maxutils
{
	/**
	 * Condition class
	 * This class enables event broadcasting.
	 */
	class Condition
	{
	private:
		boost::condition cd;
		ScopeLock&       sl;

		Condition(const Condition& cd);
		Condition();
	public:
		/**
		 * Constructor
		 * Initialize the condition with a scope lock
		 */
		Condition(ScopeLock& _sl);
		/**
		 * Destructor
		 */
		~Condition();	
		/**
		 * Wait for the condition to signal
		 */
		void wait();
		/**
		 * Broadcast a signal to all the condition waiting
		 */
		void broadcast();
		/**
		 * Return the undelying ScopeLock
		 */
		ScopeLock& scopeLock();
	};

}//namespace utils
#endif
