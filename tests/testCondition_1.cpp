#include <iostream>

#include <maxutils/all.h>

using namespace maxutils;

class TestThread : public Thread
{
private:
	int               m_id;
	Condition& m_cd;	
public:
	
	TestThread(Condition& _cd, int _id);
	virtual void run();
};

TestThread::TestThread(Condition& _cd, int _id)
:m_cd(_cd) , m_id(_id)
{
}

void TestThread::run()
{
	if (m_id == 1)
	{
		std::cout << "Thread id:" << m_id << "sleeeping"
				  << std::endl;
		Time::sleep(2.0);
		std::cout << "Thread id:" << m_id << " broadcasting"
				  << std::endl;
		m_cd.broadcast();
	}
	else
	{
		std::cout << "Thread id: " << m_id 
				  << " wainting for condition" 
				  << std::endl;
		m_cd.wait();
		std::cout << "Thread id: " << m_id 
				  << " end waiting for condition"
				  << std::endl;
	}
}
int main()
{
	std::cout << "================" << std::endl;
	std::cout << "testCondition #1" << std::endl;
	std::cout << "================" << std::endl;
	
	std::cout << std::endl;

	std::cout << "Mutex + ScopeLock + "
			  << "Condition + Thread "
			  << "Creation" << std::endl;
	Mutex     mutex;
	ScopeLock sl(mutex); 
	Condition condition(sl);
	TestThread tt1(condition , 1);
	TestThread tt2(condition , 2);

	std::cout << "Threads Start"    << std::endl;
	tt1.start();
	tt2.start();
	std::cout << "sleeping 10s"     << std::endl;
	Time::sleep(10);
	std::cout << "Threads Stop"     << std::endl;
	tt1.shouldStop(true);
	tt2.shouldStop(true);
	std::cout << "Threads Join"     << std::endl;
	tt1.join();
	tt2.join();
	return 0;
}
