/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxutils/Condition.h>

using namespace maxutils;

Condition::Condition(ScopeLock& _sl)
:sl(_sl)
{
}

void Condition::wait()
{
	cd.wait(*(sl.lock_p));
}

void Condition::broadcast()
{
	cd.notify_all();
}

ScopeLock& Condition::scopeLock()
{
	return sl;
}
Condition::~Condition()
{
}

