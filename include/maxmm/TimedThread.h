/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_TimedThread_h
#define maxmm_TimedThread_h

#include <maxmm/Thread.h>

namespace maxmm
{


    //! \brief thread that time control the execution of its loop method.
    //!
    class TimedThread : public Thread
    {
    public: 
        
        //! \brief Constructor.
        //!
        //! \param period the period of execution.
        //!
        TimedThread( double period );
        
        //! \brief Destructor,
        //!
        ~TimedThread( void );
        
        //! \brief implement the run method.
        //!
        virtual void run(); 
    
    protected:
        
        //! \brief user defined method which will be executed every specified
        //! period.
        //!
        virtual void  loop( void ) = 0;

        //! \brief period in sec of the thread execution method.
        //!
        double        _period;

    };


}//namespace maxmm



#endif

