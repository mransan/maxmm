/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_StdContainerTest_h
#define maxmm_StdContainerTest_h

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace maxmm
{
    namespace test
    {
        class StdContainerTest :  public CppUnit::TestFixture 
        {
            public:
                void setUp( void );
                void tearDown( void );
                
                
                //! \brief this test shows different ways to create an
                //! std::vector< >:
                //!
                //! \li default constructed every object using the std::vector
                //! constructor with a size_t method parameter.
                //! \li use the reserve to allocate necessary memory
                //! \li use resize to default construct every element of the
                //! std::vector.
                //! \li use the std::fill_n to copy one value to every element
                //!  of the std::vector.
                //! \li use the std::generate_n method in combination with a
                //! functor( called generator ) to assign value to the vector
                //! elements. ( This generator can hold state and therefore
                //! assign different value to each element. ). 
                //! \li std::fill_n and std::generator_n can work with or
                //! without std::back_inserter( v ) iterator. If the vector was
                //! not previously allocated ( with reserve or resize ) the
                //! back_insertor must be used. If elements are allocated then a
                //! classical input iterator is sufficient.
                //!
                //! Benchmarking shows:
                //! \li time ( sec:0 , nsec:000072527 ) : constructor( ) + reserve  
                //! \li time ( sec:0 , nsec:056216257 ) : constructor( ) + resize
                //! \li time ( sec:0 , nsec:056311825 ) : constructor( size , T ) 
                //! \li time ( sec:0 , nsec:056330553 ) : constructor( ) + reserve + fill_n 
                //! \li time ( sec:0 , nsec:056764806 ) : constructor( size ) 
                //! \li time ( sec:0 , nsec:064239077 ) : constructor( ) + reserve + generate_n ( functor )
                //! \li time ( sec:0 , nsec:180670446 ) : constructor( ) + reserve + generate_n ( lambda ) 
                //! \li time ( sec:0 , nsec:225022744 ) : constructor( ) + back_inserter + fill_n 
                //! \li time ( sec:0 , nsec:226583238 ) : constructor( ) + back_inserter + generate_n ( functor )
                //! \li time ( sec:0 , nsec:309918750 ) : constructor( ) + back_inserter + generate_n ( lambda )
                //!
                void test_vector_creation( void );
                void test_list_creation( void );

                static CppUnit::TestSuite* getSuite( void );

            private:

        };
    }
}

#endif
