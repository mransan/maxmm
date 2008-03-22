#include <maxutils/all.h>
#include <TestCommon.h>


using namespace maxutils;

class TestTimedThread : public TimedThread
{
private:
	int m_iter;
public:
	
	TestTimedThread(double period);
	TestTimedThread(double period, int _iter);
	virtual void loop();
	~TestTimedThread();
};

TestTimedThread::~TestTimedThread()
{
}

TestTimedThread::TestTimedThread(double period)
: TimedThread(period), m_iter(0)
{
}

TestTimedThread::TestTimedThread(double period, int _iter)
: TimedThread(period), m_iter(_iter)
{
}

void TestTimedThread::loop()
{
    std::cout << "iter : " << m_iter << std::endl;
    m_iter ++;
}

int main()
{
	initLogger();
    
    LOG_INFO  << "=============" << std::endl;
	LOG_INFO  << "testThread #1" << std::endl;
	LOG_INFO  << "=============" << std::endl;
	
	LOG_INFO << std::endl;

	LOG_INFO << "Thread Creation" << std::endl;
    TestTimedThread tt(0.001, 10);
	LOG_INFO << "Thread Start"    << std::endl;
	tt.start();
	LOG_INFO << "sleeping 3s"     << std::endl;
	Time::sleep(3);
	LOG_INFO << "Thread Stop"     << std::endl;
	tt.shouldStop(true);
	LOG_INFO << "Thread Join"     << std::endl;
	tt.join();

	return 0;
}
