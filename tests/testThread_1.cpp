#include <iostream>

#include <maxutils/all.h>

using namespace maxutils;

class TestThread : public Thread
{
private:
	int m_iter;
public:
	
	TestThread();
	TestThread(int _iter);
	virtual void run();
	~TestThread();
};

TestThread::~TestThread()
{

}


TestThread::TestThread()
{
}

TestThread::TestThread(int _iter)
:m_iter(_iter)
{
}

void TestThread::run()
{
	while(!shouldStop())
	{
		std::cout << "iter : " << m_iter << std::endl;
		Time::sleep(0.1);
		m_iter ++;
	}
}
int main()
{
	std::cout << "=============" << std::endl;
	std::cout << "testThread #1" << std::endl;
	std::cout << "=============" << std::endl;
	
	std::cout << std::endl;

	std::cout << "Thread Creation" << std::endl;
	TestThread tt(10);
	std::cout << "Thread Start"    << std::endl;
	tt.start();
	std::cout << "sleeping 1s"     << std::endl;
	Time::sleep(1);
	std::cout << "Thread Stop"     << std::endl;
	tt.shouldStop(true);
	std::cout << "Thread Join"     << std::endl;
	tt.join();

	return 0;
}
