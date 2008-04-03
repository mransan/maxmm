/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxutils/Time.h>
#include <cmath>

using namespace maxutils;

const long int Time::NANOSEC_IN_SEC  = 1000000000;
const long int Time::MICROSEC_IN_SEC = 1000000;
const long int Time::MILLISEC_IN_SEC = 1000;

Time::Time()
{
	ts.tv_sec  = 0;
	ts.tv_nsec = 0;
}

Time::Time(const Time& t)
{
	ts.tv_sec  = t.ts.tv_sec;
	ts.tv_nsec = t.ts.tv_nsec;
}

Time Time::operator=(const Time& t)
{
	ts.tv_sec  = t.ts.tv_sec;
	ts.tv_nsec = t.ts.tv_nsec;

	return *this;
}

Time Time::operator=(const double& t)
{
	
    double sec; 
    ts.tv_sec  = static_cast<__time_t>(floor(t));
	
    sec = t - floor(t);
    
    ts.tv_nsec = static_cast<long int>(sec*Time::NANOSEC_IN_SEC);

    if (ts.tv_nsec < 0)
	{
		ts.tv_sec --;
		ts.tv_nsec += Time::NANOSEC_IN_SEC;
	}

	return *this;
}

Time Time::operator+(const Time& _t)
{
	Time ret;
	ret.ts.tv_sec  = ts.tv_sec  + _t.ts.tv_sec ;
	ret.ts.tv_nsec = ts.tv_nsec + _t.ts.tv_nsec;
	if(ret.ts.tv_nsec >= Time::NANOSEC_IN_SEC)
	{
		ret.ts.tv_nsec -= Time::NANOSEC_IN_SEC;
		ret.ts.tv_sec  += 1;
	}
	return ret;
}

Time Time::operator-(const Time& _t)
{
	Time ret;
	ret.ts.tv_sec  = ts.tv_sec  - _t.ts.tv_sec ;
	ret.ts.tv_nsec = ts.tv_nsec - _t.ts.tv_nsec;
	if(ret.ts.tv_nsec < 0)
	{
		ret.ts.tv_nsec += Time::NANOSEC_IN_SEC;
		ret.ts.tv_sec  -= 1;
	}
	return ret;
}

Time Time::operator+=(const double& d)
{
	*this = *this + d;
	return  *this;
}

Time Time::operator+=(const Time& _t)
{
	ts.tv_sec  += _t.ts.tv_sec ;
	ts.tv_nsec += _t.ts.tv_nsec;
	if (ts.tv_nsec >= Time::NANOSEC_IN_SEC)
	{
		ts.tv_nsec -= Time::NANOSEC_IN_SEC;
		ts.tv_sec  += 1;
	}

	return *this;
}

bool Time::operator>=(const Time& _t)
{
	Time t = *this - _t;
	if (t.ts.tv_sec >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Time::operator>(const Time& _t)
{
	Time t = *this - _t;
	if ( (t.ts.tv_sec > 0) || ((t.ts.tv_sec == 0) && (t.ts.tv_nsec > 0))) 
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Time::operator==(const Time& _t)
{
	if ( (ts.tv_sec  == _t.ts.tv_sec)
          &&
         (ts.tv_nsec == _t.ts.tv_nsec)
       )
    {
        return true;
    }
    return false;
}

bool Time::operator<=(const Time& _t)
{
	Time t = *this - _t;
	if ((t.ts.tv_sec < 0 ) || ((t.ts.tv_sec == 0 ) &&(t.ts.tv_nsec == 0)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Time::operator<(const Time& _t)
{
	Time t = *this - _t;
	if (t.ts.tv_sec < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
Time Time::operator-=(const double& d)
{
	*this = *this - d;
	return  *this;
}

Time Time::operator-=(const Time& _t)
{
	ts.tv_sec  -= _t.ts.tv_sec ;
	ts.tv_nsec -= _t.ts.tv_nsec;
	if (ts.tv_nsec < 0)
	{
		ts.tv_nsec += Time::NANOSEC_IN_SEC;
		ts.tv_sec  -= 1;
	}

	return *this;
}

void Time::update()
{
	int ret = clock_gettime(CLOCK_REALTIME , &ts);
}

Time Time::now()
{
	Time t;
	t.update();
	return t;
}

void Time::sleep(double sec)
{
	struct timespec tsToSleep = toTimespec(sec);

	clock_nanosleep(CLOCK_REALTIME ,
					0              ,
					&tsToSleep      ,
					NULL);
}

void Time::sleepUntil(double sec)
{

	struct timespec tsToSleep = toTimespec(sec);

	clock_nanosleep(CLOCK_REALTIME ,
					TIMER_ABSTIME  ,
					&tsToSleep      ,
					NULL);
}

void Time::sleepUntil(Time t)
{


	clock_nanosleep(CLOCK_REALTIME ,
					TIMER_ABSTIME  ,
					&t.ts          ,
					NULL);
}


struct timespec Time::toTimespec (double sec)
{
	struct timespec ts;

	ts.tv_sec  = static_cast<__time_t>(floor(sec));
	sec       -= ts.tv_sec;
	ts.tv_nsec = static_cast<long int>(sec*Time::NANOSEC_IN_SEC);

	return ts;
}


Time maxutils::operator+(Time t , double sec) 
{
	Time ret;
	ret.ts.tv_sec = static_cast<__time_t>(t.ts.tv_sec + floor(sec));
	
	sec -= floor(sec);

	ret.ts.tv_nsec = static_cast<long int>(t.ts.tv_nsec + sec*Time::NANOSEC_IN_SEC);

	if (ret.ts.tv_nsec >= Time::NANOSEC_IN_SEC)
	{
		ret.ts.tv_sec ++;
		ret.ts.tv_nsec -= Time::NANOSEC_IN_SEC;
	}
	
	return ret;
}

Time maxutils::operator-(Time t , double sec) 
{
	Time ret;
	ret.ts.tv_sec = static_cast<__time_t>(t.ts.tv_sec - floor(sec));
	
	sec -= floor(sec);

	ret.ts.tv_nsec = static_cast<long int>(t.ts.tv_nsec - sec*Time::NANOSEC_IN_SEC);

	if (ret.ts.tv_nsec < 0)
	{
		ret.ts.tv_sec --;
		ret.ts.tv_nsec += Time::NANOSEC_IN_SEC;
	}
	
	return ret;
}

std::ostream& maxutils::operator<< (std::ostream& o, const Time& t)
{
	return o << "time(sec:" << t.ts.tv_sec 
		     << " , nsec:"   << t.ts.tv_nsec 
		     << ")";
}
