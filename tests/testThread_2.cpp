#include <iostream>
#include <vector>

#include <maxutils/all.h>

using namespace maxutils;

typedef std::vector<int>           IntArray;
typedef std::vector<int>::iterator IntArrayIt;

class SharedData
{
friend std::ostream& operator<< (std::ostream&    o,
								 SharedData& d);
private:
	IntArray      m_intA;
	Mutex  m_intA_mtx;

	static const int Size = 10;
public:
	SharedData();
	void reset(int start);
};


SharedData::SharedData()
{
	for(int i=0 ; i< SharedData::Size ; i++)
	{
		m_intA.push_back(0);
	}
}
void SharedData::reset(int start)
{
	IntArrayIt iter;
	ScopeLock sl(m_intA_mtx);
	for (iter=m_intA.begin() ; iter!=m_intA.end() ; iter++)
	{
		*iter = start++; 
	}
}

std::ostream& operator<< (std::ostream&     o,
						  SharedData& d
						 )
{
	IntArrayIt iter;
	o << "( ";
	ScopeLock sl(d.m_intA_mtx);
	for (iter=d.m_intA.begin() ; iter!=d.m_intA.end() ; iter++)
	{
		o << *iter << ", ";
	}
	o << ")";
	return o;
}

class TestThread : public Thread
{
private:
	SharedData& m_SD;
	int         m_start;
public:
	~TestThread();	
	TestThread(SharedData& _SDi, int _start);
	virtual void run();
};

TestThread::~TestThread()
{
	
}
TestThread::TestThread(SharedData& _SD, int _start)
:m_SD(_SD) , m_start(_start)
{
}

void TestThread::run()
{
	while(!shouldStop())
	{
		m_SD.reset(m_start);
		std::cout << "SD : " << m_SD << std::endl;
		Time::sleep(0.1);
	}
}
int main()
{
	std::cout << "=============" << std::endl;
	std::cout << "testThread #2" << std::endl;
	std::cout << "=============" << std::endl;
	
	std::cout << std::endl;

	std::cout << "Threads Creation" << std::endl;
	SharedData SD;
	TestThread tt1(SD,0  );
	TestThread tt2(SD,-10);

	std::cout << "Threads Start"    << std::endl;
	tt1.start();
	tt2.start();
	std::cout << "sleeping 1s"     << std::endl;
	Time::sleep(1);
	std::cout << "Threads Stop"     << std::endl;
	tt1.shouldStop(true);
	tt2.shouldStop(true);
	std::cout << "Threads Join"     << std::endl;
	tt1.join();
	tt2.join();
	return 0;
}
