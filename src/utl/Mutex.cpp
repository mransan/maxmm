/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#include <maxmm/Mutex.h>

namespace maxmm
{

Mutex::Mutex(void)
:_boost_mtx()
{
}
Mutex::~Mutex(void)
{
}

ScopeLock::ScopeLock(Mutex& mtx)
:_mutex(mtx)
{
	_boost_lock = new boost::mutex::scoped_lock(mtx._boost_mtx);
}

ScopeLock::~ScopeLock(void)
{
	delete _boost_lock;
}

void ScopeLock::lock(void)
{
	_boost_lock->lock();
}

void ScopeLock::unlock(void)
{
	_boost_lock->unlock();
}
}
