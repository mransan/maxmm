#include <MTWrapperTest.h>
#include <algorithm>  // for std::for_each
#include <cstddef>    // for std::size_t

using namespace maxutils;

MTWrapperTest::Global* MTWrapperTest::Global::m_instance = 0 ;

MTWrapperTest::Global::Global()
:   m_distribution(0 , 255),
    m_numGen(static_cast<long unsigned int>(clock())),
    m_var(m_numGen, m_distribution),
    m_data(3)
{

}

MTWrapperTest::Global::Data::Data()
:m_size(DATA_SIZE)
{
    m_block = new TItem[m_size];
    CheckSumGen crc;
    crc = std::for_each(  m_block,
                            m_block + m_size,
                            crc
                     ) ;
   m_checksum = crc();
}

MTWrapperTest::Global::Data::Data(const Data &d)
{
    m_size  = d.m_size;
    m_block = new TItem[m_size];
    for(std::size_t i=0 ; i<m_size ; i++)
    {
        m_block[i] = d.m_block[i];
    }
    m_checksum = d.m_checksum;
}

MTWrapperTest::Global::Data::~Data()
{
    delete [] m_block;
}

bool MTWrapperTest::Global::Data::consistent()
{
     CheckSumGen crc;
     crc = std::for_each(  m_block,
                           m_block + m_size,
                           crc
                     ) ;
    if (m_checksum != crc())
    {
        return false;
    }
    else
    {
        return true;
    }
}


MTWrapperTest::Global::AssignRandom::AssignRandom()
{
}
void MTWrapperTest::Global::AssignRandom::operator()(Data::TItem &t)
{
    int tmp = MTWrapperTest::Global::randomPeriod();
    t = static_cast<Data::TItem>(tmp);
}


void MTWrapperTest::Global::Data::reset()
{
    std::for_each(    m_block,
                      m_block + m_size, 
                      MTWrapperTest::Global::AssignRandom()
                 );
    CheckSumGen crc;
    crc = std::for_each(  m_block,
                          m_block + m_size,
                          crc
                     ) ;
    m_checksum = crc();
    crc.reset();
}

MTWrapperTest::Global& MTWrapperTest::Global::instance()
{
    return *m_instance;
}

void MTWrapperTest::Global::init()
{
    m_instance = new MTWrapperTest::Global();
}
void MTWrapperTest::Global::destroy()
{
    if(m_instance) delete m_instance;
}

int MTWrapperTest::Global::randomPeriod()
{
    return m_instance->m_var();
}

MTWrapperTest::TestThread::TestThread(int _id)
:m_id (_id), 
m_assertFailed(false),
TimedThread(0.1)
{
    updateData = boost::bind( &MTWrapper<MTWrapperTest::Global::Data>
                                                    ::update,
                              &MTWrapperTest::Global::instance().m_data,
                              _1,
                              m_DataRetList);

}
MTWrapperTest::TestThread::~TestThread()
{
    for (MTWrapper<MTWrapperTest::Global::Data>::RetiredListItr 
          itr  = m_DataRetList.begin();
          itr != m_DataRetList.end();
          itr ++
      )
    {
        delete *itr;
    }

}
bool MTWrapperTest::TestThread::hasFailed()
{
    return m_assertFailed;
}

void MTWrapperTest::TestThread::loop()
{
    m_period = static_cast<double>(Global::randomPeriod()%40 + 10)/1000.0;
    LOG_DEBUG   << "(th : " 
                << m_id 
                <<  ") reseting loop period to : " << m_period << std::endl;
   
    //Reading block 
    {
        MTWrapperTest::Global::Data *d;
        MTWrapperTest::Global::DataLock sl ( Global::instance().m_data,
                                                &d);
        //LOG_INFO << "reading checksum: " << static_cast<int>(d->m_checksum) << std::endl;
        if(!d->consistent())
        {
            m_assertFailed = true;
        }
    }   

    //writing block
    if (m_period > 0.0025)
    {
        MTWrapperTest::Global::Data *newData;
        {
            MTWrapperTest::Global::Data *d;
            MTWrapperTest::Global::DataLock sl ( Global::instance().m_data,
                                                    &d);
            newData = new MTWrapperTest::Global::Data(*d);
        }   
        newData->reset();
        //LOG_INFO << "reseting checksum: " << static_cast<int>(newData->m_checksum) << std::endl;
        updateData(newData);
    }
}


MTWrapperTest::MTWrapperTest()
{
}
MTWrapperTest::~MTWrapperTest()
{
}
void MTWrapperTest::setUp()
{
    Global::init();
    //Reading block 
    {
        MTWrapperTest::Global::Data *d;
        MTWrapperTest::Global::DataLock sl ( Global::instance().m_data,
                                                &d);
        d->reset();
    }   
    for(int i=0 ; i<30 ; i++)
    {
        m_threads.push_back(new TestThread(i));
    }
    std::for_each(m_threads.begin(), m_threads.end(), ThreadStarter() );
}
void MTWrapperTest::tearDown()
{
    std::for_each(m_threads.begin(), m_threads.end(), ThreadStopper());
    std::for_each(m_threads.begin(), m_threads.end(), ThreadJoiner());
    ThreadDeleter td;
    td = std::for_each(m_threads.begin(), m_threads.end(), td);
    m_threads.clear();
    Global::destroy();
    CPPUNIT_ASSERT_MESSAGE("checksum failed" , td.hasFailed()==false);
}
void MTWrapperTest::testWrapper()
{
    sleep(30);
}

CppUnit::TestSuite* MTWrapperTest::getSuite()
{
    
    CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
    
    suite->addTest( new CppUnit::TestCaller<MTWrapperTest>( "testWrapper", 
                                                      &MTWrapperTest::testWrapper 
                                                   ) 
                );
    return suite;
}

