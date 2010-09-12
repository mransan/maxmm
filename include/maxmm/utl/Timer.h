/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_ScopeTimer_h
#define maxmm_ScopeTimer_h

#include <maxmm/Time.h>
namespace maxmm
{

//! \brief Timer class to measure time elapsed since a given point.
//!
class Timer
{
public:
    //! \brief Constructor set the timer origin to now.
    //!
    Timer(void)
    :   _start(Time::now())
    { }

    //! \brief Destructor.
    //!
    ~Timer(void)
    { }

    //! \brief return the time elapsed since the initial point.
    //!
    Time elapsed(void)
    {
        return Time::now() - _start;
    }

    //! \brief reset the init point to now.
    //!
    void reset(void)
    {
        _start = Time::now();
    }

private:
    
    //! \brief origin point.
    //!
    Time _start;
};
}


#endif
