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

#include <maxutils/LockFreeWrapper.h>
#include <maxutils/TimedThread.h>

namespace maxutils
{
    namespace test
    {
        class LockFreeWrapperTest : public CppUnit::TestFixture
        {
        public:
            //
            // NESTED CLASSES
            // --------------
            //
            
            //! \brief this is the struct that will be shared among all of the
            //!  thread.
            //!
            //! The class is just a vector of unsigned char with an associated
            //! checksum.
            //!
            class Data
            {
                public :
                    //
                    // Typedef
                    // ------
                    //
                    typedef   boost::uint8_t        TItem;
                    typedef          
                        boost::crc_optimal< 8, 
                                            0x21, 
                                            0xFF, 
                                            0, 
                                            false, 
                                            false > TCheckSumGenerator; 
                    typedef std::vector< TItem >    TItemVec;
                    
                    typedef LockFreeWrapper< Data >    TLockFreeValue;
                    typedef LockFreePtr< Data >        TLockFreePtr;
                   
                    
                    
                    //
                    // Methods.
                    // -------
                    //
                    
                    //! \brief Constructor.
                    //!
                    Data( void );
                    
                    //! \brief copy Constructor.
                    //!
                    Data(const Data& d);
                    
                    //! \brief destructor.
                    //!
                    ~Data( void );
                    
                    //! \brief reset the block of data to random value and compute
                    //!   the checksum
                    //!
                    void reset( void );
                    
                    //! \brief make sure that the block of data is consitent.
                    //! 
                    //! \return true if the data is consistent.
                    //!
                    bool consistent( void );
                
                private:
                    
                    TItemVec         _block;
                    TItem            _checksum;
                    
                    static const std::size_t SIZE;
            };
       

       
            //! \brief thread for this test.
            //! 
            //! This thread has a continuously assigned random period. 
            //! Its main loop is composed of a reading block and randomly a
            //! writing block.
            //! 
            class TestThread : public maxutils::TimedThread
            {
                public:
                    //! \brief Constructor. 
                    //!
                    TestThread( int _id );
                    
                    //! \brief Destructor.
                    //!
                    ~TestThread( void );
                    
                    //! \brief main loop of the thread.
                    //!
                    virtual void loop( void );
                    
                    //! \brief indicate if the thread has failed.
                    //!
                    //! \return bool if the thread has failed.
                    //!
                    bool failed( void );

                private: 
                    //! \brief unique ID of the thread.
                    //!
                    int                         _id;
                    
                    //! \brief keep track if the data was found inconsistent
                    //!  during the test.
                    bool                        _failed;
                    
                    //! \brief reference to the global data shared among 
                    //!  all the threads.
                    Data::TLockFreeValue        &_global;

                    //! \brief local retired list of retired object of the
                    //!  global data.
                    //!
                    LockFreeWrapper< Data >
                                ::TRetiredList  _data_retired_list;
                    
                    
                    //! \brief update the global data with the given pointer.
                    //! 
                    //! As stated in maxutils::LockFreeWrapper::update the
                    //! ownership is transfered and the client must not delete
                    //! this ptr.
                    //!
                    void update_global( Data * ptr ); 
            };
             
            class ThreadDeleter
            {
            public:
                ThreadDeleter( void )
                :_failed(false)
                {
                }
                void operator()(TestThread *t)
                {
                    if( t->failed( ) )
                    {
                        _failed = true;
                    }
                    delete t;
                }
                bool failed( void )
                {
                    return _failed;
                }
            private:
                bool _failed;
            
            };
          
            //
            // END of NESTED CLASSES
            // ---------------------
            //

        public:
            typedef std::vector<TestThread *> ThreadVec;
            typedef ThreadVec::iterator       ThreadVecItr;
        
            LockFreeWrapperTest();
            ~LockFreeWrapperTest();
            void setUp();
            void tearDown();
            void testWrapper();
            static CppUnit::TestSuite* getSuite();
        
        private:
            ThreadVec m_threads;
        };
    }
}
#endif
