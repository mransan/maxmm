#include <RandomUniformTest.h>
#include <cmath>

namespace maxmm
{
    namespace test
    {
        void RandomUniformTest::setUp( void )
        {
            // No - Op.
        }

        void RandomUniformTest::tearDown( void )
        {
            // No - Op.
        }

        void RandomUniformTest::test_binary_generation( void )
        {
            maxmm::random::Uniform< int > rd_generator( 0 , 1 );
            int count_0 = 0 ; 
            int count_1 = 0 ;

            int iterations = 1000000 ; 
            for( int i = 0 ; i < iterations ; ++i )
            {
                int val = rd_generator( ) ; 
                switch( val )
                {
                    case 0 :
                        ++count_0 ; 
                        break;
                    case 1 :
                        ++ count_1 ;
                        break;
                    default:
                        break;
                }
            }
           
            double  diff = ( abs( count_0 - count_1) / ( double )iterations )  * 100  ;
            std::cout << "nb 0 : " << count_0 << std::endl;
            std::cout << "nb 1 : " << count_1 << std::endl;
            std::cout << "error : " <<  diff  << "% " << std::endl;
        
            CPPUNIT_ASSERT( diff < 1 ) ; 
        }


        void RandomUniformTest::test_10nb_generation( void )
        {
            maxmm::random::Uniform< int > rd_generator( 0 , 9 );
            std::vector< int >  count_vec( 10 );
        
            int iterations = 100000;
            for( int i = 0 ; i < iterations ; ++i )
            {
                ++count_vec[ rd_generator( ) ];
            }

            for( int i=0 ; i < count_vec.size( ) ; ++i )
            {
                int next = ( i + 1 ) % count_vec.size( );
                
                double diff = ( abs( count_vec[i] - count_vec[next] ) 
                                    / ( double ) iterations ) * 100;

                std::cout << "count [ " << i << " ] : " << diff << " % " << std::endl;
                CPPUNIT_ASSERT( diff < 1.0 );
            }
        }
        
        CppUnit::TestSuite* RandomUniformTest::getSuite( void )
        {
            CppUnit::TestSuite          *suite = new CppUnit::TestSuite();
            
            suite->addTest( new CppUnit::TestCaller< RandomUniformTest >( "RandomUniformTest::test_binary_generation", 
                                                              &RandomUniformTest::test_binary_generation 
                                                           ) ) ; 
            suite->addTest( new CppUnit::TestCaller< RandomUniformTest >( "RandomUniformTest::test_10nb_generation", 
                                                              &RandomUniformTest::test_10nb_generation 
                                                           ) );
            return suite;
        }
   
    }
}
