/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_TimeTT_h
#define maxmm_TimeTT_h

#include <boost/operators.hpp>

#include <time.h>
#include <stdint.h>

namespace maxmm
{
//!  \brief time class 
//!
//! This class is based on the struct timespec data defined in time.h and
//! and its corresponding function.
//!
class Time
    :   public boost::additive<Time> , 
        public boost::equality_comparable<Time> , 
        public boost::less_than_comparable<Time>

{
friend bool operator==(const Time & lhs , const Time & rhs);
friend bool operator< (const Time & lhs , const Time & rhs);
friend std::ostream& operator<<(std::ostream& o, const Time& t);

public:

    //! \brief Construcor 
    //!
    //! Initialise the time to 0 (the Epoch time).
    //!
    Time(void);
    
    //! \brief Copy Constructor.
    //!
    //! \param t the time to copy.
    //!
    Time(const Time& t);
    
    //! \brief constructor from double.
    //!
    //! \param d the time in sec.
    //!
    Time(double d );
    
    //! \brief assignement operator.
    //!
    //! \param t the time to copy.
    //! \return reference to *this.
    //!
    Time & operator= (const Time&   t);
    
    
    
    //! \brief operator +=
    //!
    Time operator+= (const Time&   _t);
    
    //! \brief operator -=
    //!
    Time operator-= (const Time&   _t);
    
    
    //! \brief update to the current time 
    //!
    //! This method uses ::clock_gettime() 
    //!
    //! \return  reference to *this.
    //!
    Time & update(void);

    double to_double(void)  const ;

    //! \brief current time.
    //!
    //! \return current time.
    //!
    static Time now(void);
    
    //! \brief sleep for the given number of seconds.
    //!
    //! This method uses ::clock_nanosleep()
    //!
    //! \param t Time to sleep.
    //!
    static void   sleep(Time t);
    
    //! \brief sleep until a given time.
    //!
    //! This method uses ::clock_nanosleep()
    //!
    //! \param t the time to sleep until in sec. If this time is before the
    //!  current time the method return imediately.
    //! 
    static void   sleep_until(const Time  &t);
    
    static uint32_t nano(void);
private:
    struct timespec _ts;
    
    //! \brief convert a double to a struct timespec.
    //!
    //! \param sec number of sec.
    //! \return the equivalient timespec struct.
    //!
    static struct timespec toTimespec(double sec);
    
public:
    //! \brief Number of nano seconds in 1 seconds.
    static const long int NANOSEC_IN_SEC ;
    
    //! \brief Number of microseconds in 1 seconds.
    static const long int MICROSEC_IN_SEC;
    
    //! \brief Number of milliseconds in 1 seconds.
    static const long int MILLISEC_IN_SEC;


};

}//namespace maxmm

#endif
