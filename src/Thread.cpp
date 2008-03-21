/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxutils/Thread.h>
#include <maxutils/Logger.h>

#include <boost/function.hpp>
#include <boost/bind.hpp>
using namespace maxutils;
Thread::~Thread()
{
}

Thread::Thread()
:m_thr_ptr(),  m_shouldStop(false)
{
}
bool Thread::start()
{
	boost::function0<void> f= boost::bind(&Thread::run,this);
	try
	{
		m_thr_ptr.reset(new boost::thread(f));
		LOG_DEBUG << "New thread successfully created" << std::endl;
	}
	catch(...)
	{
		LOG_ERROR << "Error while creating the thread " << std::endl;
		return false;
	}
	
	return true;
}

bool Thread::shouldStop()
{
	ScopeLock sl(m_shouldStopMutex);
	return m_shouldStop;
}

void Thread::shouldStop(bool b)
{
	ScopeLock sl(m_shouldStopMutex);
	LOG_DEBUG << "Should Stop value changed:" << b << std::endl;
	m_shouldStop = b;
}

bool Thread::join()
{
	m_thr_ptr->join();
}
