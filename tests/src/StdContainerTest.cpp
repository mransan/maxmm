#include <StdContainerTest.h>
#include <TestCommon.h>

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>

#include <vector>
#include <map>
#include <algorithm>
#include <functional>

#include <maxmm/Time.h>
namespace maxmm
{
    namespace test
    {
        void StdContainerTest::setUp( void )
        {
            // No - Op.
        }
        
        void StdContainerTest::tearDown( void )
        {
            // No - Op.
        }

        //! \brief very simple class to work with class that do not have the
        //! default constructor.
        //!
        //! Since the only valid constructor has a parameter , lots of STL
        //! method cannot be called with default value.
        //!
        class IntConstruct
            : public boost::equality_comparable< IntConstruct >
        {
            public:
                IntConstruct( int i )
                {
                    _i = i ;
                }
                int value( void )
                {
                    return _i ;
                }
                friend bool operator==( 
                        const IntConstruct & lhs , 
                        const IntConstruct & rhs )
                { 
                    return lhs._i == rhs._i ;
                }
            private:
                int _i;
        };
       

        //! \brief class to generate int value. 
        //!
        //! The generator is initialised with a value which get incremented
        //! every time you operator( ) is called.
        //!
        struct IntGenerator
        {
            public :
                IntGenerator( int start )
                {
                    _start = start ;
                }
                int operator( )( void )
                {
                    return ++_start ;
                }
            private:
                int _start;
        };

        void StdContainerTest::test_vector_creation( void )
        {
            
            std::cout << std::endl;
            static std::size_t N = 10000000;
           

            std::map< maxmm::Time , std::string > benchmark;
            // 
            // Constructor + reserve + resize.
            //
            {
                maxmm::Time start = maxmm::Time::now( );
                std::vector< int >  vec( N );
                maxmm::Time end = maxmm::Time::now( );

                benchmark.insert(
                        std::make_pair(
                            end - start , 
                            "constructor( size )" ) );

                CPPUNIT_ASSERT_EQUAL( 
                        std::size_t( N ) , 
                        vec.size( ) );
            }
            
            {
                maxmm::Time start = maxmm::Time::now( );
                std::vector< int >  vec( N , 2 );
                maxmm::Time end = maxmm::Time::now( );

                benchmark.insert(
                        std::make_pair(
                            end - start , 
                            "constructor( size , T )" ) );

                CPPUNIT_ASSERT_EQUAL( 
                        std::size_t( N ) , 
                        vec.size( ) );
            }
           
            {
                maxmm::Time start = maxmm::Time::now( );
                std::vector< int > vec;
                vec.reserve( N );
                maxmm::Time end = maxmm::Time::now( );
                benchmark.insert(
                        std::make_pair(
                            end - start , 
                            "constructor( ) + reserve" ) );
               
                CPPUNIT_ASSERT_EQUAL( 
                        std::size_t( 0 ) , 
                        vec.size( ) );
            }
            
            {
                maxmm::Time start = maxmm::Time::now( );
                std::vector< int > vec;
                vec.resize( N );
                maxmm::Time end = maxmm::Time::now( );
                benchmark.insert(
                        std::make_pair(
                            end - start , 
                            "constructor( ) + resize" ) );
              
                CPPUNIT_ASSERT_EQUAL( 
                        std::size_t( N ) , 
                        vec.size( ) );
            }
            
            
            //
            // std::generate_n ( + reserve / back_inserter ).
            //
            {
                maxmm::Time start = maxmm::Time::now( );
                std::vector< int > vec;
                vec.reserve( N );
                std::generate_n( 
                        vec.begin( ) , 
                        N , 
                        boost::lambda::bind(
                            boost::lambda::constructor< int >( ) , 
                            2 ) ) ;
                maxmm::Time end = maxmm::Time::now( );
                benchmark.insert(
                        std::make_pair(
                            end - start , 
                            "constructor( ) + reserve + generate_n ( lambda )" ) );
                
                CPPUNIT_ASSERT( maxmm::test::do_not_contain( vec , 2 ) );
            }
            
            {
                maxmm::Time start = maxmm::Time::now( );
                std::vector< int > vec;
                std::generate_n( 
                        std::back_inserter( vec ) , 
                        N , 
                        boost::lambda::bind(
                            boost::lambda::constructor< int >( ) , 
                            2 ) ) ;
                maxmm::Time end = maxmm::Time::now( );
                benchmark.insert(
                        std::make_pair(
                            end - start , 
                            "constructor( ) + back_inserter + generate_n ( lambda )" ) );
                
                CPPUNIT_ASSERT( maxmm::test::do_not_contain( vec , 2 ) );
            }
          
            {
                maxmm::Time start = maxmm::Time::now( );
                std::vector< int > vec;
                vec.reserve( N );
                IntGenerator gen( -1 );
                std::generate_n( 
                        vec.begin( ) , 
                        N , 
                        gen );
                maxmm::Time end = maxmm::Time::now( );
                benchmark.insert(
                        std::make_pair(
                            end - start , 
                            "constructor( ) + reserve + generate_n ( functor )" ) );

                for( int i=0 ; i < N ; ++i )
                {
                    CPPUNIT_ASSERT_EQUAL( i , vec[ i ] );
                }
            }
            
            {
                maxmm::Time start = maxmm::Time::now( );
                std::vector< int > vec;
                IntGenerator gen( -1 );
                std::generate_n( 
                        std::back_inserter( vec ) , 
                        N , 
                        gen );
                maxmm::Time end = maxmm::Time::now( );
                benchmark.insert(
                        std::make_pair(
                            end - start , 
                            "constructor( ) + back_inserter + generate_n ( functor )" ) );

                for( int i=0 ; i < N ; ++i )
                {
                    CPPUNIT_ASSERT_EQUAL( i , vec[ i ] );
                }
            }
           
            //
            // std::fill_n
            //
            
            {
                maxmm::Time start = maxmm::Time::now( );
                std::vector< int > vec;
                vec.reserve( N );
                std::fill_n(
                        vec.begin( ) , 
                        N, 
                        2 );
                maxmm::Time end = maxmm::Time::now( );
                benchmark.insert(
                        std::make_pair(
                            end - start , 
                            "constructor( ) + reserve + fill_n " ) );

                CPPUNIT_ASSERT( maxmm::test::do_not_contain( vec , 2 ) );
            }
            
            {
                maxmm::Time start = maxmm::Time::now( );
                std::vector< int > vec;
                std::fill_n(
                        std::back_inserter( vec ) , 
                        N, 
                        2 );
                maxmm::Time end = maxmm::Time::now( );
                benchmark.insert(
                        std::make_pair(
                            end - start , 
                            "constructor( ) + back_inserter + fill_n " ) );

                CPPUNIT_ASSERT( maxmm::test::do_not_contain( vec , 2 ) );
            }
          
    
            for( std::map< maxmm::Time , std::string >::iterator 
                    itr  = benchmark.begin( ) ; 
                    itr != benchmark.end( );
                    ++itr )
            {
                std::cout << itr->first <<  " : " << itr->second << std::endl;
            }
        }

        CppUnit::TestSuite* StdContainerTest::getSuite( void )
        {
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();

            suite->addTest( new CppUnit::TestCaller<StdContainerTest>( 
                        "StdContainerTest::test_vector_creation", 
                        &StdContainerTest::test_vector_creation ) 
                        );
            return suite ;
        }
    }
}

