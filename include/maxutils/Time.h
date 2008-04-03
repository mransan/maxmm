/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_TIME_H
#define MAXUTILS_TIME_H

#include <iostream>
#include <time.h>


namespace maxutils
{
	/**
	 * Time Class
	 * This class is a time representation with all the possible operators. It contains also a static method for sleeping.
	 */
	class Time
	{
	
	friend Time operator+(Time t, double sec);
	friend Time operator-(Time t, double sec);

	friend std::ostream& operator<< (std::ostream& o,
									 const Time&   t
									);

	private:
		struct timespec ts;
		
		static struct timespec toTimespec(double sec);
	
	public:
		/**
		 * Number of nano seconds in 1 seconds.
		 */
		static const long int NANOSEC_IN_SEC ;
		/**
		 * Number of microseconds in 1 seconds.
		 */
		static const long int MICROSEC_IN_SEC;
		/**
		 * Number of milliseconds in 1 seconds.
		 */
		static const long int MILLISEC_IN_SEC;
		
		/**
		 * Constructor
		 * Create and initialize to 0 a time value.
		 */
		Time();
		/**
		 * Copy Constructor.
		 */
		Time(const Time& _t);
		/**
		 * Equal Operator.
		 */
		Time operator= (const Time&   _t);
		
        /**
		 * Equal Operator.
		 */
		Time operator= (const double  &_d);

        
        /**
		 * Operator Plus
		 */
		Time operator+ (const Time&   _t);
		/**
		 * Operator Minus.
		 */
		Time operator- (const Time&   _t);
		
		/**
		 * Operator +i=
		 * @param d : a double value.
		 */
		Time operator+=(const double&  d);
		/**
		 * Operator +=
		 * @param _t : a Time value.
		 */
		Time operator+=(const Time&   _t);
		/**
		 * Operator -=
		 * @param d :  a Time value.
		 */
		Time operator-=(const double&  d);
		/**
		 * Operator -=
		 * @param _t : a double value.
		 */
		Time operator-=(const Time&   _t);
		
		bool operator>=(const Time&   _t);
		bool operator<=(const Time&   _t);
		bool operator> (const Time&   _t);
		bool operator< (const Time&   _t);

        bool operator==(const Time&   _t);

        /**
		 * Update to the current time.
		 */
		void   update();

		/**
		 * Return the current time.
		 */
		static Time   now();
		/**
		 * sleep
		 * @param sec : The number of seconds it should sleep
		 */
		static void   sleep      (double sec);
		/**
		 * Sleep until a certaint time.
		 * @param sec : the time in seconds.
		 */
		static void   sleepUntil (double sec);
		/**
		 * Sleep until a certain time.
		 * @param t : a Time structure.
		 */
		static void   sleepUntil (Time   t  );

	};
}//namespace maxutils

#endif
