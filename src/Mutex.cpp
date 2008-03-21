/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#include <maxutils/Mutex.h>

using namespace maxutils;

Mutex::Mutex()
:m()
{
}
Mutex::~Mutex()
{
}

ScopeLock::ScopeLock(Mutex& _m)
:mutex(_m)
{
	lock_p = new boost::mutex::scoped_lock(mutex.m);
}

ScopeLock::~ScopeLock()
{
	delete lock_p;
}

void ScopeLock::lock()
{
	lock_p->lock();
}

void ScopeLock::unlock()
{
	lock_p->unlock();
}
