#include <maxutils/all.h>
#include <TestCommon.h>
#include <boost/random.hpp>
#include <vector>
using namespace maxutils;

class Global : boost::noncopyable
{
private:
    
    static Global                  *m_instance;
    Global()
    :m_MTtestString(10)  ,
     m_dist(0.001 , 0.5) ,
     m_gen(static_cast<long unsigned int>(clock())),
     m_var(m_gen, m_dist)
    {
    }
    boost::mt19937                m_gen;
    boost::uniform_real<double>   m_dist;
    boost::variate_generator<boost::mt19937&,
                             boost::uniform_real<double>
                            >     m_var;
  
public:

    MTWrapper<std::string>          m_MTtestString;

    static Global& instance()
    {
        return *m_instance;
    }
    static void   init()
    {
        m_instance = new Global();
    }
    static void   destroy()
    {
        if (m_instance) delete m_instance;
    }
    static double randomPeriod()
    {
        return m_instance->m_var();
    }
};
Global* Global::m_instance = 0;


class TestThread : public TimedThread
{
    int                                 m_id;
    MTWrapper<std::string>::RetiredList m_stringRetiredList;
public:
    TestThread(int id) 
    : TimedThread(0.01),
      m_id(id)
    {
    }

    virtual void loop();
    
    ~TestThread()
    {
        for (MTWrapper<std::string>::RetiredListItr 
                itr  = m_stringRetiredList.begin();
                itr != m_stringRetiredList.end();
                itr ++
            )
        {
            delete *itr;
        }
    }
};

void TestThread::loop()
{
    m_period = Global::randomPeriod();
    LOG_DEBUG   << "(th : " 
                << m_id 
                <<  ") reseting loop period to : " << m_period << std::endl;
    
    {
        std::string *st;
        HPRecReaderScopeLock<std::string>( Global::instance().m_MTtestString,
                                           &st);
        LOG_INFO   << "(th : " 
                   << m_id 
                   << ") Global::m_MTtestString : " << *st << std::endl; 
    }
    if (m_period > 0.4)
    {
        std::stringstream ss;
        ss << "my period is " << m_period ; ss.flush();
        std::string *st = new std::string(ss.str());
 
        LOG_INFO   << "(th : " 
                   << m_id 
                   << ") updating Global::m_MTtestString : " << *st << std::endl;
        Global::instance().m_MTtestString.update(st , m_stringRetiredList);
    }
}   

int main()
{
    initLogger();
    Global::init();
    typedef std::vector<TestThread *> ThreadVec;
    typedef ThreadVec::iterator       ThreadVecItr;
    ThreadVec threadV;
    for (int i=0 ; i<50 ; i++)
    {
        TestThread *tt = new TestThread(i);
        threadV.push_back(tt);
    }
    for (ThreadVecItr 
            itr  = threadV.begin(); 
            itr != threadV.end();
            itr ++
        )
    {
        (*itr)->start();
    }
    sleep(20);
    for (ThreadVecItr 
            itr  = threadV.begin(); 
            itr != threadV.end();
            itr ++
        )
    {
        (*itr)->shouldStop(true);
        (*itr)->join();
        delete *itr;
    }
    
    
    Global::destroy();
    Logger::close();
}
