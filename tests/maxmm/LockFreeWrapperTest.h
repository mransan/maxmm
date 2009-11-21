/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_LockFreeWrapperTest_h
#define maxmm_LockFreeWrapperTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <LockFreeWrapperTest_details.h>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>

namespace maxmm
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
            
            //! \brief collect thread information and perform statisticall
            //! analysis. Free resource as well.
            //!
            class ThreadDeleter
            {
            public:
                ThreadDeleter( void );
                void add_to_write_set( const double & d );
                void add_to_read_set( const double & d );
                void operator()(LockFreeTestThread *t);
                
                bool failed( void );
                double read_avg( void ) ;
                double write_avg( void );
            private:
                typedef boost::accumulators::accumulator_set<
                    double , 
                    boost::accumulators::stats< 
                        boost::accumulators::tag::mean > > TTimeSet;

                TTimeSet  _read_timeset;
                TTimeSet  _write_timeset;

                bool _failed;
            
            };
          
            //
            // END of NESTED CLASSES
            // ---------------------
            //

        public:
            typedef std::vector<LockFreeTestThread *> ThreadVec;
            typedef ThreadVec::iterator       ThreadVecItr;
        
            LockFreeWrapperTest( void );
            ~LockFreeWrapperTest( void );
            void setUp( void );
            void tearDown( void );
            
            void test_lock_free( void );
            void test_lock_mutex( void );
            
            bool test_lock( void );
            static CppUnit::TestSuite* getSuite();
        
        private:
            ThreadVec _threads;

            Data::TLockFreeValue _shared_data ;
            Data                 _shared_data_protected ;
            
            static const uint32_t TEST_DURATION;
        };
    }
}
#endif
