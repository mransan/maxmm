/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_LockFreeWrapperTest_details_h
#define maxmm_LockFreeWrapperTest_details_h

#include <maxmm/Thread.h>
#include <maxmm/ThreadController.h>
#include <maxmm/Random.h>
#include <maxmm/LockFreeWrapper.h>

#include <boost/crc.hpp>

namespace maxmm
{
    namespace test
    {
        
            //! \brief this is the struct that will be shared among all of the
            //!  thread.
            //!
            //! The class encapsulate a block of byte with self checking and
            //! reseting functionality.
            //!
            class Data
            {
                public :
                    //
                    // Typedef
                    // ------
                    //
                    typedef   uint8_t        TItem;
                    typedef          
                        boost::crc_optimal< 16, 
                                            0x1021, 
                                            0xFFFF, 
                                            0, 
                                            false, 
                                            false > TCheckSumGenerator; 
                    typedef std::vector< TItem >    TItemVec;
                    
                    typedef LockFreeWrapper< Data >    TLockFreeValue;
                    typedef LockFreeReaderPtr< Data >  TLockFreeReaderPtr;
                    typedef LockFreeWriterPtr< Data >  TLockFreeWriterPtr;
                  
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
                    
                    void reset_locked( void ); 
                    
                    //! \brief make sure that the block of data is consitent.
                    //! 
                    //! \return true if the data is consistent.
                    //!
                    bool consistent( void ) const ;
               
                    bool consistent_locked( void ) const ;

                    uint16_t          _checksum;
                
                    TItem            *_block;
                private:
                    
                    mutable maxmm::Mutex _lock; 
                    static const std::size_t SIZE;
            };


        //! \brief base class for thread in the LockFreeWrapperTest test suite.
        //!
        class LockFreeTestThread :  public maxmm::Thread< NoWaitController >
        {
            public:
                
                //! \brief constructor.
                //!
                LockFreeTestThread( int id );
                
                //! \brief destructor.
                //!
                virtual ~LockFreeTestThread( void ); 
                
                //! \brief indicate if the thread has failed.
                //!
                //! \return bool if the thread has failed.
                //!
                bool failed( void );
    
                const std::vector< double > &read_times( void ) const ;
                const std::vector< double > &write_times( void ) const ;

            protected: 
                virtual void loop( void ) = 0;
                virtual void init( void );
                virtual void clean( void );
                
                //! \brief unique ID of the thread.
                //!
                int                         _id;
                
                //! \brief keep track if the data was found inconsistent
                //!  during the test.
                bool                        _failed;
                
                //! \brief random number generator to randomly decide to
                //! write to the global data structure.
                maxmm::random::Uniform< uint8_t > _rd_generator;
                
                std::vector< double > _read_times;
                std::vector< double > _write_times;
     
                static const uint8_t WRITE_THRESHOLD; 
        };
        

        //! \brief test thread for lock free structure testing.
        //!
        class LockFreeThread :  public LockFreeTestThread
        {
            public:
                LockFreeThread( int id , Data::TLockFreeValue& shared_data );
                virtual ~LockFreeThread( void );
                
                void loop( void );
            protected:
                
                //! \brief reference to the global data shared among 
                //!  all the threads.
                Data::TLockFreeValue &_shared_data;
                
                //! \brief local retired list of retired object of the
                //!  global data.
                //!
                LockFreeWrapper< Data >
                            ::TRetiredList  _data_retired_list;

        };

        //! \brief test thread for mutex like synchronization
        //!
        class MutexThread : public LockFreeTestThread
        {
            public :
                MutexThread( int id , Data& shared_data );
                virtual ~MutexThread( void );

                void loop( void );
            protected:
                
                Data &_shared_data;
        };
    
    }
}


#endif
