/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/Time.h>
#include <cmath>
#include <iomanip>
#include <iostream>

#if defined(__APPLE_CC__)
#include <sys/time.h>
#include <errno.h>
namespace
{
typedef uint8_t clockid_t;

const uint8_t CLOCK_REALTIME=1;

int clock_gettime(clockid_t dummy, struct timespec* ts)
{
    int ret = -1;

    if (dummy != CLOCK_REALTIME)
    {
        errno = EINVAL;
    }
    else
    {
        struct timeval tv;
        ::gettimeofday(&tv, 0);
        ts->tv_sec = tv.tv_sec;
        ts->tv_nsec = tv.tv_usec * 1000;
        ret = 0;
    }

    return ret;
}

const int TIMER_ABSTIME = 1;

int clock_nanosleep(
    clockid_t clock , 
    int flags , 
    const struct timespec *rqtp,
    struct timespec *rmtp)
{
    struct timespec ts;
    
    
    switch(flags)
    {
        case TIMER_ABSTIME:
            ::clock_gettime(CLOCK_REALTIME , &ts);
            ts.tv_sec -= rqtp->tv_sec;
            ts.tv_nsec -= rqtp->tv_nsec;
            if(ts.tv_nsec < 0)
            {
                --ts.tv_sec;
                ts.tv_nsec += maxmm::Time::NANOSEC_IN_SEC;
            }
        case 0:
            ts = *rqtp;
            break;
        default:
            errno = EINVAL;
            return -1 ;
    }
    
    return ::nanosleep(&ts , rmtp);
}
}

#endif


namespace maxmm
{
const long int Time::NANOSEC_IN_SEC  = 1000000000;
const long int Time::MICROSEC_IN_SEC = 1000000;
const long int Time::MILLISEC_IN_SEC = 1000;

Time::Time(void)
{
    _ts.tv_sec  = 0;
    _ts.tv_nsec = 0;
}

Time::Time(const Time& t)
{
    _ts.tv_sec  = t._ts.tv_sec;
    _ts.tv_nsec = t._ts.tv_nsec;
}

Time::Time(double sec)
{
    _ts.tv_sec  = 0;
    _ts.tv_nsec = 0;
   
    _ts = Time::toTimespec(sec) ;
}

Time & Time::operator=(const Time& t)
{
    _ts.tv_sec  = t._ts.tv_sec;
    _ts.tv_nsec = t._ts.tv_nsec;

    return *this;
}

Time Time::operator+=(const Time& t)
{
    _ts.tv_sec  += t._ts.tv_sec ;
    _ts.tv_nsec += t._ts.tv_nsec;

    if(_ts.tv_nsec >= Time::NANOSEC_IN_SEC)
    {
        _ts.tv_nsec -= Time::NANOSEC_IN_SEC;
        ++_ts.tv_sec ;
    }

    return *this;
}

Time Time::operator-=(const Time& t)
{
    _ts.tv_sec  -= t._ts.tv_sec ;
    _ts.tv_nsec -= t._ts.tv_nsec;

    if(_ts.tv_nsec < 0 )
    {
        _ts.tv_nsec += Time::NANOSEC_IN_SEC;
        --_ts.tv_sec ;
    }

    return *this;
}


bool operator==(const Time& lhs , const Time& rhs)
{
    if (
            (lhs._ts.tv_sec  == rhs._ts.tv_sec)
            &&
            (lhs._ts.tv_nsec == rhs._ts.tv_nsec)
      )
    {
        return true;
    }
    return false;
}

bool operator<(const Time & lhs , const Time & rhs)
{
    if(lhs._ts.tv_sec > rhs._ts.tv_sec)
    {
        return false;
    }
    if(lhs._ts.tv_sec < rhs._ts.tv_sec)
    {
        return true;
    }
    //
    // case where tv_sec are equal
    //
    if(lhs._ts.tv_nsec < rhs._ts.tv_nsec)
    {
        return true;
    }
    return false;
}

Time & Time::update()
{
    ::clock_gettime(CLOCK_REALTIME , &_ts);
    return *this;
}

double Time::to_double(void) const 
{
    return (_ts.tv_sec + ( _ts.tv_nsec / (double)NANOSEC_IN_SEC)) ;
}

Time Time::now()
{
    Time t;
    return t.update();
}

void Time::sleep(Time t)
{
    ::clock_nanosleep(
            CLOCK_REALTIME ,
            0              ,
            &t._ts  ,
            NULL);
}

void Time::sleep_until(const Time &t)
{
    clock_nanosleep(
            CLOCK_REALTIME ,
            TIMER_ABSTIME  ,
            &t._ts          ,
            NULL);
}

struct timespec Time::toTimespec (double sec)
{
    struct timespec ts;

    ts.tv_sec  = static_cast<time_t>(floor(sec));
    sec       -= ts.tv_sec;
    ts.tv_nsec = static_cast<long int>(sec*Time::NANOSEC_IN_SEC);

    return ts;
}

std::ostream& operator<< (std::ostream& o, const Time& t)
{
    return o << "time (sec:" << t._ts.tv_sec 
             << " , nsec:"  << std::setw(9)  << std::setfill('0') << t._ts.tv_nsec 
             << ")" << std::setfill(' ');
}

uint32_t Time::nano(void)
{
    Time t = Time::now();
    return t._ts.tv_nsec;
}

} // namespace maxmm
