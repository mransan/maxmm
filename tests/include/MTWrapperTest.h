/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_MTWRAPPER_TEST_H
#define MAXUTILS_MTWRAPPER_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <boost/random.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/crc.hpp>

#include <maxutils/MTWrapper.h>
#include <maxutils/TimedThread.h>

#define DATA_SIZE 100

class MTWrapperTest : public CppUnit::TestFixture
{
public:
    class Global
    {
    private:
        
        static Global               *m_instance;
        boost::mt19937               m_numGen;
        boost::uniform_int
                    <int>            m_distribution;
        boost::variate_generator
                    <boost::mt19937,
                     boost::uniform_int<int>
                    >                m_var; 
    
        Global();
        
    public:
        struct Data
        {
            std::size_t      m_size;
            typedef          boost::uint8_t TItem;
            typedef          boost::crc_optimal<    8, 
                                                    0x21, 
                                                    0xFF, 
                                                    0, 
                                                    false, 
                                                    false> CheckSumGen; 
 
            TItem           *m_block;
            TItem            m_checksum;

            Data();
            Data(const Data& d);
            ~Data();
            void reset();
            bool consistent();
        };
        struct  AssignRandom
        {
           AssignRandom();
           void operator()(Data::TItem &t);
        };
        struct Print
        {
            Print();
            void operator()(const Data::TItem &t);
        };


        maxutils::MTWrapper
                <Data>        m_data;
        
        typedef maxutils::HPRecReaderScopeLock
                    <Data>    DataLock;
        static Global&               instance();
        static void                  init();
        static void                  destroy();
        static int                   randomPeriod();
    };

    class TestThread : public maxutils::TimedThread
    {
        int                         m_id;
        bool                        m_assertFailed;
        maxutils::MTWrapper<Global::Data>
                    ::RetiredList       m_DataRetList;
        boost::function1
            <void , Global::Data*>      updateData;
    public:
        TestThread(int _id);
        ~TestThread();
        virtual void loop();
        bool hasFailed();
    };
     
    class ThreadStarter
    {
    public:
        ThreadStarter()
        {
        }
        void operator()(TestThread * t)
        {
            t->start();
        }
    };
    
    class ThreadJoiner
    {
    public:
        ThreadJoiner()
        {}
        void operator()(TestThread *t)
        {
            t->join();
        }
    };
    class ThreadStopper
    {
    public:
        ThreadStopper()
        {}
        void operator()(TestThread *t)
        {
            t->shouldStop(true);
        }
    };
    class ThreadDeleter
    {
    public:
        ThreadDeleter()
        :m_hasFailed(false)
        {
        }
        void operator()(TestThread *t)
        {
            if(t->hasFailed())
            {
                m_hasFailed = true;
            }
            delete t;
        }
        bool hasFailed()
        {
            return m_hasFailed;
        }
    private:
        bool m_hasFailed;
    
    };
   
public:
    typedef std::vector<TestThread *> ThreadVec;
    typedef ThreadVec::iterator       ThreadVecItr;

    MTWrapperTest();
    ~MTWrapperTest();
    void setUp();
    void tearDown();
    void testWrapper();
    static CppUnit::TestSuite* getSuite();

private:
    ThreadVec m_threads;
};

#endif
