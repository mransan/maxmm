/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_THREAD_H
#define MAXUTILS_THREAD_H

#include <memory>

#include <boost/thread.hpp>

#include <maxutils/Mutex.h>

namespace maxutils
{
    /**
     * Thread Class
     * In order to create your thread, inherit from this class and overide the run method.
     */
    class Thread
    {
    protected:
        /**
         * Variable to indicate to the run() method that it should stop;
         */
        bool                         m_shouldStop;
        /**
         * Mutext to protect access to the m_shouldStop variable.
         */
        Mutex                        m_shouldStopMutex;
        /**
         * Pointer to a boost::thread.
         */
        std::auto_ptr<boost::thread> m_thr_ptr;
    
    public:
        /**
         * Constructor
         */
        Thread();
        /**
         * Destructor
         */
        virtual ~Thread();
        
        /**
         * Stat the Thread.
         * This function star the run function in its owns thread.
         * @return true if the thread was successfully created.
         */
        bool start();
        /**
         * Join the thread - wait for its completion.
         */
        bool join();
        /**
         * Indicate if the thread should stop.
         */
        bool shouldStop();
        /**
         * Set the should stop variable.
         */
        void shouldStop(bool b);
        
        /**
         * Function that will be executed in a separate thread.
         */
        virtual void run() = 0;
    };

}//namespace utils


#endif
